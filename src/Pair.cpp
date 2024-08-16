#include "Pair.hpp"

Pair::Pair(Data*& data, double& distance) : data(data), distance(distance) {}

Pair::Pair(QuadTree*& region, double& distance) : region(region), distance(distance) {}

Pair::Pair() : data(nullptr), distance(0.0) {}