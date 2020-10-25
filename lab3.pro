QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += main1.cpp \

DISTFILES += \
#        main1.cpp \
    main2.cpp \
    main3.cpp \
    main4.cpp \
    solver/solver.cpp \
    solver/queue.h \
    solver/solver.h \
    solver/stack.h \
    solver/list.h


TARGET = pr1

#main1.target = main1
#main1.depends = $$PWD/main1.cpp
#main1.commands = $(CXX) $$PWD/main1.cpp -o pr1 -g -std=gnu++11 -Wall -W -fPIC -DQT_CORE_LIB -I../../lab3 -I. -isystem /usr/include/x86_64-linux-gnu/qt5 -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -lQt5Core -lpthread

main2.target = main2
main2.depends = $$PWD/main2.cpp
main2.commands = $$PWD/build_main2.sh
main2.commands = $(CXX) $$PWD/main2.cpp -o pr2 -g -std=gnu++11 -Wall -W -fPIC -DQT_CORE_LIB -I../../lab3 -I. -isystem /usr/include/x86_64-linux-gnu/qt5 -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -lQt5Core -lpthread


main3.target = main3
main3.depends = $$PWD/main3.cpp
main3.commands = $$PWD/build_main3.sh

main4.target = main4
main4.depends = $$PWD/main4.cpp
main4.commands = $(CXX) $$PWD/main4.cpp -o pr4 -g -std=gnu++11 -Wall -W -fPIC -DQT_CORE_LIB -I../../lab3 -I. -isystem /usr/include/x86_64-linux-gnu/qt5 -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -lQt5Core -lpthread

#QMAKE_EXTRA_TARGETS += main1
#PRE_TARGETDEPS += main1
QMAKE_EXTRA_TARGETS += main2 main3 main4
PRE_TARGETDEPS += main2 main3 main4
