#include "../../Header Files/Data Structures/list.h"

using namespace std;

template <typename T>
List<T>::List() : first(nullptr), mySize(0) {}

template<typename T>
T& List<T>::operator[](int index)
{
    if (index < 0 || index >= mySize)
    {
        throw std::runtime_error("Index out of bounds: " + std::to_string(index));
    }

    List<T>::NodePointer ptr = first;

    for (int i = 0; i < index; ++i)
    {
        ptr = ptr->next;
    }

    return ptr->data;
}


template <typename T>
List<T>::List(const List<T> & originalList) : first(nullptr), mySize(originalList.mySize)
{
    if (mySize == 0) return;

    List<T>::NodePointer originalPtr, currentPtr;

    first = new Node(originalList.first->data);
    currentPtr = first;

    originalPtr = originalList.first->next;
    while (originalPtr != nullptr)
    {
        currentPtr->next = new Node(originalPtr->data);
        originalPtr = originalPtr->next;
        currentPtr = currentPtr->next;
    }
}

template <typename T>
const List<T> & List<T>::operator=(const List<T> & rightSide)
{
    mySize = rightSide.mySize;

    if (mySize == 0)
    {
        this->~List();
        first = nullptr;
        return *this;
    }

    if (this != &rightSide)
    {
        this->~List();

        List<T>::NodePointer originalPtr, lastPtr;

        first = new Node(rightSide.first->data);
        lastPtr = first;

        originalPtr = rightSide.first->next;
        while (originalPtr != nullptr)
        {
            lastPtr->next = new Node(originalPtr->data);
            originalPtr = originalPtr->next;
            lastPtr = lastPtr->next;
        }
    }

    return *this;
}

template <typename T>
inline List<T>::~List()
{
    List<T>::NodePointer prev = first, ptr;

    while (prev != nullptr)
    {
        ptr = prev->next;
        delete prev;
        prev = ptr;
    }

    first = nullptr;
    mySize = 0;
}

template <typename T>
bool List<T>::empty()
{
    return mySize == 0;
}

template <typename T>
void List<T>::insert(T item, int index)
{
    if (index < 0 || index > mySize)
    {
        throw std::runtime_error("Can't insert at index '" + std::to_string(index) + "'");
    }

    mySize++;

    List<T>::NodePointer newPtr = new Node(item), predPtr = first;

    if (index == 0)
    {
        newPtr->next = first;
        first = newPtr;
    }
    else
    {
        for (int i = 1; i < index; i++)
        {
            predPtr = predPtr->next;
        }

        newPtr->next = predPtr->next;
        predPtr->next = newPtr;
    }
}

template <typename T>
void List<T>::erase(int index)
{
    if (index < 0 || index >= mySize)
    {
        throw std::runtime_error("Can't delete at index '" + std::to_string(index) + "'");
    }

    mySize--;

    List<T>::NodePointer ptr, predPtr = first;

    if (index == 0)
    {
        ptr = first;
        first = ptr->next;
        delete ptr;
    }
    else
    {
        for (int i = 1; i < index; i++)
        {
            predPtr = predPtr->next;
        }

        ptr = predPtr->next;
        predPtr->next = ptr->next;
        delete ptr;
    }
}

template <typename T>
T List<T>::search(T item)
{
    int index;

    List<T>::NodePointer temp = first;

    for (index = 0; index < mySize; index++)
    {
        if (temp->data == item)
        {
            return index;
        }
        else
        {
            temp = temp->next;
        }
    }

    return -1; // Return -1 if not found
}

template <typename T>
int List<T>::size()
{
    return mySize;
}
