#ifndef STACK_H
#define STACK_H
#include "list.h"
template<class T>
class Stack {
    List<T> list;

public:
    Stack();
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
Stack<T>::Stack() {
}

template<class T>
void Stack<T>::push(T data) {
    list.push(data, List<T>::Position::End);
}

template<class T>
T Stack<T>::pop() {
    return list.pop(List<T>::Position::End);
}

template<class T>
T Stack<T>::get() {
    T data = list.pop(List<T>::Position::End);
    list.push(data, List<T>::Position::End);
    return data;
}

#endif // STACK_H
