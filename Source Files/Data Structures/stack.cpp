#include "../../Header Files/Data Structures/stack.h"
#include <stdexcept>

template<typename T>
Stack<T>::Stack():myTop(0){}

template<typename T>
Stack<T>::Stack(const Stack& original){
    myTop = 0;
    if (!original.empty())
    {
        myTop = new Stack::Node(original.top());
        Stack::Node* lastPtr = myTop;
        Stack::Node* origPtr;
        origPtr = original.myTop->next;

        while (origPtr != 0)
        {
            lastPtr->next = new Stack::Node(origPtr->data);
            lastPtr = lastPtr->next;
            origPtr = origPtr->next;
        }
    }
}

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
        throw std::runtime_error("Stack is empty!");
    }
}

template<typename T>
bool Stack<T>::empty(){return myTop == 0;}

template<typename T>
void Stack<T>::push(T item){
    myTop = new Stack<T>::Node(item, myTop);
}
template<typename T>
void Stack<T>::display(std::ostream & out) const
{
    Stack::Node* ptr;
    for (ptr = myTop; ptr != 0; ptr = ptr->next)
        out << ptr->data << std::endl;
}

template<typename T>
std::ostream & operator<< (std::ostream & out, const Stack<T> & aStack)
{
    aStack.display(out);
    return out;
}
template<typename T>
const Stack<T>& Stack<T>::operator=(const Stack<T>& rightHandSide){
    if (this != &rightHandSide)         // check that not st = st
    {
        this->~Stack();                  // destroy current linked list
        if (rightHandSide.empty())       // empty stack
            myTop = 0;
        else
        {                                // copy rightHandSide's list
            // Copy first node
            myTop = new Stack::Node(rightHandSide.top());

            // Set pointers to run through the stacks' linked lists
            Stack::Node* lastPtr = myTop,
                rhsPtr = rightHandSide.myTop->next;

            while (rhsPtr != 0)
            {
                lastPtr->next = new Stack::Node(rhsPtr->data);
                lastPtr = lastPtr->next;
                rhsPtr = rhsPtr->next;
            }
        }
    }
    return *this;
}
