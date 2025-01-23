#include "../../Header Files/Data Structures/list.h"
#include <utility>
#include <stdexcept>
#include <string>
using namespace std;

template <typename T>
List<T>::List() : first(nullptr), mySize(0) {}

template<typename T>
T& List<T>::operator[](int index)
{
    if (index < 0 || index >= mySize)
    {
        throw std::runtime_error("Index out of bounds. Index: " + std::to_string(index) + ", Size: " + std::to_string(mySize));
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
    if (this != &rightSide)
    {
        List<T> temp(rightSide);
        std::swap(first, temp.first);
        std::swap(mySize, temp.mySize);
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
bool List<T>::empty() const
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

    List<T>::NodePointer newPtr = new Node(item);

    if (index == 0)
    {
        newPtr->next = first;
        first = newPtr;
    }
    else
    {
        List<T>::NodePointer predPtr = first;

        for (int i = 1; i < index; ++i)
        {
            predPtr = predPtr->next;
        }

        newPtr->next = predPtr->next;
        predPtr->next = newPtr;
    }

    ++mySize;
}

template <typename T>
void List<T>::insert(T item)
{
    NodePointer newPtr = new Node(item);

    if (empty())
    {
        first = newPtr;
    }
    else
    {
        NodePointer current = first;

        while (current->next != nullptr)
        {
            current = current->next;
        }

        current->next = newPtr;
    }

    ++mySize;
}

template <typename T>
void List<T>::eraseAt(int index)
{
    if (index < 0 || index >= mySize)
    {
        throw std::runtime_error("Can't delete at index '" + std::to_string(index) + "'");
    }

    List<T>::NodePointer ptr;

    if (index == 0)
    {
        ptr = first;
        first = ptr->next;
    }
    else
    {
        List<T>::NodePointer predPtr = first;

        for (int i = 1; i < index; ++i)
        {
            predPtr = predPtr->next;
        }

        ptr = predPtr->next;
        predPtr->next = ptr->next;
    }

    delete ptr;
    ptr = nullptr;
    mySize--;
}

template <typename T>
void List<T>::erase(T item)
{
    int index = find(item);
    if (index != -1)
    {
        eraseAt(index);
    }
}

template <typename T>
int List<T>::find(T item) const
{
    if (!empty())
    {
        int index;

        List<T>::NodePointer temp = first;

        for (index = 0; index < mySize; ++index)
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
    }

    return -1;
}

template <typename T>
int List<T>::size() const
{
    return mySize;
}
