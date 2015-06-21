#pragma once

#include "boolean.h"
#include "utility.h"

/**
    Interface for structures that can be folded.
*/
template <typename f, typename z, typename s>
struct Foldable {
    using type = call<f, z, s>;
};

template <typename f, typename z, typename s>
using fold = typename Foldable<f, z, s>::type;

/**
    Check if a predicate functor `f` holds true for any value in `s`.
*/
template <typename f>
struct AnyReducer {
    template <typename p, typename c>
    using apply = logical_or<p::value, Thunk<call, f, c>>;
};

template <typename f, typename s>
constexpr const bool any = fold<AnyReducer<f>, std::false_type, s>::value;

/**
    Check if a predicate functor `f` holds true for every value in `s`.
*/
template <typename f>
struct EveryReducer {
    template <typename p, typename c>
    using apply = logical_or<p::value, Thunk<call, f, c>>;
};

template <typename f, typename s>
constexpr const bool every = fold<EveryReducer<f>, std::true_type, s>::value;
