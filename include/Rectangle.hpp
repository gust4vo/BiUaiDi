#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "Point.hpp"

class Rectangle {
    public:
        Point bottomLeft, topRight; 
        Rectangle(Point bottomLeft, Point topRight);
        bool containsPoint(Point& p);
};

#endif
