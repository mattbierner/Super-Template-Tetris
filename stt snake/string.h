/**
    Encode a string of characters as a type.
*/
#pragma once

#include "boolean.h"
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
    Interface for a type that can be converted to a string
 */
template <typename>
struct ToString;

template <char... chars>
struct ToString<String<chars...>> {
    using type = String<chars...>;
};

template <typename s>
using to_string = typename ToString<s>::type;

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

/**
    Convert an integer value into a string.
*/
template <size_t val>
struct IntToString {
    struct Rec {
        using type =
            string_join<
                typename IntToString<val / 10>::type,
                String<'0' + (val % 10)>>;
    };

    using type =
        branch_t<(val < 10),
            identity<String<'0' + (val % 10)>>,
            Rec>;
};

template <size_t val>
using int_to_string = typename IntToString<val>::type;

static_assert(
    std::is_same<
        String<'0'>,
        int_to_string<0>>::value, "");

static_assert(
    std::is_same<
        String<'3'>,
        int_to_string<3>>::value, "");

static_assert(
    std::is_same<
        String<'1', '3'>,
        int_to_string<13>>::value, "");

static_assert(
    std::is_same<
        String<'1', '3', '3', '0'>,
        int_to_string<1330>>::value, "");

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