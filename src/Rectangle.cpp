#include "Rectangle.hpp"

Rectangle::Rectangle(Point bottomLeft, Point topRight) : bottomLeft(bottomLeft), topRight(topRight) {}

// Verifies if the point is contained on the rectangle limits
bool Rectangle::containsPoint(Point& p) {
    return (p.x <= topRight.x) && (p.x >= bottomLeft.x) && 
           (p.y <= topRight.y) && (p.y >= bottomLeft.y); 
}


