#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>

namespace gui
{
    template<typename T>
    class vector
    {
        private:
        T* arr;
        int mySize;
        int myCapacity;
        public:
        typedef T* iterator;
        vector();
        vector(int newCapacity);
        template<size_t N>
        vector(const T (&arr)[N]);
        vector(int newSize, const T& initialValue);
        ~vector();
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
}
#endif
