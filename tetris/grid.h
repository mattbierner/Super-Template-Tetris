#pragma once

#include "list.h"
#include "functor.h"
#include "position.h"
#include <type_traits>
#include "utility.h"

/**
    Two dimensional grid of values.
*/
template <typename r>
struct Grid {
    using rows = r;
    
    struct GetWidth {
        using type = std::integral_constant<size_t, get<0, r>::size>;
    };
    
    static constexpr const size_t height = rows::size;
    static constexpr const size_t width =
        branch<height == 0,
            identity<std::integral_constant<size_t, 0>>,
            GetWidth>::value;

    template <typename pos>
    using nextPosition =
        Position<
            (pos::x + 1) % width,
            pos::x + 1 == width ? pos::y + 1 : pos::y>;
};

/**
    Generate an `width` by `height` grid of `value`.
*/
template <size_t width, size_t height, typename value>
using gen_grid = Grid<gen<height, gen<width, value>>>;

/**
    Create a single line grid from a list.
*/
template <Orientation orientation, typename list>
using create_list_grid =
    Grid<
        std::conditional_t<orientation == Orientation::Vertical,
            f_map<mfunc<List>, list>,
            List<list>>>;

/**
    Create a single line grid.
*/
template <Orientation orientation, size_t size, typename cell>
using create_line_grid =
    create_list_grid<
        orientation,
        gen<size, cell>>;

/**
    Get the element at `pos(x, y)` in a grid.
*/
template <typename pos, typename grid>
using grid_get = get<pos::x, get<pos::y, typename grid::rows>>;

/**
    Set the element at `pos(x, y)` in a grid to `value`.
*/
template <typename pos, typename value, typename g>
using grid_put = Grid<
    put<
        pos::y,
        put<pos::x, value, get<pos::y, typename g::rows>>,
        typename g::rows>>;

/**
    Remove row `N` from a grid.
*/
template <size_t N, typename g>
using grid_remove_row =
    Grid<
        slice_out<N, typename g::rows>>;

static_assert(
    std::is_same<
        Grid<List<
            List<float>,
            List<bool>>>,
        grid_remove_row<0, Grid<List<
            List<int>,
            List<float>,
            List<bool>>>
        >>::value, "");

static_assert(
    std::is_same<
        Grid<List<
            List<int>,
            List<bool>>>,
        grid_remove_row<1, Grid<List<
            List<int>,
            List<float>,
            List<bool>>>
        >>::value, "");

/**
    Add a new row to the head of grid, shifting the rest of the grid down by one.
*/
template <typename newRow, typename g>
using grid_cons_row =
    Grid<
        cons<newRow, typename g::rows>>;

static_assert(
    std::is_same<
        Grid<List<
            List<bool>>>,
        grid_cons_row<List<bool>, Grid<List<>>>
        >::value, "");

static_assert(
    std::is_same<
        Grid<List<
            List<bool>,
            List<int>>>,
        grid_cons_row<
            List<bool>,
            Grid<List<List<int>>>>
        >::value, "");

/**
    Transform a grid into a grid of cordinate value pairs.
*/
template <typename g>
struct GridZipPositions {
    template <typename p, typename c>
    struct apply {
        using pos = car<p>;
        using grid = caar<p>;
        using nextPos = typename grid::template nextPosition<pos>;
        using type = List<
            nextPos,
            grid_put<pos, List<pos, c>, grid>>;
    };
};

template <typename g>
using grid_zip_positions =
    caar<fold<
        GridZipPositions<g>,
        List<Position<0, 0>, g>,
        g>>;

/**
    Is `pos` within the width of the grid?
*/
template <typename pos, typename g>
constexpr bool grid_is_in_xbounds = pos::x >= 0 && pos::x < g::width;

/**
    Is `pos` within the height of the grid?
*/
template <typename pos, typename g>
constexpr bool grid_is_in_ybounds = pos::y >= 0 && pos::y < g::height;

/**
    Is `pos` within the grid?
*/
template <typename pos, typename g>
constexpr const bool grid_is_in_bounds = grid_is_in_xbounds<pos, g> && grid_is_in_ybounds<pos, g>;

/**
    Modify the current value at pos using a function that takes the current value
    and the value to be placed.
*/
template <typename combine, typename pos, typename value, typename g>
struct GridTryPut {
    struct DoPut {
        using type = grid_put<
            pos,
            call<combine, grid_get<pos, g>, value>,
            g>;
    };

    using type =
        branch<grid_is_in_bounds<pos, g>,
            DoPut,
            identity<g>>;
};

/**
    Place a row of value on this grid using binary function `combine`.
*/
template <typename combine>
struct GridPlaceRow {
    template <typename p, typename c>
    struct apply {
        using type = List<
            typename GridTryPut<combine, caar<p>, c, car<p>>::type,
            typename caar<p>::template add<Position<1, 0>>>;
    };
};

template <typename combine, typename origin, typename row, typename grid>
using grid_place_row = car<fold<GridPlaceRow<combine>, List<grid, origin>, row>>;

/**
    Place another grid with this grid using binary function `combine`.
*/
template <typename combine>
struct GridPlaceGrid {
    template <typename p, typename c>
    struct apply {
        using type = List<
            grid_place_row<combine, caar<p>, c, car<p>>,
            typename caar<p>::template add<Position<0, 1>>>;
    };
};

template <typename combine, typename origin, typename other, typename grid>
using grid_place_grid = car<fold<GridPlaceGrid<combine>, List<grid, origin>, typename other::rows>>;

/*------------------------------------------------------------------------------
    ToString
*/
template <typename... xs>
struct ToString<Grid<List<xs...>>> {
    using type = string_join<String<'\n'>, xs...>;
};

/*------------------------------------------------------------------------------
    Foldable
*/
template <typename f, typename z, typename rows>
struct Foldable<f, z, Grid<rows>> {
    struct inner {
        template <typename p, typename c>
        using apply = identity<fold<f, p, c>>;
    };

    using type = fold<inner, z, rows>;
};

/*------------------------------------------------------------------------------
    Functor
*/
template <typename f, typename rows>
struct Fmap<f, Grid<rows>> {
    struct inner {
        template <typename x>
        using apply = identity<f_map<f, x>>;
    };
    
    using type = Grid<f_map<inner, rows>>;
};

/*------------------------------------------------------------------------------
    Serialize
*/
template <typename rows>
struct Serialize<Grid<rows>> {
    using type =
        serialize_class<decltype("Grid"_string),
            rows>;
};
