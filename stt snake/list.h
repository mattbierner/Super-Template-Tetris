/**
    Basic 1D list.
*/
#pragma once

#include "foldable.h"
#include "functor.h"
#include "printer.h"
#include "serialize.h"

/**
    Ordered list of types.
*/
template <typename... elements>
struct List {
    static const size_t size = sizeof...(elements);
};

/**
    Get the head of an list
*/
template <typename list>
struct car;

template <typename x, typename... xs>
struct car<List<x, xs...>> {
    using type = x;
};

template <typename list>
using car_t = typename car<list>::type;

/**
    Get the rest of a list, excluding the head.
*/
template <typename list>
struct cdr;

template <typename x, typename... xs>
struct cdr<List<x, xs...>> {
    using type = List<xs...>;
};

template <typename list>
using cdr_t = typename cdr<list>::type;

/**
    Prepend a value onto a list
*/
template <typename x, typename list>
struct cons;

template <typename x, typename... xs>
struct cons<x, List<xs...>> {
     using type = List<x, xs...>;
};

template <typename x, typename list>
using cons_t = typename cons<x, list>::type;

/**
    Lookup a value in a list.
*/
template <size_t N, typename list>
struct get;

template <size_t N, typename list>
using get_t = typename get<N, list>::type;

template <typename x, typename... xs>
struct get<0, List<x, xs...>> {
    using type = x;
};

template <size_t N, typename x, typename... xs>
struct get<N, List<x, xs...>> {
    using type = get_t<N - 1, List<xs...>>;
};

/**
   Set the value at index `N` in the list.
*/
template <size_t N, typename newValue, typename list>
struct put;

template <size_t N, typename newValue, typename list>
using put_t = typename put<N, newValue, list>::type;

template <typename newValue, typename x, typename... xs>
struct put<0, newValue, List<x, xs...>> {
    using type = List<newValue, xs...>;
};

template <size_t N, typename newValue, typename x, typename... xs>
struct put<N, newValue, List<x, xs...>> {
    using type = cons_t<x, put_t<N - 1, newValue, List<xs...>>>;
};

/**
    Build a list of `element` repeated `N` times.
*/
template <size_t N, typename element>
struct gen {
    using type = cons_t<element, typename gen<N - 1, element>::type>;
};

template <typename element>
struct gen<0, element> {
    using type = List<>;
};

template <size_t N, typename element>
using gen_t = typename gen<N, element>::type;

/**
    Zip
*/
template <typename f, typename s1, typename s2>
struct zip {
    using type = cons_t<
        call<f, car_t<s1>, car_t<s2>>,
        zip<f, cdr_t<s1>, cdr_t<s2>>>;
};

template <typename f, typename s>
struct zip<f, List<>, s> {
    using type = List<>;
};

template <typename f, typename s>
struct zip<f, s, List<>> {
    using type = List<>;
};

template <typename f, typename s1, typename s2>
using zip_t = typename zip<f, s1, s2>::type;

/*------------------------------------------------------------------------------
    Foldable
*/
template <typename f, typename z>
struct Foldable<f, z, List<>> {
    using type = z;
};

template <typename f, typename z, typename x, typename... xs>
struct Foldable<f, z, List<x, xs...>> {
    using type = fold<f, call<f, z, x>, List<xs...>>;
};

/*------------------------------------------------------------------------------
    Functor
*/
template <typename f>
struct Fmap<f, List<>> {
    using type = List<>;
};

template <typename f, typename x, typename... xs>
struct Fmap<f, List<x, xs...>> {
    using type = cons_t<
        call<f, x>,
        fmap_t<f, List<xs...>>>;
};

/*------------------------------------------------------------------------------
    Printer
*/
template <>
struct Printer<List<>>
{
    static void Print(std::ostream& output) { /* noop */ }
};

template <typename x, typename... xs>
struct Printer<List<x, xs...>>
{
    static void Print(std::ostream& output)
    {
        Printer<x>::Print(output);
        Printer<List<xs...>>::Print(output);
    }
};

/*------------------------------------------------------------------------------
    Serialize
*/
template <typename... elements>
struct Serialize<List<elements...>>
{
    static std::ostream& Write(std::ostream& output)
    {
        output << "List<";
        Join<',', elements...>::Write(output);
        return output << ">";
    }
};
