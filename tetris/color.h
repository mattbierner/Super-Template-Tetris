#pragma once

#include "string.h"

/**
    Supported drawing colors.
*/
enum class Color : unsigned {
    Black = 0,
    Red = 1,
    Green = 2,
    Yellow = 3,
    Blue = 4,
    Magenta = 5,
    Cyan = 6,
    White = 7,
    Default = 9
};

/**
    Convert a color to its foreground color code.
*/
template <Color c>
using color_to_fg_code =
    string_join<String<>,
        decltype("\x1b["_string),
        int_to_string<30 + static_cast<unsigned>(c)>,
        String<'m'>>;
    
/**
    Convert a color to its background color code.
*/
template <Color c>
using color_to_bg_code =
    string_join<String<>,
        decltype("\x1b["_string),
        int_to_string<40 + static_cast<unsigned>(c)>,
        String<'m'>>;

/**
    Reset all colors.
*/
using colorReset = decltype("\x1b[0m"_string);
