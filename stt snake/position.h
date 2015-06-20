#pragma once

/**
    Position in two dimensional space.
*/
template <int xVal, int yVal>
struct Position {
    static constexpr const int x = xVal;
    static constexpr const int y = yVal;
    
    template <typename p2>
    using add = Position<x + p2::x, y + p2::y>;
    
    template <typename p2>
    using sub = Position<x - p2::x, y - p2::y>;
};

/**
*/
enum class Orientation {
    Vertical,
    Horizontal
};

/**
*/
template <Orientation o, int size>
using create_offset =
    std::conditional_t<o == Orientation::Vertical,
        Position<0, size>,
        Position<size, 0>>;

/**
*/
template <size_t w, size_t h>
struct Size {
    static constexpr const size_t width = w;
    static constexpr const size_t height = h;
};



/*------------------------------------------------------------------------------
    Printer
*/
template <int x, int y>
struct Printer<Position<x, y>> {
    static void Print(std::ostream& output)
    {
        output << "<" << x << ", " << y << ">";
    }
};

/*------------------------------------------------------------------------------
    Serialize
*/
template <int x, int y>
struct Serialize<Position<x, y>> {
    static std::ostream& Write(std::ostream& output)
    {
        output << "Position<" << x << "," << y << ">";
        return output;
    }
};
