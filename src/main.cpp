/*
    The following code snippets represent a max priority queue.
*/

#include <iostream>

#include <vector>   
#include <queue>

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
        if (left  < heap.size() and heap[left]  > heap[largest]) largest = left;
        if (right < heap.size() and heap[right] > heap[largest]) largest = right;
        
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
        if (empty()) std::cout << "Empty";
        for (int x : heap) std::cout << x << ' ';
        std::cout << "}\n";
    }

    void remove() { heap.clear(); }
};

class PriorityQueueByList {
private:
    struct Node {
        int   value  = -1;
        Node *parent = nullptr;
        Node *left   = nullptr;
        Node *right  = nullptr;
    };

    Node *root = nullptr;
    Node *last = nullptr;
    int   heapSize = 0;

    void heapifyUp(Node *node) {
        while (node->parent and node->parent->value < node->value) {
            std::swap(node->parent->value, node->value);

            node = node->parent;
        }
    }

    void heapifyDown(Node* node) {
        while (node) {
            Node *largest = node;
            if (node->left  and node->left->value  > largest->value) largest = node->left;
            if (node->right and node->right->value > largest->value) largest = node->right;
            
            if (largest == node) break;

            std::swap(largest->value, node->value);
            node = largest;
        }
    }
    
    Node* getInsertionPoint() const {
        Node* dummy = root;
        int levelNodes = 1;
        while (dummy) {
            if (not dummy->left ) return dummy;
            if (not dummy->right) return dummy;

            dummy = dummy->left;
        }
        return nullptr;
    }
public:
    bool empty() const { return root == nullptr; }
    int size() const { return heapSize; }

    void push(int x) {
        Node *newNode = new Node(); newNode->value = x;

        if (empty()) {
            root = last = newNode;
        } else {
            Node* parent = getInsertionPoint();

            if (not parent->left) {
                parent->left  = newNode;
            } else {
                parent->right = newNode;
            }
            newNode->parent = parent;
            last = newNode;
        }
        heapSize++;
        heapifyUp(newNode);
    }

    int top() const {
        if (empty()) throw std::runtime_error("Heap is empty");
        
        return root->value;
    }

    void pop() {
        if (empty()) throw std::runtime_error("Heap is empty");

        if (size() == 1) {
            delete root;
            root = last = nullptr;
        } else {
            std::swap(root->value, last->value);

            Node *parent = last->parent;
            if (parent->right == last) {
                parent->right = nullptr;
            } else {
                parent->left  = nullptr;
            }
            delete last;

            Node *dummy = root;
            while (dummy->left or dummy->right) {
                if (dummy->right) {
                    dummy = dummy->right;
                } else {
                    dummy = dummy->left;
                }
            }
            last = dummy;

            heapifyDown(root);
        }

        heapSize--;
    }

    void build(std::vector<int> arr) {
        for (const int x : arr) push(x);
    }

    void print(Node* node) const {
        if (node == nullptr) return;

        std::cout << node->value << ' ';
        print(node->left);
        print(node->right);
    }

    void print() const {
        std::cout << "heap[] = { ";
        if (empty()) std::cout << "Empty";
        print(root);
        std::cout << "}\n";
    }

    void remove() { 
        while (root != nullptr) pop();
        heapSize = 0;
    }
};

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

    arr = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << "\nBuilding heap from an array: ";
    for (int x : arr) std::cout << x << ' ';
    std::cout << '\n';
    pq.build(arr);
    pq.print();  

    std::cout << "\nRemoving all elements from the priority queue:\n";
    pq.remove();
    pq.print();  
}

int main() {
    PriorityQueueByArray pq;
    demo(pq);

    PriorityQueueByList pq_;
    demo(pq_);

    return 0;
}