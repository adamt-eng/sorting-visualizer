#ifndef QUEUE_H
#define QUEUE_H
template <typename QueueElement>
class Queue
{
private:
    class Node{
    public:
        Node* next;
        QueueElement data;
        Node(QueueElement item):next(0){data = item;}
    };
    Node* myTop;
public:
    typedef Node* Nodeptr;
    Queue();
   ~Queue();
    void push(QueueElement x);
    void pop();
    QueueElement front();
    bool empty();
};

#endif // Queue_H
