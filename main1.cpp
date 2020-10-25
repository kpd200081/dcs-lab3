#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QElapsedTimer>
#include <QRandomGenerator>

#include <errno.h>
#include <fcntl.h>
#include <string>
#include <unistd.h>

static int exL = 3;
static QList<QString> exOps = { "+", "-", "/", "*" };
static bool stop = false;

void stopSigHadnler(int sig, siginfo_t*, void*) {
    if(sig != SIGINT) {
        return;
    }
    qDebug() << "Stop and exit";
    stop = true;
}

int main(int argc, char* argv[]) {
    struct sigaction sa;
    sa.sa_sigaction = &stopSigHadnler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT);
    sigaction(SIGINT, &sa, nullptr);

    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOptions({
        { { "n", "ex-delay" }, "Time in seconds between expression generations", "secs" },
    });
    parser.process(a);
    if(!parser.isSet("n")) {
        qCritical() << "Not enought arguments";
        exit(-1);
    }
    int tDelay = parser.value("n").toInt();

    key_t msgKey = ftok("../lab3.pro", 1);
    int msgs = msgget(msgKey, IPC_CREAT | IPC_EXCL | 0660);
    while(!stop) {
        sleep(tDelay);
        QString expr;
        for(int i = 0; i < exL; i++) {
            expr += QString::number(QRandomGenerator::system()->bounded(1, 100));
            expr += exOps.at(QRandomGenerator::system()->bounded(0, exOps.size()));
        }
        expr += QString::number(QRandomGenerator::system()->bounded(1, 100));
        QByteArray data = expr.toUtf8();
        msgsnd(msgs, data.data(), data.size(), 0);
        qDebug() << "send: " << expr;
    }
    msgctl(msgs, IPC_RMID, nullptr);

    return 0;
}
