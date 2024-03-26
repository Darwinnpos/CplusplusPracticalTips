#pragma once
#include <cstdint>


/// 枚举的使用技巧
// 这种写法可以用或来组合多种组合
enum options : std::uint32_t {
    binary = 1u << 0
    , text = 1u << 1
    , json = 1u << 2
    , no_header = 1u << 3
    , elittle = 1u << 4
    , ebig = 1u << 5
    , ehost = 1u << 6
    , compacted = 1u << 7
    , mem = 1u << 8
    , file = 1u << 9
};

