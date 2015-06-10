/**
    Basic specilizable interface for a type that can be printed for display.
*/
#pragma once

#include <utility>

template <typename>
struct Printer;

template <typename T, T x>
struct Printer<std::integral_constant<T, x>>
{
    static void Print(std::wostream& output)
    {
        output << x;
    }
};
