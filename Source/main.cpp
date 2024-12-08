#include <iostream>
#include <vector>   

#include "priorityQueueByArray.cpp"
#include "priorityQueueByList.cpp"

template <class priority_queue>
void demo(priority_queue pq) {
    std::cout << "\nAdding elements to the priority queue:\n";
    std::vector<int> arr = {5, 3, 8, 1, 6, 2};
    for (int x : arr) {
        std::cout << "push(" << x << "): ";
        pq.push(x);
        pq.print();
    }

    std::cout << "\nTop element (largest): " << pq.top() << "\n"; 

    std::cout << "\nAfter pop, priority queue:\n";
    pq.pop();
    pq.print(); 

    std::cout << "\nSize of the priority queue: " << pq.size() << "\n"; 

    std::cout << "\nRemoving all elements from the priority queue:\n";
    pq.clear();
    pq.print(); 

    arr = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << "\nBuilding heap from an array: ";
    for (int x : arr) std::cout << x << ' ';
    std::cout << '\n';
    pq.build(arr);
    pq.print();  

    std::cout << "\nExtract all: \n";
    while (pq.size()) {
        std::cout << pq.top() << ' ';
        pq.pop();
    }
    std::cout << '\n';
}

int main() {
    std::ios_base::sync_with_stdio(0); std::cout.tie(0);
    std::cout << "Priority Queue (Max) by Array:\n";
    PriorityQueueByArray pq;
    demo(pq);

    std::cout << '\n';
    std::cout << "-------------------------\n";
    std::cout << "Priority Queue (Max) by List:\n";
    PriorityQueueByList pq_;
    demo(pq_);

    return 0;
}