#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>

template<typename T>
class Vector
{
private:
    T* arr;
    int mySize;
    int myCapacity;
public:
    typedef T* iterator;
    Vector();
    Vector(int newCapacity);
    template<size_t N>
    Vector(const T (&arr)[N]);
    Vector(int newSize, const T& initialValue);
    ~Vector();
    void clear();
    bool isEmpty();
    template<size_t N>
    void operator=(const T (&arr)[N]);
    T& at(int index);
    T& operator[](int index);
    int size();
    int capacity();
    void reserve(int newSize);
    void resize(int newSize);
    void push_back(T x);
    void push_back(T x, int pos);
    iterator begin();
    iterator end();

};

#endif
