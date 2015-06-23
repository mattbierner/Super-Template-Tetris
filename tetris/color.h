#pragma once

#include "string.h"

/**
    Supported drawing colors.
*/
enum class Color : unsigned {
    Default,
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White
};

/**
    Convert a color to its foreground color code.
*/
template <Color c>
struct ColorToFgCode { using type = decltype("\x1b[39m"_string); };
template <> struct ColorToFgCode<Color::Black> {    using type = decltype("\x1b[30m"_string); };
template <> struct ColorToFgCode<Color::Red> {      using type = decltype("\x1b[31m"_string); };
template <> struct ColorToFgCode<Color::Green> {    using type = decltype("\x1b[32m"_string); };
template <> struct ColorToFgCode<Color::Yellow> {   using type = decltype("\x1b[33m"_string); };
template <> struct ColorToFgCode<Color::Blue> {     using type = decltype("\x1b[34m"_string); };
template <> struct ColorToFgCode<Color::Magenta> {  using type = decltype("\x1b[35m"_string); };
template <> struct ColorToFgCode<Color::Cyan> {     using type = decltype("\x1b[36m"_string); };
template <> struct ColorToFgCode<Color::White> {    using type = decltype("\x1b[37m"_string); };

template <Color c>
using color_to_fg_code = typename ColorToFgCode<c>::type;

/**
    Convert a color to its background color code.
*/
template <Color c>
struct ColorToBgCode { using type = decltype("\x1b[49m"_string); };
template <> struct ColorToBgCode<Color::Black> {    using type = decltype("\x1b[40m"_string); };
template <> struct ColorToBgCode<Color::Red> {      using type = decltype("\x1b[41m"_string); };
template <> struct ColorToBgCode<Color::Green> {    using type = decltype("\x1b[42m"_string); };
template <> struct ColorToBgCode<Color::Yellow> {   using type = decltype("\x1b[43m"_string); };
template <> struct ColorToBgCode<Color::Blue> {     using type = decltype("\x1b[44m"_string); };
template <> struct ColorToBgCode<Color::Magenta> {  using type = decltype("\x1b[45m"_string); };
template <> struct ColorToBgCode<Color::Cyan> {     using type = decltype("\x1b[46m"_string); };
template <> struct ColorToBgCode<Color::White> {    using type = decltype("\x1b[47m"_string); };

template <Color c>
using color_to_bg_code = typename ColorToBgCode<c>::type;

/**
    Reset all colors.
*/
using colorReset = decltype("\x1b[0m"_string);
