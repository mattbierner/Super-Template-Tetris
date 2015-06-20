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
struct BufferDrawText<origin, orientation, String<chars...>, gfx, buffer> {
    using type =
        buffer_draw_grid<
            origin,
            create_list_grid<orientation, List<Pixel<chars, gfx>...>>,
            buffer>;
};

template <typename origin, Orientation orientation, typename str, typename gfx, typename buffer>
using buffer_draw_text = typename BufferDrawText<origin, orientation, str, gfx, buffer>::type;

/**
    Draw centered text.
*/
template <typename origin, Orientation orientation, size_t max, typename str, typename gfx, typename buffer>
struct BufferDrawCenteredText;

template <typename origin, Orientation orientation, size_t max, typename gfx, typename buffer, char... chars>
struct BufferDrawCenteredText<origin, orientation, max, String<chars...>, gfx, buffer> {
    using str = typename StringTake<max, String<chars...>>::type;

    using type =
        buffer_draw_text<
            typename origin::template add<create_offset<orientation, (max - str::size) / 2>>,
            orientation,
            str,
            gfx,
            buffer>;
};

template <typename origin, Orientation orientation, size_t max, typename str, typename gfx, typename buffer>
using buffer_draw_centered_text = typename BufferDrawCenteredText<origin, orientation, max, str, gfx, buffer>::type;


/**
    Draw a filled box.
*/
template <typename origin, typename size, typename px, typename buffer>
using buffer_draw_rect =
    buffer_draw_grid<
        origin,
        gen_grid<size::width, size::height, px>,
        buffer>;


/**
    Draw a empty box.
*/
template <typename origin, typename size, typename px, typename buffer>
using buffer_draw_rect_outline =
    buffer_draw_line<origin, Orientation::Horizontal, size::width, px,
        buffer_draw_line<origin, Orientation::Vertical, size::height, px,
            buffer_draw_line<typename origin::template add<Position<0, size::height - 1>>, Orientation::Horizontal, size::width, px,
                buffer_draw_line<typename origin::template add<Position<size::width - 1, 0>>, Orientation::Vertical, size::height, px, buffer>>>>;
