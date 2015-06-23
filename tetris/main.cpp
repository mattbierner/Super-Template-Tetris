#include <iostream>
#include <fstream>

#include "list.h"
#include "string.h"
#include "game.h"

/**
    RUNTIME - Print out a string.
*/
template <char... elements>
std::ostream& print(std::ostream& output, String<elements...>)
{
    bool Do[] = { true, (output << elements, true)... };
    (void)Do;
    return output;
}

/**
    RUNTIME - Save the game state.
*/
template <typename state>
void serialize_game()
{
    std::ofstream s;
    s.open("current_game.h");
    s << "#import \"game.h\"\n";
    s << "using state = ";
    print(s, serialize<state>{});
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
    Although the entire game is 'played' at compile time, the program must be run
    to display the current game state and serialize it for the next run.
    
    This file contains the only pieces of runtime bullshit required.
*/
int main(int argc, const char* argv[])
{
#include "current_game.h"
#include "get_input.h"

/*
   using game = play<initialState,
        Input::Down, Input::LRot, Input::Left, Input::Left, Input::Hard,
        Input::LRot, Input::Right, Input::Right, Input::Hard>;
*/

    using game = step_t<input, state>;
    print(std::cout, to_string<game>{}) << "\n";
    serialize_game<game>();

    return 0;
}
