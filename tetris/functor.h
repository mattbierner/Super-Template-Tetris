#pragma once

/**
    Interface for a type that can be fmapped.
*/
template <typename f, typename x>
struct FMap {
    using type = call<f, x>;
};

template <typename f, typename x>
using fmap = typename FMap<f, x>::type;
