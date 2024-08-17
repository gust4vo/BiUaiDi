#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "Point.hpp"

// Class representing a rectangular region
class Rectangle {
    public:
        Point bottomLeft; // The bottom-left corner of the rectangle.
        Point topRight;   // The top-right corner of the rectangle.

        bool containsPoint(Point& p);

        Rectangle(Point bottomLeft, Point topRight);

        Rectangle();
};

#endif
