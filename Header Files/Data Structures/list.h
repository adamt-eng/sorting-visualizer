#ifndef LIST_H
#define LIST_H

#include <iostream>
using namespace std;

template <typename T>
class List
{
private:
    class Node {
    public:
        T data;
        Node *next;
        Node() : next(nullptr) {}
        Node(T item) : data(item), next(nullptr) {}
    };

    typedef Node *NodePointer;

public:
    T& operator[](int index);
    List();
    List(const List<T> &originalList);
    ~List();
    const List<T> &operator=(const List<T> &rightSide);
    bool empty();
    void insert(T item, int index);
    void erase(int index);
    T search(T item);
    int size();

private:
    NodePointer first;
    int mySize;
};

#endif // LIST_H
