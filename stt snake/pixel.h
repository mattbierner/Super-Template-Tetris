#pragma once

#include "string.h"

/**
    Supported drawing colors.
*/
enum class Color
{
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

template <Color c>
struct ColorToFgCode { using type = decltype("\x1b[39m"_string); };
template <> struct ColorToFgCode<Color::Black> {    using type = decltype("\x1b[30m"_string); };
template <> struct ColorToFgCode<Color::Red> {      using type = decltype("\x1b[31m"_string); };
template <> struct ColorToFgCode<Color::Green> {    using type = decltype("\x1b[32m"_string); };
template <> struct ColorToFgCode<Color::Yellow> {   using type = decltype("\x1b[33m"_string); };
template <> struct ColorToFgCode<Color::Blue> {     using type = decltype("\x1b[34m"_string); };
template <> struct ColorToFgCode<Color::Magenta> {  using type = decltype("\x1b[35m"_string); };
template <> struct ColorToFgCode<Color::Cyan> {     using type = decltype("\x1b[36m"_string); };
template <> struct ColorToFgCode<Color::White> {     using type = decltype("\x1b[37m"_string); };

template <Color c>
using color_to_fg_code = typename ColorToFgCode<c>::type;

/**
    Styling that tells how to render pixels.
*/
template <Color fg, Color bg>
struct Gfx {
    static const Color foreground = fg;
    static const Color background = bg;
    
    template <Color newColor>
    using setBg = Gfx<fg, newColor>;
    
    template <Color newColor>
    using setFg = Gfx<newColor, bg>;
};

/**
    Use standard graphics.
*/
using default_gfx = Gfx<Color::Default, Color::Default>;

/**
    Smallest visual unit.
    
    Draws a character with a foreground and background color.
*/
template <char val, typename gfx = default_gfx>
struct Pixel {
    static constexpr const char value = val;
    static constexpr const Color foreground = gfx::foreground;
    static constexpr const Color background = gfx::background;
};

/**
    Transparent / nothing pixel.
    
    Will not be drawn when combining grids.
*/
struct empty_pixel { };

/*------------------------------------------------------------------------------
    Printer
*/
template <>
struct Printer<empty_pixel>
{
    static void Print(std::ostream& output)
    {
        output << ' ';
    }
};

template <char val, typename gfx>
struct Printer<Pixel<val, gfx>>
{
    static constexpr const char* colorReset =  "\x1b[0m";

    static const char* toFgCode(Color color)
    {
        switch (color)
        {
        case Color::Black:      return "\x1b[30m";
        case Color::Red:        return "\x1b[31m";
        case Color::Green:      return "\x1b[32m";
        case Color::Yellow:     return "\x1b[33m";
        case Color::Blue:       return "\x1b[34m";
        case Color::Magenta:    return "\x1b[35m";
        case Color::Cyan:       return "\x1b[36m";
        case Color::White:      return "\x1b[37m";
        default:                return "\x1b[39m";
        }
    };
    
    static const char* toBgCode(Color color)
    {
        switch (color)
        {
        case Color::Black:      return "\x1b[40m";
        case Color::Red:        return "\x1b[41m";
        case Color::Green:      return "\x1b[42m";
        case Color::Yellow:     return "\x1b[43m";
        case Color::Blue:       return "\x1b[44m";
        case Color::Magenta:    return "\x1b[45m";
        case Color::Cyan:       return "\x1b[46m";
        case Color::White:      return "\x1b[47m";
        default:                return "\x1b[49m";
        }
    };
    
    static void Print(std::ostream& output)
    {
        Printer<color_to_fg_code<gfx::foreground>>::Print(output);
        output << toBgCode(gfx::background) << val << colorReset;
    }
};
