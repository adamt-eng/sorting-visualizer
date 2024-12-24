#include "../../Header Files/Data Structures/dlist.h"
#include <new>
#include <stdexcept>

// Constructor with capacity
template<typename ListElement>
DList<ListElement>::DList(int newCapacity) : mySize(0)
{
    if (newCapacity > 0)
    {
        myCapacity = newCapacity;
    }
    else
    {
        throw std::runtime_error("Illegal capacity value");
    }

    arr = new (std::nothrow) ListElement[newCapacity];

    if (arr == nullptr)
    {
        throw std::runtime_error("Inadequate memory to allocate storage for list");
    }
}

// Copy constructor
template <typename ListElement>
DList<ListElement>::DList(const DList<ListElement> & original) : mySize(original.mySize), myCapacity(original.myCapacity)
{
    arr = new(std::nothrow) ListElement[myCapacity];

    if (arr == nullptr)
    {
        throw std::runtime_error("Inadequate memory to allocate storage for list");
    }
    else
    {
        for (int i = 0; i < mySize; ++i)
        {
            arr[i] = original.arr[i];
        }
    }
}

// Destructor
template <typename ListElement>
DList<ListElement>::~DList()
{
    delete[] arr;
}

// Member Functions
template <typename ListElement>
void DList<ListElement>::clear()
{
    delete[] arr;

    arr = new (std::nothrow) ListElement[myCapacity];

    if (arr == nullptr)
    {
        throw std::runtime_error("Inadequate memory to allocate storage for list");
    }

    mySize = 0;
}

template <typename ListElement>
bool DList<ListElement>::empty() const
{
    return mySize == 0;
}

template <typename ListElement>
int DList<ListElement>::size() const
{
    return mySize;
}

template <typename ListElement>
int DList<ListElement>::capacity() const
{
    return myCapacity;
}

template <typename ListElement>
void DList<ListElement>::erase(int position)
{
    if (mySize == 0)
    {
        throw std::runtime_error("List is empty");
    }

    if (position < 0 || position >= mySize)
    {
        throw std::runtime_error("Illegal location to delete");
    }

    for (int i = position; i < mySize - 1; ++i)
    {
        arr[i] = arr[i + 1];
    }

    mySize--;
}

template <typename ListElement>
void DList<ListElement>::insert(ListElement element, int position)
{
    if (mySize == myCapacity)
    {
        throw std::runtime_error("No space for list element");
    }

    if (position < 0 || position > mySize)
    {
        throw std::runtime_error("Illegal location to insert");
    }

    for (int i = mySize; i > position; --i)
    {
        arr[i] = arr[i - 1];
    }

    arr[position] = element;

    // Increment the size
    mySize++;
}

template <typename ListElement>
ListElement& DList<ListElement>::operator[](int index)
{
    if (index < 0 || index >= mySize)
    {
        throw std::runtime_error("Index out of bounds");
    }

    return arr[index];
}

template <typename ListElement>
const DList<ListElement> & DList<ListElement>::operator=(const DList<ListElement> & rightHandSide)
{
    if (this != &rightHandSide)
    {
        if (myCapacity != rightHandSide.myCapacity)
        {
            delete[] arr;
            myCapacity = rightHandSide.myCapacity;
            arr = new (std::nothrow) ListElement[myCapacity];

            if (arr == nullptr)
            {
                throw std::runtime_error("Inadequate memory to allocate storage for list");
            }
        }

        mySize = rightHandSide.mySize;

        for (int i = 0; i < mySize; ++i)
        {
            arr[i] = rightHandSide.arr[i];
        }
    }

    return *this;
}

template <typename ListElement>
int DList<ListElement>::find(ListElement item) const
{
    for (int i = 0; i < mySize; ++i)
    {
        if (arr[i] == item)
        {
            return i;
        }
    }

    return -1; // Return -1 if element not found
}
