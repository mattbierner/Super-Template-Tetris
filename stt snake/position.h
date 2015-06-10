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
};


