#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "Rectangle.hpp"
#include "MaxHeap.hpp"
#include "HashTable.hpp"

// Class representing the data of the Recharge Station stored in each QuadTree node.
class Data {
    public:
        Point point;  // The physical coordinates associated with the data
        std::string neighborhood_name; 
        std::string street_type;       
        std::string street_name;      
        std::string region_name;      
        std::string address_id;        
        int property_number;          
        int street_id;                
        int zip_code;                  
        bool activated; // Indicates if the recharge station is activated or not

        Data(Point& point, const std::string& neighborhood_name, const std::string& street_type, const std::string& street_name, 
            const std::string& region_name, const std::string& address_id, int property_number, int street_id, int zip_code, bool activated);
};

// Struct representing a node in the QuadTree.
class Node {
    public:
        Rectangle boundary;   
        Data* data;           
        int bottomRight;      // Index of the bottom-right child node.
        int bottomLeft;       // Index of the bottom-left child node.
        int topRight;         // Index of the top-right child node.
        int topLeft;          // Index of the top-left child node.

        Node(const Rectangle& r, Data* d = nullptr);
        Node();
        ~Node();
};

// Class representing the QuadTree structure.
class QuadTree {
    public:
        QuadTree(int capacity);
        ~QuadTree();  

        // Method to insert data into the QuadTree.
        bool insert(Data* data);

        // Method to divide a node into four subregions.
        void divide(int nodeIndex);

        // Method to activate or deactivate a data point in the QuadTree by ID.
        void activate(std::string id, bool command);

        // Method to calculate the minimum distance from a target point to the boundary of a given rectangle.
        double distanceToBoundary(const Point& target, const Rectangle& boundary) const;

        // Method to perform k-nearest neighbors search starting from a given node.
        void knnSearch(Point& target, MaxHeap& heap, int nodeIndex = 0);

    private:
        HashTable idTable;    // HashTable to keep track of node indices by address ID.
        Node* nodes;          // Array of nodes in the QuadTree.
        int capacity;         // Maximum capacity of the QuadTree (number of nodes).
        int size;             // Current size of the QuadTree, it also counts empty nodes created by division.
};

#endif
