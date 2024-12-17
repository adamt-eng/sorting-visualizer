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
        Node(T item) : data(item), next(0) {}
        Node(T item, Node* nextNode = 0) : data(item), next(nextNode) {}
    };
    Node* myTop;
public:
    typedef Node* NodePtr;
    Stack();
    ~Stack();
    void pop();
    T top();
    bool empty();
    void push(T item);
};

#endif
