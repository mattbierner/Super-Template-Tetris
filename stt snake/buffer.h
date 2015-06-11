#pragma once

#include "direction.h"
#include "grid.h"
#include "pixel.h"

/**
    Two dimensional grid of values.
    
    Stores a list of column lists that store cells.
*/
template <typename rows>
using Buffer = Grid<rows>;

/**
    Set the element at `pos(x, y)` in a grid to `value`.
*/
template <typename pos, typename cell, typename g>
using try_grid_put =
    std::conditional_t<cell::value == '\0',
        g,
        grid_put<pos, cell, g>>;

/**
    Set the element at `pos(x, y)` in a grid to `value`.
*/
template <typename origin, typename row, typename grid>
struct draw_row {
    using type = try_grid_put<
        origin,
        car_t<row>,
        typename draw_row<
            typename origin::template add<Position<1, 0>>,
            cdr_t<row>,
            grid>::type>;
};

template <typename origin,typename grid>
struct draw_row<origin, List<>, grid> {
    using type = grid;
};

/**
    Set the element at `pos(x, y)` in a grid to `value`.
*/
template <typename origin, typename other, typename grid>
struct draw_grid;

template <typename origin, typename otherRows, typename g>
struct draw_grid<origin, Grid<otherRows>, g> {
    using type = typename draw_row<
        origin,
        car_t<otherRows>,
        typename draw_grid<
            typename origin::template add<Position<0, 1>>,
            Grid<cdr_t<otherRows>>,
            g>::type>::type;
};

template <typename origin, typename g>
struct draw_grid<origin, Grid<List<>>, g> {
    using type = g;
};

/**
    Draw a line.
*/
template <typename origin, Orientation orientation, size_t len, typename gfx, typename buffer>
using buffer_draw_line =
    typename draw_grid<
        origin,
        std::conditional_t<orientation == Orientation::Vertical,
            gen_grid<1, len, Pixel<' ', gfx>>,
            gen_grid<len, 1, Pixel<' ', gfx>>>,
        buffer>::type;

/**
    Draw a empty box.
*/
template <typename origin, size_t width, size_t height, typename gfx, typename buffer>
using buffer_draw_rect_outline =
    buffer_draw_line<origin, Orientation::Horizontal, width, gfx,
        buffer_draw_line<origin, Orientation::Vertical, height, gfx,
            buffer_draw_line<typename origin::template add<Position<0, height - 1>>, Orientation::Horizontal, width, gfx,
                buffer_draw_line<typename origin::template add<Position<width - 1, 0>>, Orientation::Vertical, height, gfx, buffer>>>>;
