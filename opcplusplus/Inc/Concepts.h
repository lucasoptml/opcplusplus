
#pragma once

#include <concepts>

template<class T>
concept NodeType = std::derived_from<T, nodes::opNode>;

