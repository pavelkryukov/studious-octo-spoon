#ifndef TRIGONOMETRY_H
#define TRIGONOMETRY_H

#include "factorial.h"
#include "power.h"

#define SIGN(A) ((A % 2) ? 1 : -1)

#if defined(_WIN32) || defined(_WIN64)
    typedef unsigned __int64 uint64;
#else
    typedef unsigned long long int uint64;
#endif

// Power definition
template<unsigned N, typename T>
struct Power {
    static inline const T value(T X) {
        return (Power<N - 1, T>::value(X) * X);
    }
};

template<typename T>
struct Power<0, T> {
    static inline const T value(T X) {
        return (T)1;
    }
};

// Factorial definition
template<unsigned N>
struct Factorial {
    static const uint64 value = Factorial<N-1>::value * N;
};

template<>
struct Factorial<0> {
    static const uint64 value = 1;
};

// Item definitions
template<unsigned N, typename T>
struct Item {
    static inline const T value(T X) {
        return ((T)Power<N, T>::value(X) / Factorial<N>::value);
    }
};

// Sinus
template<unsigned N, typename T>
struct Sinus {
    static inline const T value(T X) {
        return Sinus<N - 1, T>::value(X) + Item<2 * N - 1, T>::value(X) * SIGN(N);
    }
};

template<typename T>
struct Sinus<0, T> {
    static inline const T value(T X) {
        return (T)0;
    }
};

// Cosinus
template<unsigned N, typename T>
struct Cosinus {
    static inline const T value(T X) {
        return Cosinus<N - 1, T>::value(X) + Item<2 * N - 2, T>::value(X) * SIGN(N);
    }
};

template<typename T>
struct Cosinus<0, T> {
    static inline const T value(T X) {
        return (T)0;
    }
};

// Exponent
template<unsigned N, typename T>
struct Exponent {
    static const T value(T X) {
        return Exponent<N - 1, T>::value(X) + Item<N - 1, T>::value(X);
    }
};

template<typename T>
struct Exponent<0, T> {
    static inline  const T value(T X) {
        return (T)0;
    }
};

// Tangens, cotangens etc.
template<unsigned N, typename T>
struct Tangent {
    static inline const T value(T X) {
        return (Sinus<N,T>::value(X) / Cosinus<N,T>::value(X));
    }
};

template<unsigned N, typename T>
struct Cotangent {
    static inline const T value(T X) {
        return (T)1 / Tangent<N, T>::value(X);
    }
};

#endif