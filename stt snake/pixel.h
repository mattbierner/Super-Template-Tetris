#pragma once

#include "color.h"
#include "string.h"
#include "serialize.h"

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

/**
    Is a cell empty?
*/
template <typename x>
struct IsEmpty {
    using type = std::is_same<x, empty_pixel>;
};

template <typename x>
constexpr const bool is_empty = std::is_same<x, empty_pixel>::value;

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
        string_add<
            typename string_add<
                color_to_fg_code<gfx::foreground>,
                color_to_bg_code<gfx::background>>::template append<val>,
            colorReset>;
};

/*------------------------------------------------------------------------------
    Serialize
*/
template <>
struct SerializeToString<empty_pixel> {
    using type = decltype("empty_pixel"_string);
};

template <Color x>
struct SerializeToString<SerializableValue<Color, x>> {
    using type =
        serialize_enum_to_string<decltype("Color"_string), Color, x>;
};

template <char val, typename gfx>
struct SerializeToString<Pixel<val, gfx>> {
    using type =
        serialize_class_to_string<decltype("Pixel"_string),
            SerializableValue<char, val>,
            gfx>;
};


template <Color fg, Color bg>
struct SerializeToString<Gfx<fg, bg>> {
    using type =
        serialize_class_to_string<decltype("Gfx"_string),
            SerializableValue<Color, fg>,
            SerializableValue<Color, bg>>;
};
