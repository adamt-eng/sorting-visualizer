#ifndef LIST_H
#define LIST_H

template <typename T>
class List
{
private:
    class Node {
    public:
        T data;
        Node *next;
        Node() : next(nullptr) {}
        Node(T item) : data(item), next(nullptr) {}
    };

    typedef Node* NodePointer;

    NodePointer first;
    int mySize;

public:
    // Constructors and Destructor
    List();
    List(const List<T> &originalList);
    ~List();

    // Operator Overloads
    const List<T> &operator=(const List<T> &rightSide);
    T& operator[](int index);

    // Member functions
    void insert(T item, int index);
    void insert(T item);
    void eraseAt(int index);
    void erase(T item);

    bool empty() const;
    int size() const;
    int find(T item) const;
};

#endif // LIST_H
