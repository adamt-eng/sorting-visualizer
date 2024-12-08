template<typename T>
class vector
{
    private:
    T* arr;
    int size;
    int capacity;
    public:
    typedef T* iterator;
    vector();
    T& at(int index);
    T& operator[](int index);
    int size();
    int capacity();
    void reserve(int newSize);
    void push_back(T x);
    iterator begin();
    iterator end();
    

};