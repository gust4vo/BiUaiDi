#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include "QuadTree.hpp"  // Inclua o arquivo de definição da classe Data e Point

void insertData(QuadTree& q, const std::string& filename) {
    std::ifstream file(filename);

    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;

        std::getline(ss, token, ';');
        std::string address_id = token;

        std::getline(ss, token, ';');
        int street_id = std::stoi(token);

        std::getline(ss, token, ';');
        std::string street_type = token;

        std::getline(ss, token, ';');
        std::string street_name = token;

        std::getline(ss, token, ';');
        int property_number = std::stoi(token);

        std::getline(ss, token, ';');
        std::string neighborhood_name = token;

        std::getline(ss, token, ';');
        std::string region_name = token;

        std::getline(ss, token, ';');
        int zip_code = std::stoi(token);

        std::getline(ss, token, ';');
        double x = std::stod(token);  // Coordenada X

        std::getline(ss, token, ';');
        double y = std::stod(token);  // Coordenada Y

        // Criar o objeto Point e Data
        Point point(x, y);

        Data* data = new Data(point, neighborhood_name, street_type, street_name, region_name, address_id, property_number, street_id, zip_code, true);
        q.insert(data);
    }

    file.close();
}

int main() {
    QuadTree q(105);

    insertData(q, "geracarga.base");

    std::ifstream file("geracarga.ev");

    std::string line;
    std::getline(file, line);

    while(std::getline(file, line)) {
        std::cout << line << std::endl;
        std::istringstream iss(line);
        char comando; iss >> comando;
        if (comando == 'C') {
            double x, y;
            int k;
            iss >> x >> y >> k;
            MinHeap heap(k);
            Point point(x, y);
            q.knnSearch(point, heap);
            while (!heap.empty()) {
                Pair address = heap.pop();
                std::cout << address.data->street_type << " " <<  address.data->street_name << ", " << 
                 address.data->property_number << ", "<< address.data->neighborhood_name << ", " 
                 << address.data->region_name << ", " << address.data->zip_code << 
                 " (" << std::fixed << std::setprecision(3) << distance(point, address.data->point) << ")" << std::endl;
            }
        }
        if (comando == 'A' || comando == 'D') {
            std::string id;
            iss >> id;
            comando == 'A' ? q.activate(id) : q.desactivate(id); 
        }
    }
    return 0;
}
