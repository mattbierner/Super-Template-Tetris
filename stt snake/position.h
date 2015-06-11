#pragma once

/**
    Position in two dimensional space.
*/
template <size_t xVal, size_t yVal>
struct Position {
    static const size_t x = xVal;
    static const size_t y = yVal;
    
    template <typename p2>
    using add = Position<x + p2::x, y + p2::y>;
    
    template <typename p2>
    using sub = Position<x - p2::x, y - p2::y>;
};

enum class Orientation
{
    Vertical,
    Horizontal
};

/*------------------------------------------------------------------------------
    Printer
*/
template <size_t x, size_t y>
struct Printer<Position<x, y>>
{
    static void Print(std::ostream& output)
    {
        output << "<" << x << ", " << y << ">";
    }
};

/*------------------------------------------------------------------------------
    Serialize
*/
template <size_t x, size_t y>
struct Serialize<Position<x, y>>
{
    static std::ostream& Write(std::ostream& output)
    {
        output << "Position<" << x << "," << y << ">";
        return output;
    }
};
