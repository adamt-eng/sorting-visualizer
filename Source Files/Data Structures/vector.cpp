#include"../../Header Files/Data Structures/vector.h"
#include <cstddef>

template<typename T>
Vector<T>::Vector() : mySize(0), myCapacity(1), arr(new T[1])
{

}
template<typename T>
Vector<T>::Vector(int newCapacity)
{
    arr = new T[newCapacity];
    myCapacity = newCapacity;
}
template<typename T>
Vector<T>::Vector(int newSize, const T& initialValue)
    : mySize(newSize), myCapacity(newSize), arr(new T[newSize])
{
    for (int i = 0; i < newSize; ++i) {
        arr[i] = initialValue;
    }
}

template<typename T>
Vector<T>::~Vector()
{
    delete[] arr;
}
template<typename T>
template<std::size_t N>
Vector<T>::Vector(const T (&arr)[N]) {
    arr = new T[N];
    mySize = N;
    myCapacity = N;
    for (std::size_t i = 0; i < N; ++i) {
        push_back(arr[i]);
    }
}
template<typename T>
template<std::size_t N>
void Vector<T>::operator=(const T (&arr)[N]) {
    clear();
    reserve(N);
    for (std::size_t i = 0; i < N; ++i) {
        push_back(arr[i]);
    }
}
template<typename T>
void Vector<T>::clear()
{
    mySize = 0;
    delete[] arr;
    myCapacity = 1;
    arr = new T[1];
}
template<typename T>
bool Vector<T>::isEmpty()
{
    return mySize == 0;
}
template<typename T>
T& Vector<T>::at(int index)
{
    return arr[index];
}

template<typename T>
T& Vector<T>::operator[](int index)
{
    return arr[index];
}

template<typename T>
typename Vector<T>::iterator Vector<T>::begin()
{
    return arr;
}

template<typename T>
typename Vector<T>::iterator Vector<T>::end()
{
    return arr+mySize;
}

template<typename T>
int Vector<T>::capacity()
{
    return myCapacity;
}

template<typename T>
int Vector<T>::size()
{
    return mySize;
}

template<typename T>
void Vector<T>::reserve(int newSize)
{
    int loop = newSize < mySize?newSize:mySize;
    T* temp = new T[newSize];
    for(int i = 0; i < loop;i++)
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
    int loop = newSize < mySize?newSize:mySize;
    T* temp = new T[newSize];
    for(int i = 0; i < loop;i++)
    {
        temp[i] = arr[i];
    }

    delete[] arr;
    arr = temp;
    mySize = newSize;
    myCapacity = newSize;
}
template<typename T>
void Vector<T>::push_back(T x)
{
    if(myCapacity - mySize <= 1)
    {
        myCapacity = isEmpty()?1:2*myCapacity;
        reserve(myCapacity);
    }
    arr[mySize] = x;
    mySize++;
}
template<typename T>
void Vector<T>::push_back(T x, int pos)
{
    if(myCapacity - mySize <= 1)
    {
        myCapacity = isEmpty()?1:2*myCapacity;
        reserve(myCapacity);
    }

    for (int i = mySize; i > pos; --i) {
        arr[i] = arr[i - 1];
    }
    arr[pos] = x;
    mySize++;
}
