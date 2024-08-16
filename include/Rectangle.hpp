#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "Point.hpp"

class Rectangle {
    public:
        Point bottomLeft, topRight; 
        bool containsPoint(Point& p);
        Rectangle(Point bottomLeft, Point topRight);
        Rectangle();
};

#endif
