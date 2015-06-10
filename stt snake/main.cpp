#include <iostream>
#include <sstream>
#include <fstream>

//#include "cell.h"
//#include "direction.h"
#include "list.h"
#include "buffer.h"
//#include "snake.h"

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

    using game = gen_grid<6, 6, Pixel<'a', Color::Red>>;

    using block = gen_grid<2, 2, Pixel<'x', Color::Green>>;
    using block2 = gen_grid<1, 3, Pixel<'x', Color::Magenta, Color::Blue>>;

   /* using g2 = typename draw_grid<
        Position<1, 0>,
        block2,
        typename draw_grid<
            Position<1, 1>,
            block,
            game>::type>::type;*/
    
    using g2 = buffer_draw_line<
        Position<1, 0>,
        Orientation::Vertical,
        3,
        game>;

    Printer<g2>::Print(std::cout);
    return 0;
}

