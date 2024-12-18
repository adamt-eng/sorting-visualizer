#ifndef LIST_H
#define LIST_H
#include <cstddef>

namespace gui{

    template <typename ListElement>
    class List
    {
    private:
        ListElement* arr;
        int mySize;
        int myCapacity;
    public:
        typedef ListElement* iterator;
        List();
        List(const List& original);
        List(int newSize, const ListElement& initialValue);
        List(int newCapacity);
        template<size_t N>
        List(const ListElement (&arr)[N]);
        ~List();
        void clear();
        bool empty();
        int size();
        int capacity();
        void erase(int position);
        void insert(ListElement element, int position);
        void insert(ListElement element);
        ListElement& operator [](int index);
        int find(ListElement item) const;
        iterator begin();
        iterator end();
    };
}
#endif // List_H
