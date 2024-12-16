#include "../../Header Files/Data Structures/stack.h"
template<typename T>
Stack<T>::Stack():myTop(0){}

template<typename T>
void Stack<T>::pop(){
    if(!empty())
    {
    Stack<T>::Nodeptr ptr = myTop;
        myTop = myTop->next;
    delete ptr;
    }
}

template<typename T>
T Stack<T>::top(){
    if(!empty())
        return myTop->data;
}

template<typename T>
bool Stack<T>::empty(){return myTop == 0;}

template<typename T>
void Stack<T>::push(T item){
    Stack<T>::Nodeptr ptr = myTop;
    myTop = new Stack<T>::Node(item);
    myTop->next = ptr;
}
