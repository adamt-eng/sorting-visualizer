#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
template<typename PriorityQueueElement>
class PriorityQueue
{
private:
    class Node{
    public:
        Node* next;
        PriorityQueueElement data;
        int priority;
         Node(PriorityQueueElement item, int p) : data(item), priority(p), next(nullptr) {}
    };
    Node* myFront;
    Node* myBack;
public:
    typedef Node* Nodeptr;
    PriorityQueue();
    ~PriorityQueue();
    void push(PriorityQueueElement vertex, int priority);
    void pop();
    PriorityQueueElement top();
    bool empty();
};

#endif // PRIORITYQUEUE_H
