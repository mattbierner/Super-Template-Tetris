#pragma once

#include "input.h"
#include "position.h"
#include "serialize.h"

/**
    Direction in two dimensional space.
*/
enum class Direction : unsigned
{
    Up,
    Down,
    Left,
    Right
};

/**
    Get the change in `x` for a given direction.
*/
template <Direction direction>
struct direction_delta_x : std::integral_constant<int, 0> { };

template <>
struct direction_delta_x<Direction::Left> : std::integral_constant<int, -1> { };

template <>
struct direction_delta_x<Direction::Right> : std::integral_constant<int, 1> { };

/**
    Get the change in `y` for a given direction.
*/
template <Direction direction>
struct direction_delta_y : std::integral_constant<int, 0> { };

template <>
struct direction_delta_y<Direction::Up> : std::integral_constant<int, -1> { };

template <>
struct direction_delta_y<Direction::Down> : std::integral_constant<int, 1> { };

/*------------------------------------------------------------------------------
    Serialize
*/
template <Direction d>
struct Serialize<SerializableValue<Direction, d>>
{
    static std::ostream& Write(std::ostream& output)
    {
        output << "Direction::";
        switch (d)
        {
        case Direction::Up: output << "Up"; break;
        case Direction::Down: output << "Down"; break;
        case Direction::Left: output << "Left"; break;
        case Direction::Right: output << "Right"; break;
        }
        return output;
    }
};

    