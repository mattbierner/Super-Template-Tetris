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
constexpr const size_t worldHeight = 3;//20;

/**
    Height of the area at the top of the playfield.
*/
constexpr const size_t deathZoneHeight = 4;

/**
    Initial game world.
*/
using InitialWorld = gen_grid<worldWidth, worldHeight + deathZoneHeight, x_cell>;

/**
    Is a cell empty?
*/
template <typename x>
using is_empty = std::is_same<x, empty_pixel>;

/**
*/
template <typename pos, typename grid>
struct playfield_is_empty {
    struct check {
        using type = is_empty<grid_get<pos, grid>>;
    };

    using type = logical_and<
        grid_is_in_bounds<pos, grid>::value,
        check>;
};

/**
    Get a list of all valid positions.
*/
template <typename grid, typename offset = Position<0, 0>>
struct PlayfieldGetPositionsReducer {
    template <typename p, typename c>
    using apply =
        std::conditional<is_empty<caar<c>>::value,
            p,
            cons<typename car<c>::template add<offset>, p>>;
    
};

template <typename grid, typename offset = Position<0, 0>>
using playfield_get_positions =
    fold<
        PlayfieldGetPositionsReducer<grid, offset>,
        List<>,
        grid_zip_positions<grid>>;


/**
*/
template <typename position, typename block, typename grid>
struct PlayfieldIsCollidingCheck {
    template <typename c>
    using apply =
        identity<std::integral_constant<bool,
            !playfield_is_empty<c, grid>::type::value>>;
};

template <typename position, typename block, typename grid>
using playfield_is_colliding =
    any<
        PlayfieldIsCollidingCheck<position, block, grid>,
        playfield_get_positions<block, position>>;