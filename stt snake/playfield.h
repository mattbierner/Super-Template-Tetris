#pragma once

#include "blocks.h"
#include "grid.h"
#include "boolean.h"

/**
    Default width of the field.
*/
constexpr const size_t worldWidth = 10;

/**
    Default height of the field.
*/
constexpr const size_t worldHeight = 22;

/**
    The initial game world.
*/
using InitialWorld = gen_grid<worldWidth, worldHeight, x_cell>;

/**
    Is a given position in the bounds of the world?
*/
template <typename pos, typename world>
using is_in_bounds =
    std::integral_constant<bool,
        pos::x < world::size && pos::y < world::size>;

/**
    Function that checks if cell is of a given type.
*/
template <typename cell, typename pos, typename world>
struct is_type :
    std::integral_constant<bool,
        grid_get<pos, world>::state == cell::state> { };

/**
    Is the cell at `pos` in `world` empty?
*/
template <typename pos, typename world>
struct is_empty :
    logical_and<
        is_in_bounds<pos, world>::value,
        Thunk<is_type, x_cell, pos, world>> { };


