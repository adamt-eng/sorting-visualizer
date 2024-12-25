#ifndef VECTOR_H
#define VECTOR_H

template<typename T>
class Vector
{
public:
    // Type definitions
    using iterator = T*;
    using const_iterator = const T*;

    // Constructors and Destructor
    Vector();
    Vector(int newCapacity);
    Vector(int newSize, const T& initialValue);
    Vector(const Vector<T>& other); // Copy constructor
    ~Vector();

    // Assignment Operators
    Vector<T>& operator=(const Vector<T>& other); // Copy assignment

    // Capacity and Size
    int size() const;
    int capacity() const;
    bool empty() const;
    void shrink_to_fit();

    // Modifiers
    void clear();
    void push_back(T item);
    void insert(T item, int pos);
    void erase(int pos);

    // Reserve and Resize
    void reserve(int newSize);
    void resize(int newSize);

    // Element Access
    T& at(int index);
    T& operator[](int index);

    // Iterators
    iterator begin();
    iterator end();

private:
    int mySize;       // Current number of elements
    int myCapacity;   // Capacity of the internal array
    T* arr;           // Pointer to the dynamically allocated array
};

#endif // VECTOR_H
