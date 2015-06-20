#pragma once

#include "list.h"
#include "functor.h"
#include "printer.h"
#include "position.h"
#include <type_traits>
#include "utility.h"

/**
    Two dimensional grid of values.
*/
template <typename r>
struct Grid {
    using rows = r;
    
    static constexpr const size_t width = get_t<0, r>::size;
    static constexpr const size_t height = rows::size;
    
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
using gen_grid = Grid<gen_t<height, gen_t<width, value>>>;

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
        gen_t<size, cell>>;

/**
    Get the element at `pos(x, y)` in a grid.
*/
template <typename pos, typename grid>
using grid_get = get_t<pos::x, get_t<pos::y, typename grid::rows>>;

/**
    Set the element at `pos(x, y)` in a grid to `value`.
*/
template <typename pos, typename value, typename g>
using grid_put = Grid<
    put_t<
        pos::y,
        put_t<pos::x, value, get_t<pos::y, typename g::rows>>,
        typename g::rows>>;

/**
    Modify the current value at pos using a function that takes the current value
    and the value to be placed.
*/
template <typename combine, typename pos, typename value, typename g>
using grid_try_put = grid_put<
    pos,
    call<combine, grid_get<pos, g>, value>,
    g>;

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
using grid_is_in_xbounds =
    std::integral_constant<bool,
        (pos::x >= 0 && pos::x < g::width)>;


/**
    Is `pos` within the height of the grid?
*/
template <typename pos, typename g>
using grid_is_in_ybounds =
    std::integral_constant<bool,
        (pos::y >= 0 && pos::y < g::height)>;

/**
    Is `pos` within the grid?
*/
template <typename pos, typename g>
using grid_is_in_bounds =
    std::integral_constant<bool,
        grid_is_in_xbounds<pos, g>::value && grid_is_in_ybounds<pos, g>::value>;

/**
    Set the element at `pos(x, y)` in a grid to `value`.
*/
template <typename combine, typename origin, typename row, typename grid>
struct GridPlaceRow {
    struct IsInBounds {
        using type = grid_try_put<
            combine,
            origin,
            car<row>,
            typename GridPlaceRow<
                combine,
                typename origin::template add<Position<1, 0>>,
                cdr<row>,
                grid>::type>;
    };

    using type = branch<grid_is_in_bounds<origin, grid>::value,
        IsInBounds,
        identity<grid>>;
};

template <typename combine, typename origin, typename grid>
struct GridPlaceRow<combine, origin, List<>, grid> {
    using type = grid;
};

template <typename combine, typename origin, typename row, typename grid>
using grid_place_row = typename GridPlaceRow<combine, origin, row, grid>::type;

/**
    Draw another buffer on top of this buffer.
*/
template <typename combine, typename origin, typename other, typename grid>
struct GridPlaceGrid;

template <typename combine, typename origin, typename other, typename grid>
using grid_place_grid = typename GridPlaceGrid<combine, origin, other, grid>::type;

template <typename combine, typename origin, typename otherRows, typename g>
struct GridPlaceGrid<combine, origin, Grid<otherRows>, g> {
    using type = grid_place_row<
        combine,
        origin,
        car<otherRows>,
        grid_place_grid<
            combine,
            typename origin::template add<Position<0, 1>>,
            Grid<cdr<otherRows>>,
            g>>;
};

template <typename combine, typename origin, typename g>
struct GridPlaceGrid<combine, origin, Grid<List<>>, g> {
    using type = g;
};

/*------------------------------------------------------------------------------
    Printer
*/
template <>
struct Printer<Grid<List<>>>
{
    static void Print(std::ostream& output) { /* noop */ }
};

template <typename x, typename... xs>
struct Printer<Grid<List<x, xs...>>>
{
    static void Print(std::ostream& output)
    {
        Printer<x>::Print(output);
        output << "\n";
        Printer<Grid<List<xs...>>>::Print(output);
    }
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
    
    using type = Grid<
        f_map<inner, rows>>;
};

/*------------------------------------------------------------------------------
    Serialize
*/
template <typename rows>
struct SerializeToString<Grid<rows>> {
    using type =
        serialize_class_to_string<decltype("Grid"_string),
            rows>;
};
