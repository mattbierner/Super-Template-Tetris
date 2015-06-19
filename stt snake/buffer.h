#pragma once

#include "direction.h"
#include "grid.h"
#include "pixel.h"
#include "string.h"

/**
    Create an empty buffer.
*/
template <size_t width, size_t height>
using empty_buffer = gen_grid<width, height, empty_pixel>;

/**
    Set the element at `pos(x, y)` in a grid to `value`.
*/
template <typename pos, typename cell, typename g>
using buffer_try_put =
    std::conditional_t<cell::value == '\0',
        g,
        grid_put<pos, cell, g>>;

/**
    Set the element at `pos(x, y)` in a grid to `value`.
*/
template <typename origin, typename rows, typename grid>
struct DrawRow {
    using type = buffer_try_put<
        origin,
        car<rows>,
        typename DrawRow<
            typename origin::template add<Position<1, 0>>,
            cdr<rows>,
            grid>::type>;
};

template <typename origin, typename grid>
struct DrawRow<origin, List<>, grid> {
    using type = grid;
};

template <typename origin, typename row, typename grid>
using draw_row = typename DrawRow<origin, row, grid>::type;

/**
    Draw another buffer on top of this buffer.
*/
template <typename origin, typename other, typename grid>
struct DrawGrid;

template <typename origin, typename other, typename grid>
using buffer_draw_grid = typename DrawGrid<origin, other, grid>::type;

template <typename origin, typename otherRows, typename g>
struct DrawGrid<origin, Grid<otherRows>, g> {
    using type = draw_row<
        origin,
        car<otherRows>,
        typename DrawGrid<
            typename origin::template add<Position<0, 1>>,
            Grid<cdr<otherRows>>,
            g>::type>;
};

template <typename origin, typename g>
struct DrawGrid<origin, Grid<List<>>, g> {
    using type = g;
};

/**
    Draw a line.
*/
template <typename origin, Orientation orientation, size_t len, typename px, typename buffer>
using buffer_draw_line =
    buffer_draw_grid<
        origin,
        create_line_grid<orientation, len, px>,
        buffer>;

/**
    Draw some text.
*/
template <typename origin, Orientation orientation, typename str, typename gfx, typename buffer>
struct BufferDrawText;

template <typename origin, Orientation orientation, typename gfx, typename buffer, char... chars>
struct BufferDrawText<origin, orientation, string<chars...>, gfx, buffer> {
    using type = buffer_draw_grid<
        origin,
        create_list_grid<orientation, List<Pixel<chars, gfx>...>>,
        buffer>;
};

template <typename origin, Orientation orientation, typename str, typename gfx, typename buffer>
using buffer_draw_text = typename BufferDrawText<origin, orientation, str, gfx, buffer>::type;

/**
    Draw a filled box.
*/
template <typename origin, size_t width, size_t height, typename px, typename buffer>
using buffer_draw_rect =
    buffer_draw_grid<origin, gen_grid<width, height, px>, buffer>;


/**
    Draw a empty box.
*/
template <typename origin, size_t width, size_t height, typename px, typename buffer>
using buffer_draw_rect_outline =
    buffer_draw_line<origin, Orientation::Horizontal, width, px,
        buffer_draw_line<origin, Orientation::Vertical, height, px,
            buffer_draw_line<typename origin::template add<Position<0, height - 1>>, Orientation::Horizontal, width, px,
                buffer_draw_line<typename origin::template add<Position<width - 1, 0>>, Orientation::Vertical, height, px, buffer>>>>;
