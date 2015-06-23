#pragma once

#include "blocks.h"
#include "random.h"
#include "serialize.h"

/**
    Random bag of blocks.
*/
template <typename rand>
struct BlockGenerator {
    using next = BlockGenerator<typename rand::next>;
    using value = get<rand::value, blocks>;
};

using initialBlockGenerator =
    BlockGenerator<
        Random<blocks::size>>;

/*------------------------------------------------------------------------------
    Serialize
*/
template <typename rand>
struct Serialize<BlockGenerator<rand>> {
    using type =
        serialize_class<decltype("BlockGenerator"_string),
            rand>;
};