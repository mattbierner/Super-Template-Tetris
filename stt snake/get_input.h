#pragma once

#include "input.h"

static constexpr const Input input =
#if defined(DROP)
    Input::Drop;
#elif defined(DOW)
    Input::Down;
#elif defined(LEFT)
    Input::Left;
#elif defined(RIGHT)
    Input::Right;
#elif defined(LROT)
    Input::LRot;
#elif defined(RROT)
    Input::RRot;
#else
    Input::None;
#endif
