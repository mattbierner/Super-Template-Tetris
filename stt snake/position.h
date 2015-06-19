#pragma once

/**
    Position in two dimensional space.
*/
template <int xVal, int yVal>
struct Position {
    static const int x = xVal;
    static const int y = yVal;
    
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
template <int x, int y>
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
template <int x, int y>
struct Serialize<Position<x, y>>
{
    static std::ostream& Write(std::ostream& output)
    {
        output << "Position<" << x << "," << y << ">";
        return output;
    }
};
