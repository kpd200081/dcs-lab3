set -x

g++ -c ../main3.cpp -o main3.o -g -std=gnu++11 -Wall -W -fPIC -DQT_CORE_LIB -I../../lab3 -I. -isystem /usr/include/x86_64-linux-gnu/qt5 -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++

g++ -c ../solver/solver.cpp -o solver.o -g -g -std=gnu++11 -Wall -W -fPIC

g++ main3.o solver.o -o pr3 -lQt5Core -lpthread 
