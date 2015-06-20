#pragma once

#include <limits>
#include <stdint.h>
#include "string.h"

template <typename T, T s, T max = std::numeric_limits<T>::max(), T a = 1103515245, T c = 12345>
struct LinearGenerator
{
    using type = T;
    static constexpr const T value = ((long)s * a + c) % max;
    using next = LinearGenerator<T, value, max, a, c>;
};


template <unsigned max, typename rand = LinearGenerator<unsigned, 0>>
struct Random
{
    static constexpr const unsigned value = rand::value % max;
    using next = Random<max, typename rand::next>;
};


/*------------------------------------------------------------------------------
    Serialize
*/
template <typename T, T s, T max, T a, T c>
struct SerializeToString<LinearGenerator<T, s, max, a, c>> {
    using type =
        serialize_class_to_string<decltype("LinearGenerator"_string),
            T,
            SerializableValue<T, s>,
            SerializableValue<T, max>,
            SerializableValue<T, a>,
            SerializableValue<T, c>>;
};

template <unsigned max, typename rand>
struct SerializeToString<Random<max, rand>> {
    using type =
        serialize_class_to_string<decltype("Random"_string),
            SerializableValue<unsigned, max>,
            rand>;
};

/*------------------------------------------------------------------------------
    ToString
*/
template <typename T, T s, T max, T a, T c>
struct ToString<LinearGenerator<T, s, max, a, c>> {
    using type = int_to_string<LinearGenerator<T, s, max, a, c>::value>;
};

template <unsigned max, typename rand>
struct ToString<Random<max, rand>> {
    using type = int_to_string<Random<max, rand>::value>;
};