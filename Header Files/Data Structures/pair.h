#ifndef PAIR_H
#define PAIR_H

template <typename T, typename U>
class Pair
{
public:
    T first;
    U second;
    Pair(const T& f = T(), const U& s = U());
    Pair(const Pair& other);

    Pair& operator=(const Pair& other);

    Pair<T, U> make_pair(const T& first, const U& second);

    bool operator<(const Pair& other) const;
    bool operator>(const Pair& other) const;
    bool operator<=(const Pair& other) const;
    bool operator>=(const Pair& other) const;
    bool operator==(const Pair& other) const;
    bool operator!=(const Pair& other) const;
};

#endif // PAIR_H
