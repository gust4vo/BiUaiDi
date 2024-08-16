#ifndef PAIR_HPP
#define PAIR_HPP

class Data;
class QuadTree;

class Pair {
public:
    union {
        Data* data;
        int regionIndex;
    };
    double distance;
    Pair(Data*& data, double& distance);
    Pair(int& regionIndex, double& distance);
    Pair();
};


#endif