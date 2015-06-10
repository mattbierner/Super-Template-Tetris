#pragma once

#include "list.h"
#include "functor.h"
#include "printer.h"
#include "position.h"
#include "utility.h"

enum class Color
{
    Default,
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White
};

static const char* colorReset =  "\x1b[0m";

const char* toFgCode(Color color)
{
    switch (color)
    {
    case Color::Black:      return "\x1b[30m";
    case Color::Red:        return "\x1b[31m";
    case Color::Green:      return "\x1b[32m";
    case Color::Yellow:     return "\x1b[33m";
    case Color::Blue:       return "\x1b[34m";
    case Color::Magenta:    return "\x1b[35m";
    case Color::Cyan:       return "\x1b[36m";
    case Color::White:      return "\x1b[37m";
    default:                return colorReset;
    }
};

/**
 
*/
template <char val, Color fg = Color::Default, Color bg = Color::Default>
struct Cell {
    static const char value = val;
};

/**
    Two dimensional grid of values.
    
    Stores a list of column lists that store cells.
*/
template <typename r>
struct Buffer {
    using rows = r;
    
    static const size_t width = get_t<0, r>::size;
    static const size_t height = rows::size;
};

/**
    Generate an width x height grid of `value`.
*/
template <size_t width, size_t height, typename value>
using gen_buffer = Buffer<gen_t<height, gen_t<width, value>>>;

/**
    Get the element at `pos(x, y)` in a grid.
*/
template <typename pos, typename grid>
using get_grid = get_t<pos::x, get_t<pos::y, typename grid::rows>>;


/**
    Set the element at `pos(x, y)` in a grid to `value`.
*/
template <typename pos, typename value, typename grid>
using put_grid = Buffer<
    put_t<
        pos::y,
        put_t<pos::x, value, get_t<pos::y, typename grid::rows>>,
        typename grid::rows>>;

/**
    Set the element at `pos(x, y)` in a grid to `value`.
*/
template <typename pos, typename cell, typename grid>
using try_put_grid =
    std::conditional_t<cell::value == ' ',
        grid,
        put_grid<pos, cell, grid>>;


template <typename origin>
struct Putter
{
    template <typename grid, typename element>
    using apply = identity<try_put_grid<origin, element, grid>>;
};

/**
    Set the element at `pos(x, y)` in a grid to `value`.
*/
template <typename origin, typename row, typename grid>
struct draw_row {
    using type = try_put_grid<
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

template <typename origin, typename otherRows, typename buffer>
struct draw_grid<origin, Buffer<otherRows>, buffer> {
    using type = typename draw_row<
        origin,
        car_t<otherRows>,
        typename draw_grid<
            typename origin::template add<Position<0, 1>>,
            Buffer<cdr_t<otherRows>>,
            buffer>::type>::type;
};

template <typename origin, typename buffer>
struct draw_grid<origin, Buffer<List<>>, buffer> {
    using type = buffer;
};


/*------------------------------------------------------------------------------
    Printer
*/
template <char val, Color fg, Color bg>
struct Printer<Cell<val, fg, bg>>
{
    static void Print(std::ostream& output)
    {
        output << toFgCode(fg) << val;
    }
};


/*------------------------------------------------------------------------------
    Printer
*/
template <>
struct Printer<Buffer<List<>>>
{
    static void Print(std::ostream& output) { /* noop */ }
};

template <typename x, typename... xs>
struct Printer<Buffer<List<x, xs...>>>
{
    static void Print(std::ostream& output)
    {
        Printer<x>::Print(output);
        output << colorReset << "\n";
        Printer<Buffer<List<xs...>>>::Print(output);
    }
};

/*------------------------------------------------------------------------------
    Foldable
*/
template <typename f, typename z, typename rows>
struct Foldable<f, z, Buffer<rows>> {
    using type = fold<
        partial<mfunc<fold>, f>,
        z,
        rows>;
};

/*------------------------------------------------------------------------------
    Functor
*/
template <typename rows, template<typename> class f>
struct Fmap<Buffer<rows>, f> {

    template <typename val>
    struct fmap_inner {
        using type = fmap_t<val, f>;
    };
    
    using type = Buffer<
        fmap_t<rows, fmap_inner>>;
};

/*------------------------------------------------------------------------------
    Serialize
*/
template <typename rows>
struct Serialize<Buffer<rows>>
{
    static std::ostream& Write(std::ostream& output)
    {
        output << "Buffer<";
        Serialize<rows>::Write(output);
        return output << ">";
    }
};

template <size_t x, size_t y>
struct Serialize<Position<x, y>>
{
    static std::ostream& Write(std::ostream& output)
    {
        output << "Position<" << x << "," << y << ">";
        return output;
    }
};
