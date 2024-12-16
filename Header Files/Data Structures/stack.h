#ifndef STACK_H
#define STACK_H
template<typename T>
class Stack
{
private:
    class Node{
    public:
        Node* next;
        T data;
        Node(T item):next(0){data = item;}
    };
    Node* myTop;
public:
    typedef Node* Nodeptr;
    Stack();
    void pop();
    T top();
    bool empty();
    void push(T item);


};

#endif
