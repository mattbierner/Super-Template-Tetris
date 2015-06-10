#pragma once

#include "input.h"

#if defined(UP)
constexpr const Input input = Input::Up;
#elif defined(RIGHT)
constexpr const Input input = Input::Right;
#elif defined(DOWN)
constexpr const Input input = Input::Down;
#elif defined(LEFT)
constexpr const Input input = Input::Left;
#else
constexpr const Input input = Input::None;
#endif