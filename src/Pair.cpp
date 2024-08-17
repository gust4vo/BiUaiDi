#include "Pair.hpp"

// Constructors 
Pair::Pair(Data*& data, double& distance) : data(data), distance(distance) {}
Pair::Pair() : data(nullptr), distance(0.0) {}