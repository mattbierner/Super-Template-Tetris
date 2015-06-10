#pragma once

/**
    Single player input command.
*/
enum class Input : unsigned
{
    None,
    Up,
    Down,
    Left,
    Right
};

/**
    List of sequential input commands.
*/
template <Input... inputs>
using PlayerInput = std::integer_sequence<Input, inputs...>;
