#include "Rectangle.hpp"

// Constructors
Rectangle::Rectangle(Point bottomLeft, Point topRight) 
    : bottomLeft(bottomLeft), topRight(topRight) {}

Rectangle::Rectangle() {}

// Checks if a given point is within the bounds of this rectangle.
bool Rectangle::containsPoint(Point& p) {
    return (p.x <= topRight.x) && (p.x >= bottomLeft.x) && 
           (p.y <= topRight.y) && (p.y >= bottomLeft.y); 
}