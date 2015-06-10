#pragma once

#include <iomanip>

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

/**
    Smallest visual unit.
    
    Draws a character with a foreground and background color.
*/
template <char val, Color fg = Color::Default, Color bg = Color::Default>
struct Pixel {
    static const char value = val;
    static const Color foreground = fg;
    static const Color background = bg;
};

/*------------------------------------------------------------------------------
    Printer
*/
template <char val, Color fg, Color bg>
struct Printer<Pixel<val, fg, bg>>
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
        output << toFgCode(fg) << toBgCode(bg) << val << colorReset;
    }
};
