#include "../Header Files/pair.h"

template <typename T, typename U>
Pair<T,U>::Pair(const T& f, const U& s) : first(f), second(s) {}

template <typename T, typename U>
void Pair<T,U>::setFirst(const T& f)
{
    first = f;
}

template <typename T, typename U>
void Pair<T,U>::setSecond(const U& s)
{
    second = s;
}

template <typename T, typename U>
bool Pair<T,U>::operator<(const Pair& other) const
{
    return (first < other.first) || ((first == other.first) && (second < other.second));
}

template <typename T, typename U>
bool Pair<T,U>::operator>(const Pair& other) const
{
    return other < *this;
}
