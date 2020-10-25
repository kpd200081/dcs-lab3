#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <QDebug>

#include <errno.h>
#include <fcntl.h>
#include <string>
#include <unistd.h>

typedef struct {
    char data[256];
    double res;
    int flg; // 0-written, 1-computed, 2-done
} Expr;

static Expr* shm = NULL;
static const int shmBufSize = 10;

int main(int, char*[]) {
    key_t msgKey = ftok("../lab3.pro", 1);
    key_t memKey = ftok("../lab3.pro", 2);
    key_t semKey = ftok("../lab3.pro", 3);
    int msgs = msgget(msgKey, 0);
    int mem = shmget(memKey, sizeof(Expr) * shmBufSize, IPC_CREAT | IPC_EXCL | 0660);
    int memSm = semget(semKey, 1, IPC_CREAT | IPC_EXCL | 0660);
    shm = reinterpret_cast<Expr*>(shmat(mem, nullptr, 0));
    if((intptr_t) shm == -1) {
        qCritical() << "Can't create mem, errno: " << errno;
        exit(-1);
    }
    for(int i = 0; i < shmBufSize; i++) {
        shm[i] = Expr { .res = 0, .flg = 2 };
        memset(shm[i].data, 0, 256);
    }
    while(true) {
        char data[256];
        int size = msgrcv(msgs, data, 256, 0, IPC_NOWAIT);
        if(size == -1) {
            if(errno == EINVAL || errno == EIDRM) {
                break;
            }
            continue;
        }
        QString exp = QString::fromUtf8(data, size);
        qDebug() << "rcv: " << exp;
        sembuf op = { .sem_num = 0, .sem_op = 0, .sem_flg = 0 };
        semop(memSm, &op, 1);
        op.sem_op = 1;
        semop(memSm, &op, 1);
        bool found = false;
        for(int i = 0; i < shmBufSize; i++) {
            if(shm[i].flg == 2) {
                memset(shm[i].data, 0, 256);
                strcpy(shm[i].data, data);
                shm[i].flg = 0;
                found = true;
                break;
            }
        }
        if(!found) {
            qDebug() << "Skip. No place in shmem buffer";
        }
        op.sem_op = -1;
        semop(memSm, &op, 1);
    }

    shmdt(shm);
    semctl(memSm, 1, IPC_RMID, nullptr);
    shmctl(mem, IPC_RMID, nullptr);
    return 0;
}
