#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "Rectangle.hpp"
#include "MinHeap.hpp"
#include <string>

class Data {
    public:
        Point point;
        std::string neighborhood_name;
        std::string street_type;
        std::string street_name;
        std::string region_name;
        std::string address_id;
        int property_number;
        int street_id;
        int zip_code;
        bool activated;

        Data(Point& point, const std::string& neighborhood_name, const std::string& street_type, const std::string& street_name, 
            const std::string& region_name, const std::string& address_id, int property_number, int street_id, int zip_code, bool activated);
};

class QuadTree {
    public:
        QuadTree(const Rectangle& r, Data* data = nullptr);
        ~QuadTree();  
        bool insert(Data* data);
        void divide();
        void activate(std::string id);
        void desactivate(std::string id);
        double distanceToBoundary(const Point& target, const Rectangle& boundary) const;
        void knnSearch(Point& target, MinHeap& heap);

    private:
        Data* data;
        Rectangle boundary;
        QuadTree* bottomRight;
        QuadTree* bottomLeft;
        QuadTree* topRight;
        QuadTree* topLeft;
};

#endif