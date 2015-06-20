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
   /** std::ofstream s;
    s.open("current_game.h");
    s << "#import \"snake.h\"\n";
    return Serialize<typename SerializeToString<s>::type>::Write(output);

    s << "using state = ";*/
    Serialize<serialize_to_string<car<state>>>::Write(std::cout);
/*    s << ";";
    s.close();*/
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
*/
    using game = typename Play<InitialState,
        Input::LRot, Input::Left, Input::Left, Input::Drop,
        Input::LRot, Input::Right, Input::Right, Input::Drop>::type;

    serialize_game<game>();

    Printer<game>::Print(std::cout);

    return 0;
}



