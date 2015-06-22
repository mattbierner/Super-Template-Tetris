/**
    Some basic helper meta programming functions.
*/
#pragma once

/**
    Metafunction that returns `T`.
*/
template <typename T>
struct identity {
    using type = T;
};

/**
    Call a metafunction with a set of arguments.
*/
template <typename f, typename... args>
using call = typename f::template apply<args...>::type;

/**
    Transform a template into a metafunction.
*/
template <template<typename...> class f>
struct mfunc {
    template <typename... args>
    using apply = identity<f<args...>>;
};

/**
    Partially apply a template metafunction, returning a new metafunction.
*/
template <typename f, typename... bound>
struct partial {
    template <typename... args>
    using apply = identity<call<f, bound..., args...>>;
};