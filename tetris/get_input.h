#pragma once

#include "input.h"

static constexpr const Input input =
#if defined(HARD)
    Input::Hard;
#elif defined(SOFT)
    Input::Soft;
#elif defined(DOWN)
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
