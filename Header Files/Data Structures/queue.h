#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
template <typename QueueElement>
class Queue
{
private:
    class Node{
    public:
        Node* next;
        QueueElement data;
        Node(QueueElement item) : data(item), next(0) {}
    };
    Node* myFront;
    Node* myBack;
public:
    Queue();
    Queue(const Queue & original);
   ~Queue();
    void push(QueueElement x);
    void pop();
    QueueElement front();
    bool empty();
    void display(std::ostream & out) const;
    friend std::ostream & operator<< (std::ostream & out, const Queue & aList);
};

#endif // Queue_H
