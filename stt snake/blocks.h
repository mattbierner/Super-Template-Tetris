#pragma once

#include "list.h"
#include "grid.h"
#include "pixel.h"

/**
    Type of block.
*/
enum class BlockType
{
    None,
    I,
    J,
    L,
    O,
    S,
    T,
    Z
};

template <BlockType k, size_t r, typename o>
struct Block {
    static const BlockType kind = k;
    using orientations = o;
    
    using pieces = get_t<r, o>;
    
    using rotateCw = Block<k, (r + 1) % o::size, o>;
    using rotateCCw = Block<k, r == 0 ? o::size - 1 : r - 1, o>;
};


template <BlockType k>
struct Cell {
    static const BlockType kind = k;
};

using x_cell = Cell<BlockType::None>;
using i_cell = Cell<BlockType::I>;
using j_cell = Cell<BlockType::J>;
using l_cell = Cell<BlockType::L>;
using o_cell = Cell<BlockType::O>;
using s_cell = Cell<BlockType::S>;
using t_cell = Cell<BlockType::T>;
using z_cell = Cell<BlockType::Z>;

/**
*/
template <typename x>
struct to_px;

template <>
struct to_px<Cell<BlockType::None>> {
    using type = empty_pixel;
};

template <>
struct to_px<Cell<BlockType::I>> {
    using type = Pixel<' ', default_gfx::setBg<Color::Cyan>>;
};

template <>
struct to_px<Cell<BlockType::J>> {
    using type = Pixel<' ', default_gfx::setBg<Color::Blue>>;
};

template <>
struct to_px<Cell<BlockType::L>> {
    using type = Pixel<' ', default_gfx::setBg<Color::White>>;
};

template <>
struct to_px<Cell<BlockType::O>> {
    using type = Pixel<' ', default_gfx::setBg<Color::Yellow>>;
};

template <>
struct to_px<Cell<BlockType::S>> {
    using type = Pixel<' ', default_gfx::setBg<Color::Green>>;
};

template <>
struct to_px<Cell<BlockType::T>> {
    using type = Pixel<' ', default_gfx::setBg<Color::Magenta>>;
};

template <>
struct to_px<Cell<BlockType::Z>> {
    using type = Pixel<' ', default_gfx::setBg<Color::Red>>;
};

/**
    Blocks.
*/
using IBlock = Block<BlockType::I, 0,
    List<
        Grid<List<
            List<x_cell, x_cell, x_cell, x_cell>,
            List<i_cell, i_cell, i_cell, i_cell>,
            List<x_cell, x_cell, x_cell, x_cell>,
            List<x_cell, x_cell, x_cell, x_cell>>>,
        Grid<List<
            List<x_cell, x_cell, i_cell, x_cell>,
            List<x_cell, x_cell, i_cell, x_cell>,
            List<x_cell, x_cell, i_cell, x_cell>,
            List<x_cell, x_cell, i_cell, x_cell>>>,
        Grid<List<
            List<x_cell, x_cell, x_cell, x_cell>,
            List<x_cell, x_cell, x_cell, x_cell>,
            List<i_cell, i_cell, i_cell, i_cell>,
            List<x_cell, x_cell, x_cell, x_cell>>>,
        Grid<List<
            List<x_cell, i_cell, x_cell, x_cell>,
            List<x_cell, i_cell, x_cell, x_cell>,
            List<x_cell, i_cell, x_cell, x_cell>,
            List<x_cell, i_cell, x_cell, x_cell>>>>>;

using JBlock = Block<BlockType::J, 0,
    List<
        Grid<List<
            List<j_cell, x_cell, x_cell>,
            List<j_cell, j_cell, j_cell>,
            List<x_cell, x_cell, x_cell>>>,
        Grid<List<
            List<x_cell, j_cell, j_cell>,
            List<x_cell, j_cell, x_cell>,
            List<x_cell, j_cell, x_cell>>>,
        Grid<List<
            List<x_cell, x_cell, x_cell>,
            List<j_cell, j_cell, j_cell>,
            List<x_cell, x_cell, j_cell>>>,
        Grid<List<
            List<x_cell, j_cell, x_cell>,
            List<x_cell, j_cell, x_cell>,
            List<j_cell, j_cell, x_cell>>>>>;

using LBlock = Block<BlockType::L, 0,
    List<
        Grid<List<
            List<x_cell, x_cell, l_cell>,
            List<l_cell, l_cell, l_cell>,
            List<x_cell, x_cell, x_cell>>>,
        Grid<List<
            List<x_cell, l_cell, x_cell>,
            List<x_cell, l_cell, x_cell>,
            List<x_cell, l_cell, l_cell>>>,
        Grid<List<
            List<x_cell, x_cell, x_cell>,
            List<l_cell, l_cell, l_cell>,
            List<l_cell, x_cell, x_cell>>>,
        Grid<List<
            List<l_cell, l_cell, x_cell>,
            List<x_cell, l_cell, x_cell>,
            List<x_cell, l_cell, x_cell>>>>>;

using OBlock = Block<BlockType::O, 0,
    gen_grid<2, 2, o_cell>>;

using SBlock = Block<BlockType::S, 0,
    List<
        Grid<List<
            List<x_cell, s_cell, s_cell>,
            List<s_cell, s_cell, x_cell>,
            List<x_cell, x_cell, x_cell>>>,
        Grid<List<
            List<x_cell, s_cell, x_cell>,
            List<x_cell, s_cell, s_cell>,
            List<x_cell, x_cell, s_cell>>>,
        Grid<List<
            List<x_cell, x_cell, x_cell>,
            List<x_cell, s_cell, s_cell>,
            List<s_cell, s_cell, x_cell>>>,
        Grid<List<
            List<s_cell, x_cell, x_cell>,
            List<s_cell, s_cell, x_cell>,
            List<x_cell, s_cell, x_cell>>>>>;

using TBlock = Block<BlockType::T, 0,
    List<
        Grid<List<
            List<x_cell, t_cell, x_cell>,
            List<t_cell, t_cell, t_cell>,
            List<x_cell, x_cell, x_cell>>>,
        Grid<List<
            List<x_cell, t_cell, x_cell>,
            List<x_cell, t_cell, t_cell>,
            List<x_cell, t_cell, x_cell>>>,
        Grid<List<
            List<x_cell, x_cell, x_cell>,
            List<t_cell, t_cell, t_cell>,
            List<x_cell, t_cell, x_cell>>>,
        Grid<List<
            List<x_cell, t_cell, x_cell>,
            List<t_cell, t_cell, x_cell>,
            List<x_cell, t_cell, x_cell>>>>>;

using ZBlock = Block<BlockType::Z, 0,
    List<
        Grid<List<
            List<z_cell, z_cell, x_cell>,
            List<x_cell, z_cell, z_cell>,
            List<x_cell, x_cell, x_cell>>>,
         Grid<List<
            List<x_cell, x_cell, z_cell>,
            List<x_cell, z_cell, z_cell>,
            List<x_cell, z_cell, x_cell>>>,
         Grid<List<
            List<x_cell, x_cell, x_cell>,
            List<z_cell, z_cell, x_cell>,
            List<x_cell, z_cell, z_cell>>>,
         Grid<List<
            List<x_cell, z_cell, x_cell>,
            List<z_cell, z_cell, x_cell>,
            List<z_cell, x_cell, x_cell>>>>>;


