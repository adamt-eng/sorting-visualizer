#include "../../Header Files/Data Structures/PriorityQueue.h"
#include <iostream>

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
PriorityQueueElement PriorityQueue<PriorityQueueElement>::top(){
    if(!empty()){
        return myFront -> data;
    }
    else{
        std::cerr << "Priority Queue is empty!";
        return -1;
    }
}

template<typename PriorityQueueElement>
void PriorityQueue<PriorityQueueElement>::push(PriorityQueueElement vertex, int priority){
    Node* newNode = new Node(vertex,priority);

    Node* current = myFront;
    Node* prev = nullptr;

    while (current != nullptr && current->priority <= priority) {
        prev = current;
        current = current->next;
    }

    // If we are inserting before the first element
    if (prev == nullptr) {
        newNode->next = myFront;
        myFront = newNode;
    } else {
        prev->next = newNode;
        newNode->next = current;
    }

    if(newNode->next == nullptr){
        myBack = newNode;
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


