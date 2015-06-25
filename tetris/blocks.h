#pragma once

#include "list.h"
#include "grid.h"
#include "pixel.h"
#include "serialize.h"


/**
    Single tetris piece.
*/
template <size_t r, typename o>
struct Block {
    using orientations = o;
    using pieces = get<r, o>;
    
    using rotateCw = Block<(r + 1) % o::size, o>;
    using rotateCcw = Block<r == 0 ? o::size - 1 : r - 1, o>;
    
    struct ToGhostPiece {
        template <typename x>
        using apply =
            std::conditional<is_empty<x>,
                empty_pixel,
                Pixel<'~', default_gfx>>;
    };
    
    /**
        Get a ghost piece to show where a block will land.
    */
    using as_ghost_piece = fmap<ToGhostPiece, pieces>;
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
using IBlock = Block<0,
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

using JBlock = Block<0,
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

using LBlock = Block<0,
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

using OBlock = Block<0,
    List<
        gen_grid<2, 2, o_cell>>>;

using SBlock = Block<0,
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

using TBlock = Block<0,
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

using ZBlock = Block<0,
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

/*------------------------------------------------------------------------------
    Serialize
*/
template <size_t r, typename o>
struct Serialize<Block<r, o>> {
    using type =
        serialize_class<decltype("Block"_string),
            SerializableValue<size_t, r>,
            o>;
};
