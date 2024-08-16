#ifndef POINT_HPP
#define POINT_HPP

#include <cmath>

class Point {
    public:
        double x, y;
        Point(double x, double y);
};

double distance(Point& a, Point& b);

#endif