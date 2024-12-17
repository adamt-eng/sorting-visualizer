#include "../../Header Files/Data Structures/queue.h"
#include <iostream>

template<typename QueueElement>
Queue<QueueElement>::Queue(){
    myFront = nullptr;
    myBack = nullptr;
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

