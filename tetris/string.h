#pragma once

#include "boolean.h"
#include "printer.h"

/**
    Encode a string of characters as a type.
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
    User defined literal to create a String from a string literal.
*/
template <typename T, T... chars>
constexpr auto operator""_string() {
    return String<chars...>{};
};

/**
    Get the head of a string.
*/
template <typename>
struct StringCar;

template <char x, char... xs>
struct StringCar<String<x, xs...>> {
    using type = String<x>;
};

/**
    Get the rest of a string
*/
template <typename>
struct StringCdr;

template <char x, char... xs>
struct StringCdr<String<x, xs...>> {
    using type = String<xs...>;
};

/**
    Interface for a type that can be converted to a string.
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
struct StringAdd;

template <typename l, char... rs>
struct StringAdd<l, String<rs...>> {
    using type = typename l::template append<rs...>;
};

template <typename l, typename r>
using string_add = typename StringAdd<to_string<l>, to_string<r>>::type;

/**
    Trim a string to be size `n`.
*/
template <size_t n, typename s>
struct StringTake {
    using type =
        string_add<
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
    Combine a list of elements, seperating neighboring elements using `joiner`.
*/
template <typename joiner, typename...>
struct StringJoin;

template <typename joiner, typename first, typename second, typename... rest>
struct StringJoin<joiner, first, second, rest...> {
    using type =
        string_add<
            first,
            string_add<
                joiner,
                typename StringJoin<joiner, second, rest...>::type>>;
};

template <typename joiner, typename first>
struct StringJoin<joiner, first> {
    using type = to_string<first>;
};

template <typename joiner>
struct StringJoin<joiner> {
    using type = String<>;
};

template <typename joiner, typename... elements>
using string_join = typename StringJoin<joiner, elements...>::type;

/**
    Convert an integer value into a string.
*/
template <long val>
struct IntToString {
    struct Rec {
        using type =
            string_add<
                typename IntToString<val / 10>::type,
                String<'0' + (val % 10)>>;
    };

    using type =
        branch<(val < 10),
            identity<String<'0' + (val % 10)>>,
            Rec>;
};

template <long val>
using int_to_string =
    string_add<
        std::conditional_t<val >= 0, String<>, String<'-'>>,
        typename IntToString<val >= 0 ? val : -val>::type>;

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

static_assert(
    std::is_same<
        String<'-', '1', '3', '3', '0'>,
        int_to_string<-1330>>::value, "");


/*------------------------------------------------------------------------------
 * Printer
 */
template <char... elements>
struct Printer<String<elements...>> {
    static std::ostream& Print(std::ostream& output)
    {
        bool Do[] = { true, (output << elements, true)... };
        (void)Do;
        return output;
    }
};

/*------------------------------------------------------------------------------
    Printer - General specilaization for any type that implements `ToString`
*/
template <typename s>
struct Printer<s> {
    static std::ostream& Print(std::ostream& output) {
        return Printer<to_string<s>>::Print(output);
    }
};
