#pragma once

#include "direction.h"
#include "grid.h"
#include "pixel.h"

/**
    Creaete an empty buffer.
*/
template <size_t width, size_t height>
using empty_buffer = gen_grid<width, height, empty_pixel>;

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
        car<row>,
        typename draw_row<
            typename origin::template add<Position<1, 0>>,
            cdr<row>,
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
struct DrawGrid;

template <typename origin, typename other, typename grid>
using buffer_draw_grid = typename DrawGrid<origin, other, grid>::type;

template <typename origin, typename otherRows, typename g>
struct DrawGrid<origin, Grid<otherRows>, g> {
    using type = typename draw_row<
        origin,
        car<otherRows>,
        typename DrawGrid<
            typename origin::template add<Position<0, 1>>,
            Grid<cdr<otherRows>>,
            g>::type>::type;
};

template <typename origin, typename g>
struct DrawGrid<origin, Grid<List<>>, g> {
    using type = g;
};

/**
    Draw a line.
*/
template <typename origin, Orientation orientation, size_t len, char px, typename gfx, typename buffer>
using buffer_draw_line =
    buffer_draw_grid<
        origin,
        std::conditional_t<orientation == Orientation::Vertical,
            gen_grid<1, len, Pixel<px, gfx>>,
            gen_grid<len, 1, Pixel<px, gfx>>>,
        buffer>;


/**
    Draw a filled box.
*/
template <typename origin, size_t width, size_t height, char px, typename gfx, typename buffer>
using buffer_draw_rect =
    buffer_draw_grid<origin, gen_grid<width, height, Pixel<px, gfx>>, buffer>;


/**
    Draw a empty box.
*/
template <typename origin, size_t width, size_t height, char px, typename gfx, typename buffer>
using buffer_draw_rect_outline =
    buffer_draw_line<origin, Orientation::Horizontal, width, px, gfx,
        buffer_draw_line<origin, Orientation::Vertical, height, px, gfx,
            buffer_draw_line<typename origin::template add<Position<0, height - 1>>, Orientation::Horizontal, width, px, gfx,
                buffer_draw_line<typename origin::template add<Position<width - 1, 0>>, Orientation::Vertical, height, px, gfx, buffer>>>>;
