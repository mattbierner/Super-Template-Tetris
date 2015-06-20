#pragma once

#include "color.h"
#include "string.h"

/**
    Styling that tells how to render pixels.
*/
template <Color fg, Color bg>
struct Gfx {
    static constexpr const Color foreground = fg;
    static constexpr const Color background = bg;
    
    template <Color newColor>
    using setBg = Gfx<fg, newColor>;
    
    template <Color newColor>
    using setFg = Gfx<newColor, bg>;
};

/**
    Standard graphics.
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
    
    Is not drawn when combining grids.
*/
struct empty_pixel { };

/*------------------------------------------------------------------------------
    ToString
*/
template <>
struct ToString<empty_pixel> {
    using type = String<' '>;
};

template <char val, typename gfx>
struct ToString<Pixel<val, gfx>> {
    using type =
        string_join<
            typename string_join<
                color_to_fg_code<gfx::foreground>,
                color_to_bg_code<gfx::background>>::template append<val>,
            colorReset>;
};

/*------------------------------------------------------------------------------
    Printer
*/
template <>
struct Printer<empty_pixel> {
    static void Print(std::ostream& output)
    {
        output << ' ';
    }
};

template <char val, typename gfx>
struct Printer<Pixel<val, gfx>> {
    static void Print(std::ostream& output)
    {
        Printer<to_string<Pixel<val, gfx>>>::Print(output);
    }
};
