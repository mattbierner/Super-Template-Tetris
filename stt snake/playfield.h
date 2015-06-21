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
struct CheckIsEmpty {
    using type = is_empty<grid_get<pos, grid>>;
};

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
    Check if row `N` is full.
*/
template <size_t N, typename g>
constexpr const bool playfield_row_is_full =
    !any<
        mfunc<IsEmpty>,
        get<N, typename g::rows>>;

static_assert(
    !playfield_row_is_full<0,
        gen_grid<2, 2, empty_pixel>>, "");

static_assert(
    playfield_row_is_full<0,
        gen_grid<2, 2, Pixel<'x'>>>, "");

static_assert(
    !playfield_row_is_full<0,
        grid_put<
            Position<0, 0>,
            empty_pixel,
            gen_grid<2, 2, Pixel<'x'>>>>, "");

/**
    Get the indicies of all full rows.
*/
template <size_t n, typename g>
struct PlayfieldGetFullRows {
    using rest = typename PlayfieldGetFullRows<n + 1, Grid<cdr<typename g::rows>>>::type;
    
    using type =
        std::conditional_t<playfield_row_is_full<0, g>,
            cons<std::integral_constant<size_t, n>, rest>,
            rest>;
};

template <size_t n>
struct PlayfieldGetFullRows<n, Grid<List<>>> {
    using type = List<>;
};

template <typename g>
using playfield_get_full_rows = typename PlayfieldGetFullRows<0, g>::type;

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