#include "../../Header Files/Data Structures/PriorityQueue.h"

template<typename PriorityQueueElement>
PriorityQueue<PriorityQueueElement>::PriorityQueue(){
    myFront = nullptr;
    myBack = nullptr;
}

template<typename PriorityQueueElement>
PriorityQueue<PriorityQueueElement>::~PriorityQueue(){
    Node* Current = myFront;
    while(Current!=nullptr){
        Node* next = Current -> next;
        delete Current;
        Current = next;
    }
    myFront = myBack = nullptr;
}

template<typename PriorityQueueElement>
bool PriorityQueue<PriorityQueueElement>::empty(){return myFront == nullptr;}

template<typename PriorityQueueElement>
PriorityQueueElement PriorityQueue<PriorityQueueElement>::front(){
    if(!empty()){
        return myFront -> data;
    }
}

template<typename PriorityQueueElement>
void PriorityQueue<PriorityQueueElement>::push(PriorityQueueElement vertex, int priority){
    Node* newNode = new Node(vertex,priority);

    if (empty() || myFront->priority > priority) {
        newNode->next = myFront;
        myFront = newNode;

    // If the queue was empty, update myBack as well
    if (myBack == nullptr) {
        myBack = newNode;
    }
    else{
        Node* current = myFront;
        while (current->next != nullptr && current->next->priority <= priority) {
            current = current->next;
        }
        newNode -> next =current->next;
        current->next =newNode;
    }

    if(newNode->next == nullptr){
        myBack = newNode;
    }
 }
}

template<typename PriorityQueueElement>
void PriorityQueue<PriorityQueueElement>::pop(){
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


