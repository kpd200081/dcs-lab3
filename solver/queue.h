#ifndef QUEUE_H
#define QUEUE_H
#include "list.h"

template<class T>
class Queue {
    List<T> list;

public:
    Queue();
    void push(T data);
    T pop();
    T get();
    bool isEmpty() {
        return list.isEmpty();
    }
    size_t size() {
        return list.size();
    }
};

template<class T>
Queue<T>::Queue() {
}

template<class T>
void Queue<T>::push(T data) {
    list.push(data, List<T>::Position::Begin);
}

template<class T>
T Queue<T>::pop() {
    return list.pop(List<T>::Position::End);
}

template<class T>
T Queue<T>::get() {
    T data = list.pop(List<T>::Position::End);
    list.push(data, List<T>::Position::End);
    return data;
}

#endif // QUEUE_H
