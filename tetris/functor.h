#pragma once

/**
    Interface for a type that can be fmapped.
*/
template <typename f, typename x>
struct Fmap {
    using type = call<f, x>;
};

template <typename f, typename x>
using f_map = typename Fmap<f, x>::type;
