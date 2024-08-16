#ifndef PAIR_HPP
#define PAIR_HPP

class Data;
class QuadTree;

class Pair {
public:
    union {
        Data* data;
        QuadTree* region;
    };
    double distance;
    Pair(Data*& data, double& distance);
    Pair(QuadTree*& region, double& distance);
    Pair();
};


#endif