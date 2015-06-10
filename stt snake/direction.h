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

/**
    For a given input, determine the new direction.
    
    Allows moving at 0, 90, and -90 angles but no 180s (which results in a noop).
*/
template <Direction direction, Input input>
struct get_new_direction : std::integral_constant<Direction, direction> { };

template <Direction direction>
struct get_new_direction<direction, Input::Up> : std::integral_constant<Direction, (direction == Direction::Down ? direction : Direction::Up)> { };

template <Direction direction>
struct get_new_direction<direction, Input::Down> : std::integral_constant<Direction, (direction == Direction::Up ? direction : Direction::Down)> { };

template <Direction direction>
struct get_new_direction<direction, Input::Left> : std::integral_constant<Direction, (direction == Direction::Right ? direction : Direction::Left)> { };

template <Direction direction>
struct get_new_direction<direction, Input::Right> : std::integral_constant<Direction, (direction == Direction::Left ? direction : Direction::Right)> { };

/**
    Move from `pos` in `direction`
*/
template <Direction direction, typename pos>
using get_next_position =
    Position<
        pos::x + direction_delta_x<direction>::value,
        pos::y + direction_delta_y<direction>::value>;


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

    