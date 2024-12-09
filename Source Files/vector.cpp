#include"../Header Files/vector.h"
#include <cstddef> 
template<typename T>
vector<T>::vector() : mySize(0), myCapacity(1),arr(new T[1])
{

}
template<typename T>
vector<T>::vector(int newCapacity)
{
    arr = new T[newCapacity];
    myCapacity = newCapacity;
}
template<typename T>
vector<T>::vector(int newSize, const T& initialValue)
    : mySize(newSize), myCapacity(newSize), arr(new T[newSize])
{
    for (int i = 0; i < newSize; ++i) {
        arr[i] = initialValue;  // Initialize all elements with the specified value
    }
}
template<typename T>
vector<T>::~vector()
{
    delete[] arr;
}
template<typename T>
template<std::size_t N>
vector<T>::vector(const T (&arr)[N]) {
    arr = new T[N];
    mySize = N;
    myCapacity = N;
    for (std::size_t i = 0; i < N; ++i) {
        push_back(arr[i]);
    }
}
template<typename T>
template<std::size_t N>
void vector<T>::operator=(const T (&arr)[N]) {
    clear();           
    reserve(N);      
    for (std::size_t i = 0; i < N; ++i) {
        push_back(arr[i]); 
    }
}
template<typename T>
void vector<T>::clear()
{
    mySize = 0;
    ~vector();
        myCapacity = 1;
        arr = new T[1];
}
template<typename T>
bool vector<T>::isEmpty()
{
    return mySize == 0;
}
template<typename T>
T& vector<T>::at(int index)
{
    return arr[index];
}

template<typename T>
T& vector<T>::operator[](int index)
{
    return arr[index];
}

template<typename T>
typename vector<T>::iterator vector<T>::begin()
{
    return arr;
}

template<typename T>
typename vector<T>::iterator vector<T>::end()
{
    return arr+mySize;
}

template<typename T>
int vector<T>::capacity()
{
    return myCapacity;
}

template<typename T>
int vector<T>::size()
{
    return mySize;
}

template<typename T>
void vector<T>::reserve(int newSize)
{
    int loop = newSize < mySize?newSize:mySize;
    T* temp = new T[newSize];
    for(int i = 0; i < loop;i++)
    {
        temp[i] = arr[i];
    }
    ~vector();
    arr = temp;
    myCapacity = newSize;
}

template<typename T>
void vector<T>::push_back(T x)
{
    if(myCapacity - mySize <= 1)
    {
        myCapacity = 2*myCapacity;
        reserve(myCapacity);
    }
    arr[mySize] = x;
    mySize++;
}
