/**
    Should the compile time `ToString` be used in place of the runtime `Print` for
    printing large objects (such as lists and grids)?
    
    `ToString` is slower than `Print`, so set this to false if printing
    multiple steps of a game in a single compile.
*/
#define USE_GAME_TO_STRING 0

/**
    Should C++17 fold expressions be used when possible?
 
    Fold expressions should compile faster than recursion.
*/
#define USE_FOLD_EXPRESSIONS 0

#include <iostream>
#include <fstream>

#include "list.h"
#include "game.h"

/**
    Save the game state.
*/
template <typename state>
void serialize_game()
{
    std::ofstream s;
    s.open("current_game.h");
    s << "#import \"game.h\"\n";
    s << "using state = ";
    Printer<serialize_to_string<state>>::Print(s);
    s << ";";
    s.close();
}

/**
    Play a game for a given set of input commands.
    
    Outputs a list of game states.
*/
template <typename s, Input... inputs>
struct Play {
    using type = List<s>;
};

template <typename s, Input... inputs>
using play = typename Play<s, inputs...>::type;

template <typename s, Input x, Input... xs>
struct Play<s, x, xs...> {
    using type = cons<s, play<step_t<x, s>, xs...>>;
};

/**
    Print out the result of the game.
*/
int main(int argc, const char* argv[])
{
#include "current_game.h"

#include "get_input.h"

   /*using game = play<initialState,
        Input::Down, Input::LRot, Input::Left, Input::Left, Input::Drop,
        Input::LRot, Input::Right, Input::Right, Input::Drop>;
*/
    using game = step_t<input, state>;
    
    Printer<game>::Print(std::cout);
    serialize_game<game>();

    return 0;
}



