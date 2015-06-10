/**
    Basic interface for a type that can be fmapped.
*/
#pragma once

template <typename f, typename x>
struct Fmap {
    using type = call<f, x>;
};

template <typename f, typename x>
using fmap_t = typename Fmap<f, x>::type;
