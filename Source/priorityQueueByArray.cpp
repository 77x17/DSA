#include <vector>
#include <iostream>

class PriorityQueueByArray {
private:
    std::vector<int> heap; 

    void heapifyUp(int index) {
        int parent = (index - 1) / 2;
        while (index != 0 and heap[parent] < heap[index]) {
            std::swap(heap[parent], heap[index]);

            index  = parent;
            parent = (index - 1) / 2;
        }
    }

    void heapifyDown(int index) {
        int largest = index;
        int left    = index * 2 + 1;
        int right   = index * 2 + 2;
        if (left  < (int)heap.size() and heap[left]  > heap[largest]) largest = left;
        if (right < (int)heap.size() and heap[right] > heap[largest]) largest = right;
        
        if (largest != index) {
            std::swap(heap[largest], heap[index]);

            heapifyDown(largest);
        }
    }
public:
    bool empty() const { return heap.empty(); }
    int size() const { return heap.size(); }

    void push(int x) {
        heap.push_back(x);
        heapifyUp(heap.size() - 1);
    }

    int top() const {
        if (empty()) throw std::runtime_error("Heap is empty");
        
        return heap.front();
    }

    void pop() {
        if (empty()) throw std::runtime_error("Heap is empty");
        
        heap.front() = heap.back();
        heap.pop_back();
        heapifyDown(0);
    }

    void build(std::vector<int> arr) {
        heap = arr;
        for (int i = heap.size() / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }

    void print() const {
        std::cout << "heap[] = { ";
        if (empty()) std::cout << "Empty ";
        for (int x : heap) std::cout << x << ' ';
        std::cout << "}\n";
    }

    void clear() { heap.clear(); }
};