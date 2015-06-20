#pragma once

#include <ostream>
#include "string.h"

/**
    Interface for an object that can be serialized to a string.
    
    Objects are serialized to C++.
*/
template <typename>
struct SerializeToString;

template <typename x>
using serialize_to_string = typename SerializeToString<x>::type;

template <char... chars>
struct SerializeToString<String<chars...>> {
    using type = String<chars...>;
};

/**
    Serialize templated class `name` with `elements` template paramters.
*/
template <typename name, typename... elements>
using serialize_class_to_string =
    string_add<
        name,
        string_add<
            String<'<'>,
            string_add<
                string_join<String<','>, serialize_to_string<elements>...>,
                String<'>'>>>>;

/**
    Serialize enum class `name` of type `t` and value `x`.
*/
template <typename name, typename t, t x>
using serialize_enum_to_string =
    string_add<
        string_add<
            decltype("static_cast<"_string),
            string_add<
                name,
                String<'>', '('>>>,
        string_add<
            int_to_string<static_cast<unsigned>(x)>,
            String<')'>>>;

/*------------------------------------------------------------------------------
    Basic Type Serialization
*/
template <>
struct SerializeToString<bool> { using type = decltype("bool"_string); };

template <>
struct SerializeToString<int> { using type = decltype("int"_string); };

template <>
struct SerializeToString<unsigned> { using type = decltype("unsigned"_string); };

template <>
struct SerializeToString<size_t> { using type = decltype("size_t"_string); };

/*------------------------------------------------------------------------------
    Value Type Serialization
*/
/**
    Wrapper around a type that can be serialized.
*/
template <typename T, T x>
struct SerializableValue { };

template <typename T, T x>
struct SerializeToString<SerializableValue<T, x>> {
    using type = int_to_string<x>;
};

template <>
struct SerializeToString<SerializableValue<bool, false>> {
    using type = decltype("false"_string);
};

template <>
struct SerializeToString<SerializableValue<bool, true>> {
    using type = decltype("true"_string);
};
