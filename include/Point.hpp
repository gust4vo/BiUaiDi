#ifndef POINT_HPP
#define POINT_HPP

#include <cmath>

// Simple class for representing a point in 2d space with coordinates (x, y)
class Point {
    public:
        double x, y;
        Point(double x, double y);
        Point();
};

double distance(Point& a, Point& b);

#endif