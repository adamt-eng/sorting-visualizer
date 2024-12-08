#include"vector.h"
template<typename T>
vector<T>::vector() : size(0),capacity(0)
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
vector<T>::iterator vector<T>::begin()
{
    return arr;
}

template<typename T>
vector<T>::iterator vector<T>::end()
{
    return arr+size;
}

template<typename T>
int vector<T>::capacity()
{
    return capacity;
}

template<typename T>
int vector<T>::size()
{
    return size;
}

template<typename T>
void vector<T>::reserve(int newSize)
{
    int loop = newSize < size?newSize:size;
    T* temp = new T[newSize];
    for(int i = 0; i < loop;i++)
    {
        temp[i] = arr[i];
    }
    delete[] arr;
    arr = temp;
    capacity = newSize;
}
template<typename T>
void vector<T>::push_back(T x)
{
    if(capacity - size <= 1)
    {
        capacity = (int)(1.5*capacity);
        reserve(capacity);
    }
    arr[size] = x;
    size++;

}