#pragma once

template <template<typename...> class T, typename... args>  
struct Thunk {  
    using type = T<args...>;
};

/**
    Encodes a boolean as a type so we can perform call by need operations on it.
*/
template <bool, typename right>  
struct logical_and : right::type { };

template <typename right>  
struct logical_and<false, right> : std::false_type { };

/**
    Branch on `value` using call by need evaluation.
*/
template <bool value, typename consequent, typename alternate>
struct branch {
    using type = typename consequent::type;
};

template <typename consequent, typename alternate>
struct branch<false, consequent, alternate> {
    using type = typename alternate::type;
};

template <bool value, typename consequent, typename alternate>
using branch_t = typename branch<value, consequent, alternate>::type;
