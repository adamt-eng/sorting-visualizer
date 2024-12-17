#include "../../Header Files/Data Structures/stack.h"
#include <iostream>

template<typename T>
Stack<T>::Stack():myTop(0){}

template<typename T>
Stack<T>::~Stack() {
    while (!empty()) {
        pop();
    }
}

template<typename T>
void Stack<T>::pop(){
    if(!empty())
    {
        Stack<T>::NodePtr ptr = myTop;
        myTop = myTop->next;
        delete ptr;
    }
}

template<typename T>
T Stack<T>::top(){
    if(!empty()){
        return myTop->data;
    }
    else{
        std::cerr << "Stack is empty!";
        return -1;
    }
}

template<typename T>
bool Stack<T>::empty(){return myTop == 0;}

template<typename T>
void Stack<T>::push(T item){
    myTop = new Stack<T>::Node(item, myTop);
}
