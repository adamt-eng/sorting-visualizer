#include "../../Header Files/Data Structures/queue.h"
#include <iostream>

template<typename QueueElement>
Queue<QueueElement>::Queue(){
    myFront = nullptr;
    myBack = nullptr;
}
template<typename QueueElement>
Queue<QueueElement>::Queue(const Queue & original)
{
    myFront = myBack = 0;
    if (!original.empty())
    {
        myFront = myBack = new Queue::Node(original.front());

        Queue::Node* origPtr = original.myFront->next;
        while (origPtr != 0)
        {
            myBack->next = new Queue::Node(origPtr->data);
            myBack = myBack->next;
            origPtr = origPtr->next;
        }
    }
}
template<typename QueueElement>
Queue<QueueElement>::~Queue<QueueElement>(){
    Node* Current = myFront;
    while(Current!=nullptr){
        Node* next = Current -> next;
        delete Current;
        Current = next;
    }
    myFront = myBack = nullptr;
}

template<typename QueueElement>
bool Queue<QueueElement>::empty(){return myFront == nullptr;}

template<typename QueueElement>
QueueElement Queue<QueueElement>::front(){
    if(!empty()){
        return myFront -> data;
    }
    else{
        std::cerr << "Queue is empty!";
        return -1;
    }
}

template<typename QueueElement>
void Queue<QueueElement>::push(QueueElement x){
    Node* newNode = new Node(x);

    if(empty()){
        myFront = newNode;
        myBack = newNode;
    }
    else{
        myBack->next = newNode;
        myBack = newNode;
    }
}

template<typename QueueElement>
void Queue<QueueElement>::pop(){
    if(empty()){
        return;
    }
    else{
        Node* Temp =myFront;
        myFront = myFront->next;
        if (myFront == nullptr) {
            myBack = nullptr;
        }
        delete Temp;
    }
}
template<typename QueueElement>
//--- Definition of display()
void Queue<QueueElement>::display(std::ostream & out) const
{
    Queue::Node* ptr;
    for (ptr = myFront; ptr != 0; ptr = ptr->next)
        out << ptr->data << "  ";
    out << std::endl;
}

template<typename QueueElement>
std::ostream& operator<<(std::ostream& out, const Queue<QueueElement>& aQueue) {
    aQueue.display(out);
    return out;
}
