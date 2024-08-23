#include "QuadTree.hpp"

#define MAX_X 619122.989979841 // Maximum x coordinate found in "enderecobh.csv"
#define MAX_Y 7812836.09085508 // Maximum y coordinate found in "enderecobh.csv"

// All constructors and destructors
Node::Node(const Rectangle& r, Data* d) : boundary(r), data(d), bottomRight(-1), 
              bottomLeft(-1), topRight(-1), topLeft(-1) {}

Node::Node() : data(nullptr), bottomRight(-1), bottomLeft(-1), topRight(-1), topLeft(-1) {}

Node::~Node() {
    delete data;
}

Data::Data(Point& point, const std::string& neighborhood_name, const std::string& street_type, 
           const std::string& street_name, const std::string& region_name, const std::string& address_id, 
           int property_number, int street_id, int zip_code, bool activated)
          : point(point), neighborhood_name(neighborhood_name), street_type(street_type), 
            street_name(street_name), region_name(region_name), address_id(address_id), 
            property_number(property_number), street_id(street_id), zip_code(zip_code), activated(activated) {}

QuadTree::QuadTree(int capacity) : idTable(capacity), capacity(capacity), size(1) {
    nodes = new Node[capacity];
    nodes[0].boundary = Rectangle(Point(0, 0), Point(MAX_X, MAX_Y));
}

QuadTree::~QuadTree() {
    delete[] nodes;  
}

bool QuadTree::insert(Data* data) {
    // Check if the point is within the boundary
    if (!nodes[0].boundary.containsPoint(data->point)) {
        return false;
    }

    int nodeIndex = 0;
    while (true) {
        if (nodes[nodeIndex].data == nullptr) {
            // Insert data into the node, and the nodeIndex into the Hash table
            nodes[nodeIndex].data = data;
            idTable.insert(data->address_id, nodeIndex);
            return true;
        } 

        // Node is not divided, so divide it    
        if (nodes[nodeIndex].bottomLeft == -1)
            divide(nodeIndex);

        // Check what region the node should be, based on his coordinates
        if (nodes[nodes[nodeIndex].bottomLeft].boundary.containsPoint(data->point)) {
            nodeIndex = nodes[nodeIndex].bottomLeft;
        } else if (nodes[nodes[nodeIndex].topLeft].boundary.containsPoint(data->point)) {
            nodeIndex = nodes[nodeIndex].topLeft;
        } else if (nodes[nodes[nodeIndex].topRight].boundary.containsPoint(data->point)) {
            nodeIndex = nodes[nodeIndex].topRight;
        } else if (nodes[nodes[nodeIndex].bottomRight].boundary.containsPoint(data->point)) {
            nodeIndex = nodes[nodeIndex].bottomRight;
        }
    }
}

// Divide a node into four regions
void QuadTree::divide(int nodeIndex) {
    Point mid(nodes[nodeIndex].data->point.x, nodes[nodeIndex].data->point.y);

    // Create four empty "child" nodes
    nodes[size++] = Node(Rectangle(Point(mid.x, nodes[nodeIndex].boundary.bottomLeft.y), 
                                   Point(nodes[nodeIndex].boundary.topRight.x, mid.y)));
    nodes[nodeIndex].bottomRight = size - 1;

    nodes[size++] = Node(Rectangle(Point(nodes[nodeIndex].boundary.bottomLeft.x, 
                                         nodes[nodeIndex].boundary.bottomLeft.y), mid));
    nodes[nodeIndex].bottomLeft  = size - 1;

    nodes[size++] = Node(Rectangle(mid, Point(nodes[nodeIndex].boundary.topRight.x, 
                                              nodes[nodeIndex].boundary.topRight.y)));
    nodes[nodeIndex].topRight    = size - 1;

    nodes[size++] = Node(Rectangle(Point(nodes[nodeIndex].boundary.bottomLeft.x, mid.y), 
                                   Point(mid.x, nodes[nodeIndex].boundary.topRight.y)));
    nodes[nodeIndex].topLeft     = size - 1;
}

// Calculate the minimum distance from a point to a certain boundary
double QuadTree::distanceToBoundary(const Point& target, const Rectangle& boundary) const {
    double dx = 0.0, dy = 0.0;

    if (target.x < boundary.bottomLeft.x) {
        dx = boundary.bottomLeft.x - target.x; 
    } else if (target.x > boundary.topRight.x) {
        dx = target.x - boundary.topRight.x;   
    }

    if (target.y < boundary.bottomLeft.y) {
        dy = boundary.bottomLeft.y - target.y; 
    } else if (target.y > boundary.topRight.y) {
        dy = target.y - boundary.topRight.y; 
    }

    return sqrt(dx * dx + dy * dy);
}

// Perform k-nearest neighbors search
void QuadTree::knnSearch(Point& target, MaxHeap& heap, int nodeIndex) {
    if (nodes[nodeIndex].data != nullptr && nodes[nodeIndex].data->activated) {
        double dist = distance(target, nodes[nodeIndex].data->point);
        if (!heap.full()) {
            heap.push(Pair(nodes[nodeIndex].data, dist));
        } else if (dist < heap.top().distance) {
            heap.pop();
            heap.push(Pair(nodes[nodeIndex].data, dist));
        }
    }

    if (nodes[nodeIndex].bottomLeft == -1) return;

    int regionToVisit = -1;
    // More probability to find a shorter distanced node when visiting the region of the target
    if (nodes[nodes[nodeIndex].bottomLeft].boundary.containsPoint(target)) {
        regionToVisit = nodes[nodeIndex].bottomLeft;
    } else if (nodes[nodes[nodeIndex].topLeft].boundary.containsPoint(target)) {
        regionToVisit = nodes[nodeIndex].topLeft;
    } else if (nodes[nodes[nodeIndex].topRight].boundary.containsPoint(target)) {
        regionToVisit = nodes[nodeIndex].topRight;
    } else if (nodes[nodes[nodeIndex].bottomRight].boundary.containsPoint(target)) {
        regionToVisit = nodes[nodeIndex].bottomRight;
    }

    if (regionToVisit != -1) {
        knnSearch(target, heap, regionToVisit);
    }

    // Search in the remaining regions, because there is still a chance of finding good points
    int subRegions[] = {nodes[nodeIndex].bottomLeft, nodes[nodeIndex].topLeft, 
                        nodes[nodeIndex].topRight, nodes[nodeIndex].bottomRight};
    for (int i = 0; i < 4; i++) {
        if (subRegions[i] != regionToVisit) {
            double distToBoundary = distanceToBoundary(target, nodes[subRegions[i]].boundary);
            if (!heap.full() || distToBoundary < heap.top().distance) {
                knnSearch(target, heap, subRegions[i]);
            }
        }
    }
}

// Activate or deactivate a point by its ID
void QuadTree::activate(std::string id, bool command) {
    HashNode* current = idTable.getIndex(id);

    while (current != nullptr) {
        if (nodes[current->index].data->address_id == id) break;
        current = current->next;
    }

    if (current == nullptr) return;

    if (command) {
        if (nodes[current->index].data->activated) {
            std::cout << "Ponto de recarga " << id << " já estava ativo." << std::endl;
        } else {
            nodes[current->index].data->activated = true;
            std::cout << "Ponto de recarga " << id << " ativado." << std::endl;
        }
        return;
    }

    if (!nodes[current->index].data->activated) {
        std::cout << "Ponto de recarga " << id << " já estava desativado." << std::endl;
    } else {
        nodes[current->index].data->activated = false;
        std::cout << "Ponto de recarga " << id << " desativado." << std::endl;
    }
}