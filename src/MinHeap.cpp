#include "MinHeap.hpp"

MinHeap::MinHeap(int c) : size(0), capacity(c) {
    heap = new Pair[capacity];
}

MinHeap::~MinHeap() {
    delete[] heap;
}

bool MinHeap::empty() const {
    return size == 0;
}

bool MinHeap::full() const {
    return size == capacity;
}

void MinHeap::push(const Pair& d_point) {
    if (size >= capacity) {
        std::cout << "Heap overflow" << std::endl;
        return;
    }

    heap[size] = d_point;

    heapifyUp(size);
    size++;
}

Pair MinHeap::top() const {
    if (empty()) {
        std::cout << "Heap is empty" << std::endl;
        return Pair();
    }
    return heap[0];
}

Pair MinHeap::pop() {
    if (empty()) {
        std::cout << "Heap is empty" << std::endl;
        return Pair();
    }
    Pair tmp = heap[0];
    heap[0] = heap[size - 1];
    size--;
    heapifyDown(0);
    return tmp;
}       

void MinHeap::swap(int i, int j) {
    Pair aux = heap[i];
    heap[i] = heap[j];
    heap[j] = aux;
}


void MinHeap::heapifyUp(int pos) {
    while (pos > 0) {
        int father = (pos - 1) / 2;
        if (heap[father].distance > heap[pos].distance) {
            swap(father, pos);
            pos = father;
        } else break;
    }
}

void MinHeap::heapifyDown(int pos) {
    int leftSon, rightSon, smallest;
    while (true) {
        leftSon = 2 * pos + 1;
        rightSon = 2 * pos + 2;
        smallest = pos;

        if (leftSon < size && heap[leftSon].distance < heap[smallest].distance) {
            smallest = leftSon;
        }

        if (rightSon < size && heap[rightSon].distance < heap[smallest].distance) {
            smallest = rightSon;
        }

        if (smallest != pos) {
            swap(smallest, pos);
            pos = smallest;
        } else break;
    }
}

int MinHeap::findMaxIndex() const {
    int maxIndex = size / 2;
    for (int i = size / 2 + 1; i < size; ++i) {
        if (heap[i].distance > heap[maxIndex].distance) {
            maxIndex = i;
        }
    }
    return maxIndex;
}

void MinHeap::update(const Pair& new_point) {
    int maxIndex = findMaxIndex(); 

    if(new_point.distance > heap[maxIndex].distance) return;

    heap[maxIndex] = new_point;

    heapifyUp(maxIndex); 
}