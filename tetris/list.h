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
    static constexpr const size_t size = sizeof...(elements);
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
    Lookup the value at index `N` in a list.
*/
template <size_t N, typename list>
struct Get;

template <size_t N, typename list>
using get = typename Get<N, list>::type;

template <typename x, typename... xs>
struct Get<0, List<x, xs...>> {
    using type = x;
};

template <size_t N, typename x, typename... xs>
struct Get<N, List<x, xs...>> {
    using type = get<N - 1, List<xs...>>;
};

/**
   Set the value at index `N` in a list.
*/
template <size_t N, typename newValue, typename list>
struct Put;

template <size_t N, typename newValue, typename list>
using put = typename Put<N, newValue, list>::type;

template <typename newValue, typename x, typename... xs>
struct Put<0, newValue, List<x, xs...>> {
    using type = List<newValue, xs...>;
};

template <size_t N, typename newValue, typename x, typename... xs>
struct Put<N, newValue, List<x, xs...>> {
    using type = cons<x, put<N - 1, newValue, List<xs...>>>;
};

/**
   Remove the value at index `N` in a list.
*/
template <size_t N, typename list>
struct SliceOut;

template <size_t N, typename list>
using slice_out = typename SliceOut<N, list>::type;

template <typename x, typename... xs>
struct SliceOut<0, List<x, xs...>> {
    using type = List<xs...>;
};

template <size_t N, typename x, typename... xs>
struct SliceOut<N, List<x, xs...>> {
    using type = cons<x, slice_out<N - 1, List<xs...>>>;
};

/**
    Build a list of `element` repeated `N` times.
*/
template <size_t N, typename element>
struct Gen {
    using type = cons<element, typename Gen<N - 1, element>::type>;
};

template <typename element>
struct Gen<0, element> {
    using type = List<>;
};

template <size_t N, typename element>
using gen = typename Gen<N, element>::type;

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
template <typename f, typename... elements>
struct Fmap<f, List<elements...>> {
    using type = List<call<f, elements>...>;
};

/*------------------------------------------------------------------------------
    ToString
*/
template <typename... elements>
struct ToString<List<elements...>> {
    using type = string_join<String<>, elements...>;
};

/*------------------------------------------------------------------------------
    Serialize
*/
template <typename... elements>
struct Serialize<List<elements...>> {
    using type =
        serialize_class<decltype("List"_string),
            elements...>;
};
