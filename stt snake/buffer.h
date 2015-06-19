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
    std::conditional_t<std::is_same<cell, empty_pixel>::value,
        g,
        grid_put<pos, cell, g>>;

/**
    Set the element at `pos(x, y)` in a grid to `value`.
*/
struct buffer_combine {
    template <typename current, typename toPlace>
    using apply =
        std::conditional<std::is_same<toPlace, empty_pixel>::value,
            current,
            toPlace>;
};

template <typename origin, typename row, typename grid>
using draw_row = grid_place_row<buffer_combine, origin, row, grid>;

/**
    Draw another buffer on top of this buffer.
*/
template <typename origin, typename other, typename grid>
using buffer_draw_grid = grid_place_grid<buffer_combine, origin, other, grid>;

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
