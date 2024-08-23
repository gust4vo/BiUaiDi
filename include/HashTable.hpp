#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <string>

// Node structure used in the hash table to store an index and a pointer to the next node (for handling collisions).
struct HashNode {
    int index;       
    HashNode* next;  

    HashNode(int idx);
};

class HashTable {
private:
    HashNode** table;  
    int capacity; // Capacity of the hash table (it will be the number of addresses).
    
    // Hash function to map a string key to an index in the table.
    int hashFunction(const std::string& id) const;

public:
    HashTable(int size);

    ~HashTable();

    // Inserts a new index-value pair into the hash table.
    void insert(const std::string& id, int index);

    HashNode* getIndex(const std::string& id) const;
};

#endif