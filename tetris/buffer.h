#pragma once

#include "grid.h"
#include "pixel.h"
#include "string.h"

/**
    Create an empty buffer.
*/
template <size_t width, size_t height>
using empty_buffer = gen_grid<width, height, empty_pixel>;


/**
    Draw another buffer on top of this buffer.
*/
struct BufferCombine {
    template <typename current, typename toPlace>
    using apply =
        std::conditional<is_empty<toPlace>,
            current,
            toPlace>;
};

template <typename origin, typename other, typename grid>
using buffer_draw_grid = grid_place_grid<BufferCombine, origin, other, grid>;

/**
    Draw a line of `px` repeated `len` times.
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
template <
    typename origin,
    Orientation orientation,
    size_t max,
    typename str,
    typename gfx,
    typename buffer,
    typename trimmedStr = typename StringTake<max, str>::type>
using buffer_draw_centered_text =
    buffer_draw_text<
        typename origin::template add<create_offset<orientation, (max - trimmedStr::size) / 2>>,
        orientation,
        trimmedStr,
        gfx,
        buffer>;

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
    Draw an empty box.
*/
template <typename origin, typename size, typename px, typename buffer>
using buffer_draw_rect_outline =
    buffer_draw_line<origin, Orientation::Horizontal, size::width, px,
        buffer_draw_line<origin, Orientation::Vertical, size::height, px,
            buffer_draw_line<typename origin::template add<Position<0, size::height - 1>>, Orientation::Horizontal, size::width, px,
                buffer_draw_line<typename origin::template add<Position<size::width - 1, 0>>, Orientation::Vertical, size::height, px, buffer>>>>;
