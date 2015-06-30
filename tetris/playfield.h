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
    Initial game world.
*/
using InitialWorld = gen_grid<worldWidth, worldHeight + deathZoneHeight, x_cell>;

/**
    Is a given position is empty?
    
    Positions outside of the grid are not considered empty.
*/
template <typename pos, typename grid>
struct CheckIsEmpty :
    IsEmpty<grid_get<pos, grid>> { };

template <typename pos, typename grid>
constexpr const bool playfield_is_empty =
    logical_and<
        grid_is_in_bounds<pos, grid>,
        Thunk<CheckIsEmpty, pos, grid>>::value;

/**
    Get a list of all non empty positions.
*/
template <typename grid, typename offset>
struct PlayfieldGetPositionsReducer {
    template <typename p, typename c>
    using apply =
        std::conditional<is_empty<caar<c>>,
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
    Check if a collision is occuring for a given block.
*/
template <typename position, typename block, typename grid>
struct PlayfieldIsCollidingCheck {
    template <typename c>
    using apply =
        identity<std::integral_constant<bool,
            !playfield_is_empty<c, grid>>>;
};

template <typename position, typename block, typename grid>
constexpr const bool playfield_is_colliding =
    any<
        PlayfieldIsCollidingCheck<position, block, grid>,
        playfield_get_positions<block, position>>;

/**
    Get the indicies of all full rows.
*/
struct PlayfieldGetFullRows {
    template <typename p, typename c>
    using apply =
        identity<List<
            std::conditional_t<any<mfunc<IsEmpty>, c>,
                car<p>,
                cons<std::integral_constant<size_t, caar<p>::value>, car<p>>>,
            std::integral_constant<size_t, caar<p>::value + 1>>>;
};

template <typename g>
using playfield_get_full_rows = car<
    fold<
        PlayfieldGetFullRows,
        List<List<>, std::integral_constant<size_t, 0>>,
        typename g::rows>>;

static_assert(
    std::is_same<
        List<>,
        playfield_get_full_rows<
            gen_grid<2, 2, empty_pixel>>>::value, "");

static_assert(
    std::is_same<
        List<
            std::integral_constant<size_t, 1>>,
        playfield_get_full_rows<
            grid_put<
                Position<0, 0>,
                empty_pixel,
                gen_grid<2, 2, Pixel<'x'>>>>>::value, "");

/**
    Remove row `i` from grid, adding a new empty row at the top.
*/
template <size_t i, typename g>
using playfield_remove_row =
    grid_cons_row<
        gen<g::width, empty_pixel>,
        grid_remove_row<i, g>>;

