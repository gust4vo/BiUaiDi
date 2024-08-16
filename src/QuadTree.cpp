#include "QuadTree.hpp"

QuadTree::QuadTree(const Rectangle& r, Data* data) : data(data), boundary(r), bottomRight(nullptr), 
                    bottomLeft(nullptr), topRight(nullptr), topLeft(nullptr) {}
            
QuadTree::~QuadTree() {
    delete bottomRight;
    delete bottomLeft;
    delete topRight;
    delete topLeft;    
}

Data::Data(Point& point, const std::string& neighborhood_name, const std::string& street_type, const std::string& street_name, 
            const std::string& region_name, const std::string& address_id, int property_number, int street_id, int zip_code, bool activated)
          : point(point), neighborhood_name(neighborhood_name), street_type(street_type), street_name(street_name), 
            region_name(region_name), address_id(address_id), property_number(property_number), street_id(street_id), 
            zip_code(zip_code), activated(activated) {}

bool QuadTree::insert(Data* data) {
    if (!boundary.containsPoint(data->point)) {
        return false;
    }

    if (this->data == nullptr) {
        this->data = data;
        return true;
    } 

    if (bottomLeft == nullptr) divide();

    if (bottomLeft->insert(data))       return true;
    else if (topLeft->insert(data))     return true;
    else if (topRight->insert(data))    return true;
    else if (bottomRight->insert(data)) return true;
    return false;
}

void QuadTree::divide() {
    Point mid(data->point.x, data->point.y);

    bottomRight = new QuadTree(Rectangle(Point(mid.x, boundary.bottomLeft.y), Point(boundary.topRight.x, mid.y)));
    bottomLeft  = new QuadTree(Rectangle(Point(boundary.bottomLeft.x, boundary.bottomLeft.y), mid));
    topRight    = new QuadTree(Rectangle(mid, Point(boundary.topRight.x, boundary.topRight.y)));
    topLeft     = new QuadTree(Rectangle(Point(boundary.bottomLeft.x, mid.y), Point(mid.x, boundary.topRight.y)));
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


void QuadTree::knnSearch(Point& target, MinHeap& heap) {
    if (data != nullptr && data->activated) {
        double dist = distance(target, data->point);
        if (!heap.full()) {
            heap.push(Pair(data, dist));
        } else {
            heap.update(Pair(data, dist));
        }
    }

    if (bottomLeft == nullptr) return; 

    MinHeap regions(4);
     
    if (bottomRight != nullptr) {
        double dist = distanceToBoundary(target, bottomRight->boundary);
        regions.push(Pair(bottomRight, dist));
    }
    if (bottomLeft != nullptr) {
        double dist = distanceToBoundary(target, bottomLeft->boundary);
        regions.push(Pair(bottomLeft, dist));
    }
    if (topRight != nullptr) {
        double dist = distanceToBoundary(target, topRight->boundary);
        regions.push(Pair(topRight, dist));
    }
    if (topLeft != nullptr) {
        double dist = distanceToBoundary(target, topLeft->boundary);
        regions.push(Pair(topLeft, dist));
    }

    while(!regions.empty()) {
        regions.pop().region->knnSearch(target, heap);
    }
}

void QuadTree::activate(std::string id) {
    if (data != nullptr && data->address_id == id) {
        if(data->activated) {
            std::cout << "Ponto de recarga " << id << " já estava ativo." << std::endl;
        } else {
            std::cout << "Ponto de recarga " << id << " ativado." << std::endl;
            data->activated = true;
        }
        return;
    }

    if (bottomLeft == nullptr) {
        return;
    }

    if (bottomLeft != nullptr) bottomLeft->activate(id);
    if (bottomRight != nullptr) bottomRight->activate(id);
    if (topLeft != nullptr) topLeft->activate(id);
    if (topRight != nullptr) topRight->activate(id);
}
void QuadTree::desactivate(std::string id) {
    if (data != nullptr && data->address_id == id) {
        if(!data->activated) {
            std::cout << "Ponto de recarga " << id << " já estava desativado." << std::endl;
        } else {
            std::cout << "Ponto de recarga " << id << " desativado." << std::endl;
            data->activated = false;
        }
        return;
    }

    if (bottomLeft == nullptr) {
        return;
    }

    if (bottomLeft != nullptr) bottomLeft->desactivate(id);
    if (bottomRight != nullptr) bottomRight->desactivate(id);
    if (topLeft != nullptr) topLeft->desactivate(id);
    if (topRight != nullptr) topRight->desactivate(id);
}