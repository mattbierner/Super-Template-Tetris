#pragma once

/**
    Player input command.
*/
enum class Input : unsigned {
    None,
    Hard, // Hard Drop
    Soft, // Soft Drop
    Down, // Soft, move block down by 4
    Left,
    Right,
    LRot,
    RRot
};

