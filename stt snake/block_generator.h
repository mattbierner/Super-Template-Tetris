#pragma once

#include "blocks.h"
#include "printer.h"
#include "random.h"

template <typename rand>
struct BlockGenerator {
    using next = BlockGenerator<typename rand::next>;
    using value = get_t<rand::value, blocks>;
};

using initialBlockGenerator = BlockGenerator<
    Random<blocks::size>>;

/*------------------------------------------------------------------------------
    Printer
*/
template <typename rand>
struct Printer<BlockGenerator<rand>>
{
    static void Print(std::ostream& output)
    {
        Printer<rand>::Print(output);
    }
};