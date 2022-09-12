#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <inttypes.h>
#include <vector>
#include <format>

#include <glm/glm.hpp>

#include "util/log.hpp"

#define BIT(x) (1 << x)

template<typename T>
using scope = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr scope<T> CreateScope(Args&& ... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr ref<T> CreateRef(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}