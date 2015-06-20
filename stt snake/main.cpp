#include <iostream>
#include <sstream>
#include <fstream>

//#include "cell.h"
//#include "direction.h"
#include "list.h"
#include "buffer.h"
#include "playfield.h"
#include "blocks.h"
#include "block_generator.h"
#include "game.h"
 
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

template <typename s, Input... inputs>
struct Play {
    using type = List<s>;
};

template <typename s, Input x, Input... xs>
struct Play<s, x, xs...> {
    using type = cons<s, typename Play<step_t<x, s>, xs...>::type>;
};

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

    using game = gen_grid<6, 6, empty_pixel>;

    using block = gen_grid<2, 2, Pixel<'x', default_gfx::setFg<Color::Green>>>;
    using block2 = gen_grid<1, 3, Pixel<'x', default_gfx::setFg<Color::Magenta>::setBg<Color::Blue>>>;

   using g2 = //buffer_draw_grid<
       // Position<1, 0>,
        //block2,
        buffer_draw_grid<
            Position<1, 1>,
            block,
            game>;
    
    struct xx {
        template <typename x>
        using apply = identity<std::integral_constant<bool, x::value != '\0'>>;
    };
    
   // using g = buffer_draw_grid<Position<4, 1>, typename OBlock::pieces, game>;

    //Printer<g>::Print(std::cout);


    Printer<typename Play<InitialState,
        Input::LRot, Input::Left, Input::Left, Input::Left, Input::Left, Input::Left, Input::Up,
        Input::Left, Input::Left>::type>::Print(std::cout);

    return 0;
}

