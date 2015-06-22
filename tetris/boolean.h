#pragma once

template <template<typename...> class T, typename... args>  
struct Thunk {  
    using type = typename T<args...>::type;
};

/**
    Call by need logical and operation.
*/
template <bool, typename right>  
struct logical_and : right::type { };

template <typename right>  
struct logical_and<false, right> : std::false_type { };

/**
    Call by need logical or operation.
*/
template <bool, typename right>  
struct logical_or : right::type { };

template <typename right>  
struct logical_or<true, right> : std::true_type { };

/**
    Branch on `value` using call by need evaluation.
*/
template <bool value, typename consequent, typename alternate>
struct Branch {
    using type = typename consequent::type;
};

template <typename consequent, typename alternate>
struct Branch<false, consequent, alternate> {
    using type = typename alternate::type;
};

template <bool value, typename consequent, typename alternate>
using branch = typename Branch<value, consequent, alternate>::type;
