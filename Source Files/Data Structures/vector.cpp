#include "../../Header Files/Data Structures/vector.h"
#include <stdexcept>

// Constructors and Destructor
template<typename T>
Vector<T>::Vector() : mySize(0), myCapacity(1), arr(new T[1]) {}

template<typename T>
Vector<T>::Vector(int newCapacity)
{
    if (newCapacity >= 0)
    {
        myCapacity = newCapacity;
        mySize = newCapacity;
        arr = new T[newCapacity];
    }
    else
    {
        throw std::out_of_range("Capacity can't be a negative value.");
    }
}

template<typename T>
Vector<T>::Vector(int newSize, const T& initialValue) : mySize(newSize), myCapacity(newSize), arr(new T[newSize])
{
    for (int i = 0; i < newSize; ++i)
    {
        arr[i] = initialValue;
    }
}

template<typename T>
Vector<T>::Vector(const Vector<T>& other) : mySize(other.mySize), myCapacity(other.myCapacity), arr(new T[other.myCapacity])
{
    for (int i = 0; i < other.mySize; ++i)
    {
        arr[i] = other.arr[i];
    }
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
    if (this != &other)
    {
        T* newArr = new T[other.myCapacity];

        for (int i = 0; i < other.mySize; ++i)
        {
            newArr[i] = other.arr[i];
        }

        delete[] arr;
        arr = newArr;
        mySize = other.mySize;
        myCapacity = other.myCapacity;
    }
    return *this;
}

template<typename T>
Vector<T>::~Vector()
{
    delete[] arr;
}

// Capacity and Size
template<typename T>
int Vector<T>::size() const
{
    return mySize;
}

template<typename T>
int Vector<T>::capacity() const
{
    return myCapacity;
}

template<typename T>
bool Vector<T>::empty() const
{
    return mySize == 0;
}

template<typename T>
void Vector<T>::shrink_to_fit()
{
    if (myCapacity > mySize)
    {
        T* temp = new T[mySize];

        for (int i = 0; i < mySize; ++i)
        {
            temp[i] = arr[i];
        }

        delete[] arr;
        arr = temp;
        myCapacity = mySize;
    }
}

// Reserve and Resize
template<typename T>
void Vector<T>::reserve(int newSize)
{
    if (newSize <= myCapacity) return; // No need to allocate if newSize is smaller or equal

    T* temp = new T[newSize];

    for (int i = 0; i < mySize; ++i)
    {
        temp[i] = arr[i];
    }

    delete[] arr;
    arr = temp;
    myCapacity = newSize;
}

template<typename T>
void Vector<T>::resize(int newSize)
{
    if (newSize < mySize)
    {
        mySize = newSize;
    }
    else
    {
        reserve(newSize);

        for (int i = mySize; i < newSize; ++i)
        {
            arr[i] = T(); 
        }

        mySize = newSize;
    }
}

// Element Access
template<typename T>
T& Vector<T>::at(int index)
{
    if (index < 0 || index >= mySize)
    {
        throw std::out_of_range("Index out of bounds");
    }

    return arr[index];
}

template<typename T>
T& Vector<T>::operator[](int index)
{
    if (index < 0 || index >= mySize)
    {
        throw std::out_of_range("Index out of bounds");
    }
    return arr[index];
}

// Modifiers
template<typename T>
void Vector<T>::clear()
{
    delete[] arr;
    arr = new T[1]; // Reset to valid state
    mySize = 0;
    myCapacity = 1;
}

template<typename T>
void Vector<T>::push_back(T item)
{
    if (mySize >= myCapacity)
    {
        reserve(myCapacity == 0 ? 1 : myCapacity * 2);
    }

    arr[mySize] = item;
    ++mySize;
}

template<typename T>
void Vector<T>::insert(T item, int pos)
{
    if (pos < 0)
    {
        throw std::out_of_range("Can't insert at negative index.");
    }
    else if (pos > mySize)
    {
        resize(pos);
    }

    if (mySize >= myCapacity)
    {
        reserve(myCapacity == 0 ? 1 : myCapacity * 2);
    }

    for (int i = mySize; i > pos; --i)
    {
        arr[i] = arr[i - 1];
    }

    arr[pos] = item;
    ++mySize;
}


template<typename T>
void Vector<T>::erase(int pos)
{
    if (pos < 0 || pos >= mySize)
    {
        throw std::out_of_range("Index out of bounds");
    }

    for (int i = pos; i < mySize - 1; ++i)
    {
        arr[i] = arr[i + 1];
    }

    --mySize;
}

// Iterators
template<typename T>
typename Vector<T>::iterator Vector<T>::begin()
{
    return arr;
}

template<typename T>
typename Vector<T>::iterator Vector<T>::end()
{
    return arr + mySize;
}
