#include "../../Header Files/Data Structures/list.h"
#include <new>
#include <stdexcept>

namespace gui{

template<typename ListElement>
List<ListElement>::List(int newCapacity) : mySize(0)
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

template <typename ListElement>
List<ListElement>::List(const List<ListElement> & original) : mySize(original.mySize), myCapacity(original.myCapacity){
    arr = new(std::nothrow) ListElement[myCapacity];

    if (arr != nullptr)
    {
        for(int i = 0; i < mySize; i++)
            arr[i] = original.arr[i];
    }
    else
    {
        throw std::runtime_error("Inadequate memory to allocate storage for list");
    }
}

template <typename ListElement>
List<ListElement>::List(int newSize, const ListElement& initialValue): mySize(newSize), myCapacity(newSize){
    arr = new (std::nothrow) ListElement[myCapacity];

    if (arr == nullptr)
    {
        throw std::runtime_error("Inadequate memory to allocate storage for list");
    }
    else
    {
        for (int i = 0; i < mySize; i++) {
            arr[i] = initialValue;
        }
    }
}

template <typename ListElement>
template<std::size_t N>
List<ListElement> :: List(const ListElement (&array)[N]){
    arr = new (std::nothrow)  ListElement[N];
    if (arr == nullptr)
    {
        throw std::runtime_error("Inadequate memory to allocate storage for list");
    }
    else
    {
        mySize = N;
        myCapacity = N;
        for (std::size_t i = 0; i < N; ++i) {
            arr[i] = array[i];
        }
    }
}

template <typename ListElement>
List<ListElement>::~List(){
    delete[] arr;
}

template <typename ListElement>
void List<ListElement>::clear(){
    mySize = 0;
}

template <typename ListElement>
bool List<ListElement> ::empty(){return mySize == 0;}

template <typename ListElement>
int List<ListElement> ::size(){return mySize;}

template <typename ListElement>
int List<ListElement> ::capacity(){return myCapacity;}

template <typename ListElement>
void List<ListElement>::erase(int position) {
    if (mySize == 0)
    {
        throw std::runtime_error("List is empty");
    }

    if (position < 0 || position >= mySize)
    {
        throw std::runtime_error("Illegal location to delete");
    }

    for (int i = position; i < mySize - 1; i++)
        arr[i] = arr[i + 1];

    mySize--;
}

template <typename ListElement>
void List<ListElement>::insert(ListElement element, int position) {
    if (mySize == myCapacity)
    {
        throw std::runtime_error("No space for list element");
    }

    if (position < 0 || position > mySize)
    {
        throw std::runtime_error("Illegal location to insert");
    }

    for (int i = mySize; i > position; i--) {
        arr[i] = arr[i - 1];
    }

    arr[position] = element;

    // Increment the size
    mySize++;
}

template <typename ListElement>
void List<ListElement>::insert(ListElement element){
}

template <typename ListElement>
ListElement& List<ListElement> ::operator[](int index){
    if (index < 0 || index >= mySize) {
        throw std::runtime_error("Index out of bounds");
    }
    return arr[index];
}

template <typename ListElement>
int List<ListElement> ::find(ListElement item) const{
    for (int i = 0; i < mySize; i++) {
        if (arr[i] == item) {
            return i;
        }
    }
    return-1;
}

template<typename ListElement>
typename List<ListElement>::iterator List<ListElement>::begin()
{
    return arr;
}

template<typename ListElement>
typename List<ListElement>::iterator List<ListElement>::end()
{
    return arr + mySize;
}

}
