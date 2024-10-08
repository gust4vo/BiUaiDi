#include "Point.hpp"

// Calculate the euclidean distance between this two points
double distance(Point& a, Point& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

Point::Point(double x, double y) : x(x), y(y) {}
Point::Point() : x(0), y(0) {}