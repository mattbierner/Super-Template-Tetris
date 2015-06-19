/**
    Encode a string of characters as a type.
*/
#pragma once

#include "printer.h"

/**
    Character input.
*/
template <char... chars>
struct String {

    template <char... other>
    using append = String<chars..., other...>;
};

/**
*/
template <typename l, typename r>
struct StringJoin;

template <typename l, char... rs>
struct StringJoin<l, String<rs...>> {
    using type = typename l::template append<rs...>;
};

template <typename l, typename r>
using string_join = typename StringJoin<l, r>::type;

/**
*/
template <typename T, T... chars>
constexpr auto operator""_string()
{
    return String<chars...>{};
}

/*------------------------------------------------------------------------------
 * Printer
 */
template <>
struct Printer<String<>>
{
    static std::ostream& Print(std::ostream& output) { return output; }
};

template <char x, char... xs>
struct Printer<String<x, xs...>>
{
    static std::ostream& Print(std::ostream& output)
    {
        return Printer<String<xs...>>::Print(output << x);
    }
};