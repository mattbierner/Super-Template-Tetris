/**
    Linear feedback shift register compile time random number generator.
    
    https://gist.github.com/mattbierner/d6d989bf26a7e54e7135
*/
#pragma once

#include <utility>

namespace prandom
{

/**
    Ordered collection of bits.
*/
template <bool... values>
using bitset = std::integer_sequence<bool, values...>;

/**
    Ordered collection of indicies.
*/
template <size_t... values>
using indicies = std::integer_sequence<size_t, values...>;

/**
    Convert a `bitset` to an integer type `T`.
*/
template <typename T, T value, typename set>
struct to_int;

template <typename T,  T value>
struct to_int<T, value, bitset<>> :
    std::integral_constant<T, value> { };

template <typename T, T value, bool x, bool... xs>
struct to_int<T, value, bitset<x, xs...>> :
    std::integral_constant<T,
        to_int<T, static_cast<T>((value << 1) | x), bitset<xs...>>::value> { };

/**
    Get the head of an `integer_sequence`
*/
template <typename list>
struct car;

template <typename T, T x, T... xs>
struct car<std::integer_sequence<T, x, xs...>> : std::integral_constant<T, x> { };

/**
    Get the rest of an `integer_sequence` excluding the head.
*/
template <typename list>
struct cdr;

template <typename T, T x, T... xs>
struct cdr<std::integer_sequence<T, x, xs...>> {
    using type = std::integer_sequence<T, xs...>;
};

template <typename list>
using cdr_t = typename cdr<list>::type;

/**
    Prepend a value onto an `integer_sequence`.
*/
template <typename rest, typename rest::value_type head>
struct cons;

template <typename T, T x, T... xs>
struct cons<std::integer_sequence<T, xs...>, x>
{
     using type = std::integer_sequence<T, x, xs...>;
};

template <typename rest, typename rest::value_type head>
using cons_t = typename cons<rest, head>::type;

/**
    Lookup a value in an `integer_sequence`.
*/
template <size_t N, typename T>
struct get;

template <size_t N, typename T, T x, T... xs>
struct get<N, std::integer_sequence<T, x, xs...>> :
    std::integral_constant<T,
        get<N - 1, std::integer_sequence<T, xs...>>::value> { };

template <typename T, T x, T... xs>
struct get<0, std::integer_sequence<T, x, xs...>> :
    std::integral_constant<T, x> { };

/**
    Clamp an `integer_sequence` to length `N`.
*/
template <size_t N, typename T>
struct take;

template <typename T, T... xs>
struct take<0, std::integer_sequence<T, xs...>> {
    using type = std::integer_sequence<T>;
};

template <size_t N, typename T, T... xs>
struct take<N, std::integer_sequence<T, xs...>> {
    using type = cons_t<
        typename take<N - 1,
            cdr_t<std::integer_sequence<T, xs...>>>::type,
        car<std::integer_sequence<T, xs...>>::value>;
};

template <size_t N, typename T>
using take_t = typename take<N, T>::type;

/**
    Get the most significant bit for the next iteration.
*/
template <typename state, typename taps>
struct get_next;

template <typename T>
struct get_next<T, indicies<>> :
    std::integral_constant<bool, false> { };

template <typename T, size_t tap, size_t... taps>
struct get_next<T, indicies<tap, taps...>> :
    std::integral_constant<bool,
        (get<tap, T>::value) ^ (get_next<T, indicies<taps...>>::value)> { };

/**
    Linear feedback shift register.
*/
template <typename state, typename taps>
struct Lfsr {
    /**
        Iterate once to get the next state.
    */
    using next =
        Lfsr<
            take_t<state::size(),
                cons_t<
                    state,
                    get_next<state, taps>::value>>,
            taps>;
    
    /**
        Convert the current state to an integer.
    */
    template <typename T>
    using value = to_int<T, 0, state>;
};

/**
    Simple wrapper around a lfsr
*/
template <unsigned max, typename lfsr>
struct PseudoRandomGenerator {
    using next = PseudoRandomGenerator<max, typename lfsr::next>;

    static const unsigned value = static_cast<unsigned>((lfsr::template value<unsigned>::value - 1) % max);
};

} // prandom

/*------------------------------------------------------------------------------
    Serialize
*/
template <typename state, typename taps>
struct Serialize<prandom::Lfsr<state, taps>>
{
    static std::ostream& Write(std::ostream& output)
    {
        output << "prandom::Lfsr<";
        Join<',',
            state, taps>::Write(output);
        return output << ">";
    }
};

template <unsigned max, typename lfsr>
struct Serialize<prandom::PseudoRandomGenerator<max, lfsr>>
{
    static std::ostream& Write(std::ostream& output)
    {
        output << "prandom::PseudoRandomGenerator<";
        Join<',',
            SerializableValue<unsigned, max>,
            lfsr>::Write(output);
        return output << ">";
    }
};
