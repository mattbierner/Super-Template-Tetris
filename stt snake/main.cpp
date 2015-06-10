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

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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

    using game = gen_buffer<6, 6, Cell<'a', Color::Red>>;

    using block = gen_buffer<2, 2, Cell<'x', Color::Green>>;

    using g2 = typename draw_grid<Position<1, 1>, block, game>::type;

    Printer<g2>::Print(std::cout);

    return 0;
}

