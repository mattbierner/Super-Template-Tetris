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
    static constexpr const size_t size = sizeof...(chars);

    /**
        Add additional characters to the end of the string.
    */
    template <char... other>
    using append = String<chars..., other...>;
};

/**
*/
template <typename>
struct StringCar;

template <char x, char... xs>
struct StringCar<String<x, xs...>> {
    using type = String<x>;
};

/**
*/
template <typename>
struct StringCdr;

template <char x, char... xs>
struct StringCdr<String<x, xs...>> {
    using type = String<xs...>;
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
    Combine two strings.
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
template <size_t n, typename s>
struct StringTake {
    using type =
        string_join<
            typename StringCar<s>::type,
            typename StringTake<n - 1, typename StringCdr<s>::type>::type>;
};

template <typename s>
struct StringTake<0, s> {
    using type = String<>;
};

template <size_t n>
struct StringTake<n, String<>> {
    using type = String<>;
};


template <size_t n, typename s>
using string_take = typename StringTake<n, s>::type;


static_assert(
    std::is_same<
        String<>,
        string_take<10, String<>>>::value, "");

static_assert(
    std::is_same<
        String<'a'>,
        string_take<10, String<'a'>>>::value, "");

static_assert(
    std::is_same<
        String<'a', 'b'>,
        string_take<2, String<'a', 'b', 'c', 'd'>>>::value, "");

/**
*/
template <typename T, T... chars>
constexpr auto operator""_string() {
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