#include"../Header Files/vector.h"

template<typename T>
vector<T>::vector() : mySize(0), myCapacity(0),arr(new T[1])
{

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
    delete[] arr;
    arr = temp;
    myCapacity = newSize;
}

template<typename T>
void vector<T>::push_back(T x)
{
    if(myCapacity - mySize <= 1)
    {
        myCapacity = (int)(1.5*myCapacity);
        reserve(myCapacity);
    }
    arr[mySize] = x;
    mySize++;
}
