/**
    Basic specilizable interface for a type that can be fmapped.
*/
#pragma once

template <typename x, template<typename> class f>
struct Fmap;

template <typename x, template<typename> class f>
using fmap_t = typename Fmap<x, f>::type;
