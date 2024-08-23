#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "QuadTree.hpp"  

// Function to insert data from a file and return the QuadTree.
QuadTree* insertData(const std::string& filename, int& size) {
    std::ifstream file(filename);

    std::string line;
    std::getline(file, line);  

    size = std::stoi(line); // Takes the number of Stations (or addresses) in the first line.
    QuadTree* Stations = new QuadTree(4 * size - 3);// Explained on main().

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
        double x = std::stod(token);  // X coordinate

        std::getline(ss, token, ';');
        double y = std::stod(token);  // Y coordinate

        // Create a Point and Data object
        Point point(x, y);
        Data* data = new Data(point, neighborhood_name, street_type, street_name, region_name, address_id, property_number, street_id, zip_code, true);
        Stations->insert(data);
        std::cout << "inserido" << std::endl;
    }

    file.close();

    return Stations;
}

int main(int argc, char* argv[]) {
    int size;

    std::string baseFile;
    std::string eventFile;

    if(argc < 5) {
        baseFile = "geracarga.base";
        eventFile = "geracarga.ev";
    } else {
        for (int i = 1; i < argc; i += 2) {
            if (std::string(argv[i]) == "-b") baseFile = argv[i + 1];
            else eventFile = argv[i + 1];
        }
    }

    // In the worst scenario, all nodes will be inserted in a occupped region, creating 4 empty nodes for 1 node.
    // This gives us a total of 4 * s - 3 nodes (s = number of recharging stations). 
    // Impossible situation in the real-world, but that's what it will be allocated in the QuadTree to ensure that there's sufficient space.

    QuadTree* Stations = insertData(baseFile, size);

    std::ifstream file(eventFile);
    std::string line;
    std::getline(file, line); // Skip the header line, it's not useful.
    auto start = std::chrono::high_resolution_clock::now();

    while(std::getline(file, line)) {
        std::cout << line << std::endl;
        std::istringstream iss(line);
        char command; iss >> command;
        if (command == 'C') { // Command 'C' for k-nearest neighbors search.
            double x, y;
            int k;
            iss >> x >> y >> k;
            MaxHeap heap(k);
            Point point(x, y);
            Stations->knnSearch(point, heap);

            int neighborhoodSize = heap.getSize();
            Pair* results = new Pair[neighborhoodSize]; 
            for(int i = 1; i <= neighborhoodSize; i++) results[neighborhoodSize-i] = heap.pop();

            // Print the k-nearest neighbors.
            for(int i = 0; i < neighborhoodSize; i++) {
                Pair address = results[i];
                std::cout << address.data->street_type << " " << address.data->street_name << ", " << 
                 address.data->property_number << ", "<< address.data->neighborhood_name << ", " 
                 << address.data->region_name << ", " << address.data->zip_code << 
                 " (" << std::fixed << std::setprecision(3) << distance(point, address.data->point) << ")" << std::endl;
            }
            delete[] results; // Clean up the results array.
        }

        // Command 'A' to activate or 'D' to deactivate a recharging station.
        if (command == 'A' || command == 'D') {
            std::string id;
            iss >> id;
            command == 'A' ? Stations->activate(id, true) : Stations->activate(id, false); 
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Tempo de execução: " << elapsed.count() << " segundos" << std::endl;
    std::ofstream times_file("tempos.txt", std::ios_base::app); 
    times_file << "(" << size << ", " << std::fixed << std::setprecision(6) << elapsed.count() << 
    ")," <<  std::endl;

    return 0;
}
