#include "QuadTree.hpp"

Node::Node(const Rectangle& r, Data* d) : boundary(r), data(d), bottomRight(-1), 
              bottomLeft(-1), topRight(-1), topLeft(-1) {}

Node::Node() : data(nullptr), bottomRight(-1), bottomLeft(-1), topRight(-1), topLeft(-1) {}


Data::Data(Point& point, const std::string& neighborhood_name, const std::string& street_type, const std::string& street_name, 
            const std::string& region_name, const std::string& address_id, int property_number, int street_id, int zip_code, bool activated)
          : point(point), neighborhood_name(neighborhood_name), street_type(street_type), street_name(street_name), 
            region_name(region_name), address_id(address_id), property_number(property_number), street_id(street_id), 
            zip_code(zip_code), activated(activated) {}

QuadTree::QuadTree(int capacity) : capacity(capacity), size(1) {
    nodes = new Node[capacity];
    nodes[0].boundary = Rectangle(Point(0, 0), Point(10000000.0, 10000000.0));
}

QuadTree::~QuadTree() {
    delete[] nodes;  
}

bool QuadTree::insert(Data* data) {
    if (!nodes[0].boundary.containsPoint(data->point)) {
        return false;
    }

    int nodeIndex = 0;
    while (true) {
        if (nodes[nodeIndex].data == nullptr) {
            nodes[nodeIndex].data = data;
            return true;
        } 

        if (nodes[nodeIndex].bottomLeft == -1)
            divide(nodeIndex);

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

void QuadTree::divide(int nodeIndex) {
    Point mid(nodes[nodeIndex].data->point.x, nodes[nodeIndex].data->point.y);

    nodes[size++] = Node(Rectangle(Point(mid.x, nodes[nodeIndex].boundary.bottomLeft.y), Point(nodes[nodeIndex].boundary.topRight.x, mid.y)));
    nodes[nodeIndex].bottomRight = size - 1;

    nodes[size++] = Node(Rectangle(Point(nodes[nodeIndex].boundary.bottomLeft.x, nodes[nodeIndex].boundary.bottomLeft.y), mid));
    nodes[nodeIndex].bottomLeft  = size - 1;

    nodes[size++] = Node(Rectangle(mid, Point(nodes[nodeIndex].boundary.topRight.x, nodes[nodeIndex].boundary.topRight.y)));
    nodes[nodeIndex].topRight    = size - 1;

    nodes[size++] = Node(Rectangle(Point(nodes[nodeIndex].boundary.bottomLeft.x, mid.y), Point(mid.x, nodes[nodeIndex].boundary.topRight.y)));
    nodes[nodeIndex].topLeft     = size - 1;
}

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


void QuadTree::knnSearch(Point& target, MinHeap& heap, int nodeIndex) {

    if (nodes[nodeIndex].data != nullptr && nodes[nodeIndex].data->activated) {
        double dist = distance(target, nodes[nodeIndex].data->point);
        if (!heap.full()) {
            heap.push(Pair(nodes[nodeIndex].data, dist));
        } else {
            heap.update(Pair(nodes[nodeIndex].data, dist));
        }
    }

    if (nodes[nodeIndex].bottomLeft == -1) return; 

    MinHeap regions(4);
     
    if (nodes[nodeIndex].bottomRight != -1) {
        double dist = distanceToBoundary(target, nodes[nodes[nodeIndex].bottomRight].boundary);
        regions.push(Pair(nodes[nodeIndex].bottomRight, dist));
    }
    if (nodes[nodeIndex].bottomLeft != -1) {
        double dist = distanceToBoundary(target, nodes[nodes[nodeIndex].bottomLeft].boundary);
        regions.push(Pair(nodes[nodeIndex].bottomLeft, dist));
    }
    if (nodes[nodeIndex].topRight != -1) {
        double dist = distanceToBoundary(target, nodes[nodes[nodeIndex].topRight].boundary);
        regions.push(Pair(nodes[nodeIndex].topRight, dist));
    }
    if (nodes[nodeIndex].topLeft != -1) {
        double dist = distanceToBoundary(target, nodes[nodes[nodeIndex].topLeft].boundary);
        regions.push(Pair(nodes[nodeIndex].topLeft, dist));
    }

    while(!regions.empty()) {
        knnSearch(target, heap, regions.pop().regionIndex);
    }
}

void QuadTree::activate(std::string id, int nodeIndex) {
    if (nodes[nodeIndex].data != nullptr && nodes[nodeIndex].data->address_id == id) {
        if (nodes[nodeIndex].data->activated) {
            std::cout << "Ponto de recarga " << id << " já estava ativo." << std::endl;
        } else {
            nodes[nodeIndex].data->activated = true;
            std::cout << "Ponto de recarga " << id << " ativado." << std::endl;
        }
        return;
    }

    if (nodes[nodeIndex].bottomLeft == -1) {
        return;
    }

    if (nodes[nodeIndex].bottomLeft != -1) activate(id, nodes[nodeIndex].bottomLeft);
    if (nodes[nodeIndex].bottomRight != -1) activate(id, nodes[nodeIndex].bottomRight);
    if (nodes[nodeIndex].topLeft != -1) activate(id, nodes[nodeIndex].topLeft);
    if (nodes[nodeIndex].topRight != -1) activate(id, nodes[nodeIndex].topRight);
}

void QuadTree::desactivate(std::string id, int nodeIndex) {
    if (nodes[nodeIndex].data != nullptr && nodes[nodeIndex].data->address_id == id) {
        if (!nodes[nodeIndex].data->activated) {
            std::cout << "Ponto de recarga " << id << " já estava desativado." << std::endl;
        } else {
            nodes[nodeIndex].data->activated = false;
            std::cout << "Ponto de recarga " << id << " desativado." << std::endl;
        }
        return;
    }

    if (nodes[nodeIndex].bottomLeft == -1) {
        return;
    }

    if (nodes[nodeIndex].bottomLeft != -1) desactivate(id, nodes[nodeIndex].bottomLeft);
    if (nodes[nodeIndex].bottomRight != -1) desactivate(id, nodes[nodeIndex].bottomRight);
    if (nodes[nodeIndex].topLeft != -1) desactivate(id, nodes[nodeIndex].topLeft);
    if (nodes[nodeIndex].topRight != -1) desactivate(id, nodes[nodeIndex].topRight);
}
