#pragma once

#include "printer.h"

/**
    Character input.
*/
template <char... chars>
using string = std::integer_sequence<char, chars...>;

template <typename T, T... chars>
constexpr auto operator""_string()
{
    return string<chars...>{};
}

/*------------------------------------------------------------------------------
 * Printer
 */
template <>
struct Printer<string<>>
{
    static std::ostream& Print(std::ostream& output) { return output; }
};

template <char x, char... xs>
struct Printer<string<x, xs...>>
{
    static std::ostream& Print(std::ostream& output)
    {
        return Printer<string<xs...>>::Print(output << x);
    }
};