#ifndef DLIST_H
#define DLIST_H

template <typename ListElement>
class DList {
public:
    // Type Definitions
    typedef ListElement* iterator;

    // Constructors and Destructor
    DList(const DList& original);
    DList(int newCapacity = 1024);
    ~DList();

    // Member Functions
    void clear();
    bool empty() const;
    int size() const;
    int capacity() const;
    void erase(int position);
    void insert(ListElement element, int position);
    ListElement& operator[](int index);
    int find(ListElement item) const;

    // Assignment Operator
    const DList & operator=(const DList & rightHandSide);

private:
    // Member Variables
    ListElement* arr;
    int mySize;
    int myCapacity;
};

#endif // DLIST_H
