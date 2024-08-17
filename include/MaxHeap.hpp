#ifndef MAXHEAP_HPP
#define MAXHEAP_HPP

#include "Pair.hpp" 
#include <iostream>

class MaxHeap {
private:
    Pair* heap;      // Dynamic array to store the heap elements (Pair objects).
    int size;        // Current number of elements in the heap.
    int capacity;    // Maximum capacity of the heap.

    void heapifyUp(int pos);
    void heapifyDown(int pos);

public:
    MaxHeap(int c);
    ~MaxHeap();

    // Pushes a new Pair into the heap, maintaining the max-heap property.
    void push(const Pair& d_point);

    // Swaps the elements at indices i and j in the heap.
    void swap(int i, int j);

    // Removes and returns the top element (the largest element) from the heap.
    Pair pop();

    // Checks if the heap is empty.
    bool empty() const;

    // Checks if the heap is full.
    bool full() const;

    // Returns the top element (the largest element) without removing it.
    Pair top() const;

    // Returns the current size of the heap (number of elements).
    int getSize() const;
};

#endif  
