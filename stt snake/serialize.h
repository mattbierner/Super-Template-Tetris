#pragma once

#include <ostream>
#include "string.h"

/**
    Interface for an object that can be serialized.
*/
template <typename, typename = void>
struct Serialize;

template <char x, char... xs>
struct Serialize<String<x, xs...>> {
    static std::ostream& Write(std::ostream& output)
    {
        return Serialize<String<xs...>>::Write(output << x);
    }
};

template <>
struct Serialize<String<>> {
    static std::ostream& Write(std::ostream& output)
    {
        return output;
    }
};

/**
    Interface for an object that can be serialized to a string.
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
    Serializes a list of elements, seperating neighboring elements using `joiner`.
*/
template <char joiner, typename... list>
struct Join;

template <char joiner, typename first, typename second, typename... rest>
struct Join<joiner, first, second, rest...>
{
    static std::ostream& Write(std::ostream& output)
    {
        Serialize<first>::Write(output);
        output << joiner;
        return Join<joiner, second, rest...>::Write(output);
    }
};

template <char joiner, typename first>
struct Join<joiner, first>
{
    static std::ostream& Write(std::ostream& output)
    {
        return Serialize<first>::Write(output);
    }
};

template <char joiner>
struct Join<joiner>
{
    static std::ostream& Write(std::ostream& output)
    {
        return output;
    }
};

template <typename name, typename... elements>
using serialize_class_to_string =
    string_add<
        name,
        string_add<
            String<'<'>,
            string_add<
                string_join<String<','>, serialize_to_string<elements>...>,
                String<'>'>>>>;

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
