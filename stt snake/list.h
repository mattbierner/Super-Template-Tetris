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
struct Car;

template <typename x, typename... xs>
struct Car<List<x, xs...>> {
    using type = x;
};

template <typename list>
using car = typename Car<list>::type;


/**
    Get the rest of a list, excluding the head.
*/
template <typename list>
struct Cdr;

template <typename x, typename... xs>
struct Cdr<List<x, xs...>> {
    using type = List<xs...>;
};

template <typename list>
using cdr = typename Cdr<list>::type;

/**
    Car helpers
*/
template <typename list>
using caar = car<cdr<list>>;

template <typename list>
using caaar = car<cdr<cdr<list>>>;


/**
    Prepend a value onto a list
*/
template <typename x, typename list>
struct Cons;

template <typename x, typename... xs>
struct Cons<x, List<xs...>> {
     using type = List<x, xs...>;
};

template <typename x, typename list>
using cons = typename Cons<x, list>::type;

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
    using type = cons<x, put_t<N - 1, newValue, List<xs...>>>;
};

/**
    Build a list of `element` repeated `N` times.
*/
template <size_t N, typename element>
struct gen {
    using type = cons<element, typename gen<N - 1, element>::type>;
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
    using type = cons<
        call<f, car<s1>, car<s2>>,
        zip<f, cdr<s1>, cdr<s2>>>;
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
    using type = cons<
        call<f, x>,
        f_map<f, List<xs...>>>;
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
struct SerializeToString<List<elements...>> {
    using type =
        serialize_class_to_string<decltype("List"_string),
            elements...>;
};
