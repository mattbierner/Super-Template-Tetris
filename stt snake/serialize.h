#pragma once

#include <ostream>

/**
    Interface for an object that can be serialized.
*/
template <typename>
struct Serialize;

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

/*------------------------------------------------------------------------------
    Basic Type Serialization
*/
template <>
struct Serialize<bool> { static std::ostream& Write(std::ostream& output) { return output << "boo"; } };

template <>
struct Serialize<int> { static std::ostream& Write(std::ostream& output) { return output << "int"; } };

template <>
struct Serialize<unsigned> { static std::ostream& Write(std::ostream& output) { return output << "unsigned"; } };

template <>
struct Serialize<size_t> { static std::ostream& Write(std::ostream& output) { return output << "size_t"; } };

/*------------------------------------------------------------------------------
    Value Type Serialization
*/
/**
    Wrapper around a type that can be serialized.
*/
template <typename T, T x>
struct SerializableValue { };

template <typename T, T x>
struct Serialize<SerializableValue<T, x>>
{
    static std::ostream& Write(std::ostream& output) { return output << std::boolalpha << x; }
};

/**
    Serializes an integer_sequence.
*/
template <typename T, T... elements>
struct Serialize<std::integer_sequence<T, elements...>>
{
    static std::ostream& Write(std::ostream& output)
    {
        output << "std::integer_sequence<";
        Join<',', T, SerializableValue<T, elements>...>::Write(output);
        return output << ">";
    }
};

