#ifndef DLIST_H
#define DLIST_H
#include <cstddef>

namespace gui{

    template <typename ListElement>
    class DList
    {
    private:
        ListElement* arr;
        int mySize;
        int myCapacity;
    public:
        typedef ListElement* iterator;
        DList();
        DList(const DList& original);
        DList(int newSize, const ListElement& initialValue);
        DList(int newCapacity);
        template<size_t N>
        DList(const ListElement (&arr)[N]);
        ~DList();
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
