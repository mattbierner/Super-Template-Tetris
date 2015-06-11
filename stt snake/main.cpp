#include <iostream>
#include <sstream>
#include <fstream>

//#include "cell.h"
//#include "direction.h"
#include "list.h"
#include "buffer.h"
//#include "snake.h"
#include "blocks.h"

/**
    Save the output state
*/
template <typename state>
void serialize_game()
{
    std::ofstream s;
    s.open("current_game.h");
    s << "#import \"snake.h\"\n";
    
    s << "using state = ";
    Serialize<state>::Write(s);
    s << ";";
    s.close();
}

/**
    Print out the result of the game.
*/
int main(int argc, const char* argv[])
{
/*#include "current_game.h"

#include "get_input.h"
    
    // on first turn, don't consume any input but print the game world.
#if defined(FIRSTTURN)
    using game = state;
#else
    using game = step_t<input, state>;
#endif

    Printer<game>::Print(std::cout);
    
    serialize_game<game>();
*/

    using game = gen_grid<6, 6, Pixel<'a', default_gfx::setFg<Color::Red>>>;

    using block = gen_grid<2, 2, Pixel<'x', default_gfx::setFg<Color::Green>>>;
    using block2 = gen_grid<1, 3, Pixel<'x', default_gfx::setFg<Color::Magenta>::setBg<Color::Blue>>>;

   /* using g2 = typename draw_grid<
        Position<1, 0>,
        block2,
        typename draw_grid<
            Position<1, 1>,
            block,
            game>::type>::type;*/
    
    struct xxxx {
        template <typename x>
        using apply = identity<typename to_px<x>::type>;
    };
    
    using x = fmap_t<xxxx, SBlock::rotateCw::rotateCw::rotateCw::pieces>;
    
    using g2 = buffer_draw_rect_outline<
        Position<1, 1>,
        4,
        4,
        default_gfx::setBg<Color::Blue>,
        game>;


    Printer<x>::Print(std::cout);
    return 0;
}

