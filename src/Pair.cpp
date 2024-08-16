#include "Pair.hpp"

Pair::Pair(Data*& data, double& distance) : data(data), distance(distance) {}

Pair::Pair(int& regionIndex, double& distance) : regionIndex(regionIndex), distance(distance) {}

Pair::Pair() : data(nullptr), distance(0.0) {}