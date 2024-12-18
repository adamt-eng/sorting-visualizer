#include "../../Header Files/Data Structures/list.h"

namespace gui{
    template <typename ListElement>
    List<ListElement>::List() : arr(nullptr), mySize(0), myCapacity(0) {}

    template <typename ListElement>
    List<ListElement>::List(const List<ListElement>& original) : arr(new ListElement[original.myCapacity]), mySize(original.mySize), myCapacity(original.myCapacity) {
        for (int i = 0; i < mySize; i++) {
            arr[i] = original.arr[i];
        }
    }
    template<typename ListElement>
    List<ListElement>::List(int newCapacity)
    {
        arr = new ListElement[newCapacity];
        myCapacity = newCapacity;
        mySize = 0;
    }

    template <typename ListElement>
    List<ListElement>::List(int newSize, const ListElement& initialValue): mySize(newSize),myCapacity(newSize){
         arr = new ListElement[myCapacity];
         for (int i = 0; i < mySize; i++) {
            arr[i] = initialValue;
        }
    }

    template <typename ListElement>
    template<std::size_t N>
    List<ListElement> :: List(const ListElement (&array)[N]){
        arr = new ListElement[N];
        mySize = 0;
        myCapacity = N;
        for (std::size_t i = 0; i < N; ++i) {
            insert(array[i]);
        }
    }

    template <typename ListElement>
    List<ListElement>::~List(){
        delete[] arr;
    }

    template <typename ListElement>
    void List<ListElement>::clear(){
        delete [] arr;
        arr = nullptr;
        mySize = 0;
        myCapacity = 0;
    }

    template <typename ListElement>
    bool List<ListElement> ::empty(){return mySize == 0;}

    template <typename ListElement>
    int List<ListElement> ::size(){return mySize;}

    template <typename ListElement>
    int List<ListElement> ::capacity(){return myCapacity;}

    template <typename ListElement>
    void List<ListElement>::erase(ListElement x) {
        int i = 0;
        while (i < mySize && arr[i] != x) {
            i++;
        }
        if (i < mySize) {
            for (int j = i; j < mySize - 1; j++) {
                arr[j] = arr[j + 1];
            }
            mySize--;
        }
    }

    template <typename ListElement>
    void List<ListElement>::insert(ListElement element, int position) {
        if (position < 0 || position > mySize) {return;}

        if (mySize >= myCapacity) {return;}

        for (int i = mySize; i > position; i--) {
            arr[i] = arr[i - 1];
        }

        arr[position] = element;

        // Increment the size
        mySize++;
    }

    template <typename ListElement>
    void List<ListElement>::insert(ListElement element){
        insert(element,mySize);
    }

    template <typename ListElement>
    ListElement& List<ListElement> ::operator[](int index){
        return arr[index];
    }
    template <typename ListElement>
    int List<ListElement> ::find(ListElement item) const{
        for (int i = 0; i < mySize; i++) {
            if (arr[i] == item) {
                return i;
            }
        }
        return-1;
    }
    template<typename ListElement>
    typename List<ListElement>::iterator List<ListElement>::begin()
    {
        return arr;
    }

    template<typename ListElement>
    typename List<ListElement>::iterator List<ListElement>::end()
    {
        return arr + mySize;
    }

}
