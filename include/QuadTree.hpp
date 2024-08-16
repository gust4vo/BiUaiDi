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

struct Node {
    public:
        Rectangle boundary;
        Data* data;
        int bottomRight;
        int bottomLeft;
        int topRight;
        int topLeft;
        Node();
        Node(const Rectangle& r, Data* d = nullptr);
};

class QuadTree {
    public:
        QuadTree(int capacity);
        ~QuadTree();  
        bool insert(Data* data);
        void divide(int nodeIndex);
        void activate(std::string id, int nodeIndex = 0);
        void desactivate(std::string id, int nodeIndex = 0);
        double distanceToBoundary(const Point& target, const Rectangle& boundary) const;
        void knnSearch(Point& target, MinHeap& heap, int nodeIndex = 0);

    private:
        Node* nodes;
        int capacity;
        int size;
};

#endif