#include <cstddef>
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
    void push_back(T x);
    iterator begin();
    iterator end();

};