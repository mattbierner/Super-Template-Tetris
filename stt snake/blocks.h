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

/**
    Single tetris piece.
*/
template <BlockType k, size_t r, typename o>
struct Block {
    static constexpr const BlockType kind = k;
    
    using orientations = o;
    using pieces = get_t<r, o>;
    
    using rotateCw = Block<k, (r + 1) % o::size, o>;
    using rotateCcw = Block<k, r == 0 ? o::size - 1 : r - 1, o>;
    
    struct ToGhostPiece {
        template <typename x>
        using apply =
            std::conditional<std::is_same<x, empty_pixel>::value,
                empty_pixel,
                Pixel<'~', default_gfx>>;
    };
    
    /**
        Get a ghost piece to show where a block will land.
    */
    using ghostPiece = fmap_t<ToGhostPiece, pieces>;
};

using x_cell = empty_pixel;
using i_cell = Pixel<' ', default_gfx::setBg<Color::Cyan>>;
using j_cell = Pixel<' ', default_gfx::setBg<Color::Blue>>;
using l_cell = Pixel<' ', default_gfx::setBg<Color::White>>;
using o_cell = Pixel<' ', default_gfx::setBg<Color::Yellow>>;
using s_cell = Pixel<' ', default_gfx::setBg<Color::Green>>;
using t_cell = Pixel<' ', default_gfx::setBg<Color::Magenta>>;
using z_cell = Pixel<' ', default_gfx::setBg<Color::Red>>;

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
    List<
        gen_grid<2, 2, o_cell>>>;

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

/**
    List of all blocks.
*/
using blocks = List<IBlock, JBlock, LBlock, OBlock, SBlock, TBlock, ZBlock>;
