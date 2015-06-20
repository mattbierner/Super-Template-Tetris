#pragma once

#include "blocks.h"
#include "random.h"
#include "string.h"
#include "serialize.h"

template <typename rand>
struct BlockGenerator {
    using next = BlockGenerator<typename rand::next>;
    using value = get_t<rand::value, blocks>;
};

using initialBlockGenerator =
    BlockGenerator<
        Random<blocks::size>>;

/*------------------------------------------------------------------------------
    SerializeToString
*/
template <typename rand>
struct SerializeToString<BlockGenerator<rand>>
{
    using type =
        serialize_class_to_string<decltype("BlockGenerator"_string),
            rand>;
};