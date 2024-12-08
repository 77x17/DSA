#include <vector>
#include <iostream>
#include <queue>

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

    Node* createNode(int value) {
        Node* node = new Node();
        node->value = value;
        node->left = node->right = node->parent = nullptr;
        return node;
    }

    std::string binaryRepresentation(int number) {
        std::string result = "";
        while (number > 0) {
            char bit = (number % 2) + '0'; 
            result = bit + result;         
            number /= 2;                   
        }
        return result;
    }

    Node* findInsertionPoint() {
        std::string path = binaryRepresentation(heapSize + 1); 
        path.erase(0, 1); 
        
        Node* dummy = root;
        for (size_t i = 0; i < path.length() - 1; i++) { 
            if (path[i] == '0') {
                dummy = dummy->left;
            } else {
                dummy = dummy->right;
            }
        }
        return dummy;
    }

    void deleteLastNode() {
        Node* parent = last->parent;
        if (parent->right == last) {
            parent->right = nullptr; 
        } else {
            parent->left = nullptr;  
        }
        delete last; 
    }

    Node* findLastNode() {
        std::string path = binaryRepresentation(heapSize); 
        path.erase(0, 1); 

        Node* dummy = root;
        for (size_t i = 0; i < path.length(); ++i) { 
            if (path[i] == '0') {
                dummy = dummy->left;
            } else {
                dummy = dummy->right;
            }
        }
        return dummy;
    }
public:
    bool empty() const { return root == nullptr; }

    int size() const { return heapSize; }

    void push(int x) {
        Node* newNode = createNode(x);
        if (root == nullptr) {
            root = last = newNode; 
        } else {
            Node* parent = findInsertionPoint(); 
            if (parent->left == nullptr) {
                parent->left = newNode; 
            } else {
                parent->right = newNode; 
            }
            newNode->parent = parent; 
        }
        last = newNode; 
        heapSize++; 
        heapifyUp(newNode); 
    }

    int top() const {
        if (empty()) throw std::runtime_error("Heap is empty");
        
        return root->value;
    }

    void pop() {
        if (root == nullptr) {
            throw std::runtime_error("Heap is empty");
        }
        if (heapSize == 1) {
            delete root;
            root = last = nullptr;
            heapSize--;
        } else {
            std::swap(root->value, last->value);

            deleteLastNode();
            heapSize--;

            if (heapSize > 1) {
                last = findLastNode();
            } else {
                last = root;
            }

            heapifyDown(root);
        }
    }

    void clear() { 
        while (root != nullptr) pop(); 
        heapSize = 0; 
    }

    void build(std::vector<int> arr) {
        clear();
        for (const int x : arr) push(x); 
    }
    
    void print() const {
        std::cout << "heap[] = { ";
        if (empty()) {
            std::cout << "Empty }\n"; 
            return;
        }
        std::deque<Node*> dq;
        dq.push_back(root);
        while (dq.size()) {
            Node* dummy = dq.front(); dq.pop_front();
            std::cout << dummy->value << ' ';
            if (dummy->left) dq.push_back(dummy->left);
            if (dummy->right) dq.push_back(dummy->right);
        }

        std::cout << "}\n";
    }
};