#ifndef MINHEAP_HPP
#define MINHEAP_HPP

#include "Pair.hpp" 
#include <iostream>

class MinHeap {
private:
    Pair* heap;
    int size;
    int capacity; 
    void heapifyUp(int pos);
    void heapifyDown(int pos);

public:
    MinHeap(int c);
    ~MinHeap();

    void push(const Pair& d_point); 
    void swap(int i, int j);
    Pair pop();
    bool empty() const;
    bool full() const;
    Pair top() const;
    int findMaxIndex() const;
    void update(const Pair& new_point); 
};

#endif 