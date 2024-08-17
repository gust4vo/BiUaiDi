#include "HashTable.hpp"

// Constructors and destructors
HashNode::HashNode(int idx) : index(idx), next(nullptr) {}

HashTable::HashTable(int size) : capacity(size) {
    table = new HashNode*[capacity];
    for (int i = 0; i < capacity; i++) 
        table[i] = nullptr;
}

HashTable::~HashTable() {
    for (int i = 0; i < capacity; i++) {
        while (table[i]) {
            HashNode* toDelete = table[i];  
            table[i] = table[i]->next;     
            delete toDelete;              
        }
    }
    delete[] table;
}

// Hash function that computes a hash value for a given string key.
int HashTable::hashFunction(const std::string& id) const {
    int hash = 0;
    for (char c : id) {
        hash = (hash * 31 + c) % capacity; 
    }
    return hash; 
}

// Insert a new key-index pair into the hash table.
void HashTable::insert(const std::string& id, int index) {
    int hash = hashFunction(id);  // Compute the hash 
    HashNode* newNode = new HashNode(index);  
    if (table[hash] == nullptr) {
        table[hash] = newNode;
    } else {
        // If it is not empty, traverse the linked list to find the last node.
        HashNode* current = table[hash];
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
}

HashNode* HashTable::getIndex(const std::string& id) const {
    int hash = hashFunction(id);  
    return table[hash];  // Return the linked list (can represent the actual indexNode, if there was no collision)
}
