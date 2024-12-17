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
        Node(QueueElement item) : data(item), next(0) {}
    };
    Node* myFront;
    Node* myBack;
public:
    Queue();
   ~Queue();
    void push(QueueElement x);
    void pop();
    QueueElement front();
    bool empty();
};

#endif // Queue_H
