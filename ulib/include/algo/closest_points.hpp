#pragma once

#include <vector>
#include <utility>

namespace algo {

/// @brief usual 2d distance
double EuclideanDist(std::pair<int, int> lhs, std::pair<int, int> rhs);

namespace slow {

std::pair<size_t, size_t> ClosestEuclideanPair(
    const std::vector<std::pair<int, int>>& pts);

}  // namespace slow

std::pair<size_t, size_t> ClosestEuclideanPair(
    const std::vector<std::pair<int, int>>& pts);

}  // namespace algo
