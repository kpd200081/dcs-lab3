#include "solver/solver.h"

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
    key_t memKey = ftok("../lab3.pro", 2);
    key_t semKey = ftok("../lab3.pro", 3);
    int mem = shmget(memKey, sizeof(Expr) * shmBufSize, 0);
    int memSm = semget(semKey, 1, 0);
    shm = reinterpret_cast<Expr*>(shmat(mem, nullptr, 0));
    if((intptr_t) shm == -1) {
        qCritical() << "Can't get mem, errno: " << errno;
        exit(-1);
    }
    while(true) {
        sembuf op = { .sem_num = 0, .sem_op = 0, .sem_flg = 0 };
        if(semop(memSm, &op, 1) == -1) {
            if(errno == EINVAL || errno == EIDRM) {
                break;
            }
            continue;
        }
        op.sem_op = 1;
        semop(memSm, &op, 1);
        QString ex;
        for(int i = 0; i < shmBufSize; i++) {
            if(shm[i].flg == 0) {
                ex = QString::fromUtf8(shm[i].data);
                shm[i].res = calculate(ex.toStdString());
                shm[i].flg = 1;
                qDebug() << "Compute in mem seg" << i;
            }
        }
        op.sem_op = -1;
        semop(memSm, &op, 1);
    }

    shmdt(shm);
    return 0;
}
