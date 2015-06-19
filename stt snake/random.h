#pragma once

#include <limits>
#include <stdint.h>
#include "printer.h"

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
struct Serialize<LinearGenerator<T, s, max, a, c>>
{
    static std::ostream& Write(std::ostream& output)
    {
        output << "LinearGenerator<";
        Serialize<T>::Write(output);
        return output << "," << s << "," << max << "," << a << "," << c << ">";
    }
};

template <unsigned max, typename rand>
struct Serialize<Random<max, rand>>
{
    static std::ostream& Write(std::ostream& output)
    {
        output << "Random<";
        Serialize<rand>::Write(output);
        return output << "," << "," << max << ">";
    }
};

/*------------------------------------------------------------------------------
    Printer
*/
template <typename T, T s, T max, T a, T c>
struct Printer<LinearGenerator<T, s, max, a, c>>
{
    static void Print(std::ostream& output)
    {
        output << LinearGenerator<T, s, max, a, c>::value;
    }
};

template <unsigned max, typename rand>
struct Printer<Random<max, rand>>
{
    static void Print(std::ostream& output)
    {
        output << Random<max, rand>::value;
    }
};