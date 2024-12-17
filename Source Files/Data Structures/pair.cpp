#include "../../Header Files/Data Structures/pair.h"

template <typename T, typename U>
Pair<T,U>::Pair(const T& f, const U& s) : first(f), second(s) {}

template <typename T, typename U>
Pair<T, U>::Pair(const Pair& other) : first(other.first), second(other.second) {}

template <typename T, typename U>
Pair<T, U>& Pair<T, U>::operator=(const Pair& other)
{
    if (this != &other)
    {
        first = other.first;
        second = other.second;
    }

    return *this;
}

template <typename T, typename U>
Pair<T, U> make_pair(const T& first, const U& second)
{
    return Pair<T, U>(first, second);
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

template <typename T, typename U>
bool Pair<T, U>::operator<=(const Pair& other) const
{
    return !(*this > other);
}

template <typename T, typename U>
bool Pair<T, U>::operator>=(const Pair& other) const
{
    return !(*this < other);
}

template <typename T, typename U>
bool Pair<T, U>::operator==(const Pair& other) const
{
    return first == other.first && second == other.second;
}

template <typename T, typename U>
bool Pair<T, U>::operator!=(const Pair& other) const
{
    return !(*this == other);
}
