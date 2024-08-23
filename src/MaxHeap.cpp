#include "MaxHeap.hpp"

MaxHeap::MaxHeap(int c) : size(0), capacity(c) {
    heap = new Pair[capacity];
}

MaxHeap::~MaxHeap() {
    delete[] heap;
}

bool MaxHeap::empty() const {
    return size == 0;
}

bool MaxHeap::full() const {
    return size == capacity;
}

void MaxHeap::push(const Pair& d_point) {
    if (size >= capacity) {
        std::cout << "Heap overflow" << std::endl;
        return;
    }

    heap[size] = d_point;

    heapifyUp(size);
    size++;
}

Pair MaxHeap::top() const {
    if (empty()) {
        std::cout << "Heap is empty" << std::endl;
        return Pair();
    }
    return heap[0];
}

Pair MaxHeap::pop() {
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

void MaxHeap::swap(int i, int j) {
    Pair aux = heap[i];
    heap[i] = heap[j];
    heap[j] = aux;
}

void MaxHeap::heapifyUp(int pos) {
    while (pos > 0) {
        int father = (pos - 1) / 2;
        if (heap[father].distance < heap[pos].distance) {
            swap(father, pos);
            pos = father;
        } else break;
    }
}

void MaxHeap::heapifyDown(int pos) {
    int leftSon, rightSon, largest;
    while (true) {
        leftSon = 2 * pos + 1;
        rightSon = 2 * pos + 2;
        largest = pos;

        if (leftSon < size && heap[leftSon].distance > heap[largest].distance) {
            largest = leftSon;
        }

        if (rightSon < size && heap[rightSon].distance > heap[largest].distance) {
            largest = rightSon;
        }

        if (largest != pos) {
            swap(largest, pos);
            pos = largest;
        } else break;
    }
}

int MaxHeap::getSize() const {
    return size;
}