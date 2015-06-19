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
constexpr const size_t worldHeight = 20;

/**
    Height of the area at the top of the playfield.
*/
constexpr const size_t deathZoneHeight = 4;

/**
    The initial game world.
*/
using InitialWorld = gen_grid<worldWidth, worldHeight + deathZoneHeight, x_cell>;

/**
    Get a list of all valid positions.
*/
template <typename grid, typename offset = Position<0, 0>>
struct PlayfieldGetPositions {
    struct reducer {
        template <typename p, typename c>
        using apply = std::conditional<std::is_same<caar<c>, empty_pixel>::value,
            p,
            cons<typename car<c>::template add<offset>, p>>;
    };
    
    using type = fold<reducer, List<>, grid_zip_positions<grid>>;
};

template <typename grid, typename offset = Position<0, 0>>
using playfield_get_positions = typename PlayfieldGetPositions<grid, offset>::type;

/**
*/
template <typename pos, typename grid>
struct playfield_is_empty {
    struct check {
        using type = std::is_same<grid_get<pos, grid>, empty_pixel>;
    };

    using type = logical_and<
        grid_is_in_bounds<pos, grid>::value,
        check>;
};

/**
*/
template <typename position, typename block, typename grid>
struct PlayfieldIsColliding {
    struct xx {
        template <typename c>
        using apply = identity<std::integral_constant<bool,
            !playfield_is_empty<c, grid>::type::value>>;
    };

    using type = any<xx, playfield_get_positions<block, position>>;
};

template <typename position, typename block, typename grid>
using playfield_is_colliding = typename PlayfieldIsColliding<position, block, grid>::type;