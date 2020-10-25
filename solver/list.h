#ifndef LIST_H
#define LIST_H
#include <iostream>
template<class T>
class List {
    class Node {
    public:
        T data;
        Node* pNext;
        Node* pPrev;
        Node(T _data, Node* _pPrev, Node* _pNext): data(_data), pNext(_pNext), pPrev(_pPrev) {
        }
    };
    Node* first;
    Node* last;

public:
    enum Position { Begin, End };
    List();
    void push(T data, Position pos);
    T pop(Position pos);
    bool isEmpty() {
        return first == 0;
    }
    size_t size() {
        if(first == 0)
            return 0;
        Node* cur = first;
        size_t i = 1;
        while(cur != last) {
            cur = cur->pNext;
            i++;
        }
        return i;
    }
};

template<class T>
List<T>::List() {
    last = first = 0;
}

template<class T>
void List<T>::push(T data, List::Position pos) {
    if(pos == Position::Begin) {
        if(first == 0) {
            last = first = new Node(data, 0, 0);
        } else {
            first->pPrev = new Node(data, 0, first);
            first = first->pPrev;
        }
    }
    if(pos == Position::End) {
        if(first == 0) {
            last = first = new Node(data, 0, 0);
        } else {
            last->pNext = new Node(data, last, 0);
            last = last->pNext;
        }
    }
}

template<class T>
T List<T>::pop(List::Position pos) {
    T data;
    if(this->isEmpty()) {
        return 0;
    }
    if(pos == Position::Begin) {
        data = first->data;
        if(first == last) {
            delete first;
            first = last = 0;
        } else {
            first = first->pNext;
            delete first->pPrev;
            first->pPrev = 0;
        }
    }
    if(pos == Position::End) {
        data = last->data;
        if(first == last) {
            delete first;
            first = last = 0;
        } else {
            last = last->pPrev;
            delete last->pNext;
            last->pNext = 0;
        }
    }
    return data;
}

#endif // LIST_H
