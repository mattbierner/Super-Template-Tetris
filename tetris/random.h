#pragma once

#include <limits>
#include "string.h"

/**
    Random number generator.
*/
template <typename T, T s, T max = std::numeric_limits<T>::max(), T a = 1103515245, T c = 12345>
struct LinearGenerator {
    static constexpr const T value = ((long)s * a + c) % max;
    using next = LinearGenerator<T, value, max, a, c>;
};

/**
    Random number generator in [0, max).
*/
template <unsigned max, typename rand = LinearGenerator<unsigned, 0>>
struct Random {
    static constexpr const unsigned value = rand::value % max;
    using next = Random<max, typename rand::next>;
};

/*------------------------------------------------------------------------------
    Serialize
*/
template <typename T, T s, T max, T a, T c>
struct Serialize<LinearGenerator<T, s, max, a, c>> {
    using type =
        serialize_class<decltype("LinearGenerator"_string),
            T,
            SerializableValue<T, s>,
            SerializableValue<T, max>,
            SerializableValue<T, a>,
            SerializableValue<T, c>>;
};

template <unsigned max, typename rand>
struct Serialize<Random<max, rand>> {
    using type =
        serialize_class<decltype("Random"_string),
            SerializableValue<unsigned, max>,
            rand>;
};
