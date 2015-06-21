#pragma once

/**
    Player input command.
*/
enum class Input : unsigned {
    None,
    Drop, // Hard Drop
    Down, // Soft, move block down by 4
    Left,
    Right,
    LRot,
    RRot
};

