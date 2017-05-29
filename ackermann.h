template<int M, int N>
struct Ackermann {
    static const int value = Ackermann<M - 1, Ackermann<M, N - 1>::value >::value;
};

// If N = 0

template<int M>
struct Ackermann<M, 0> {
    static const int value = Ackermann<M - 1, 1>::value;
};

// If M = 0

template<int N>
struct Ackermann<0, N> {
    static const int value = N + 1;
};

// Some simplifiers

template<int N>
struct Ackermann<1, N> {
    static const int value = N + 2;
};

template<int N>
struct Ackermann<2, N> {
    static const int value = (N << 1) + 3;
};

// More cheat simplifier
/*
template<int N>
struct Ackermann<3, N> {
    static const int value = (1 << (N + 3)) - 3;
};*/