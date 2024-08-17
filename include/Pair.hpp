#ifndef PAIR_HPP
#define PAIR_HPP

// Avoid circular dependecies.
class Data;
class QuadTree;

// Pair class to epresent a pair of pointer to Data, and the distance value.
// Used on MaxHeap implementation
class Pair {
public:
    Data* data;     
    double distance; 

    Pair(Data*& data, double& distance);
    Pair();
};

#endif 
