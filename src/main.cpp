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
        if (empty()) std::cout << "Empty ";
        for (int x : heap) std::cout << x << ' ';
        std::cout << "}\n";
    }

    void remove() { heap.clear(); }
};

class PriorityQueueByList {
private:
    struct Node {
        int   value  = -1;         // Giá trị của node
        Node *parent = nullptr;   // Con trỏ trỏ tới node cha
        Node *left   = nullptr;   // Con trỏ trỏ tới con trái
        Node *right  = nullptr;   // Con trỏ trỏ tới con phải
    };

    Node *root = nullptr;          // Con trỏ tới node gốc của heap
    Node *last = nullptr;          // Con trỏ tới node được thêm gần đây nhất
    int   heapSize = 0;            // Kích thước của heap

    // Duy trì tính chất heap sau khi thêm một phần tử
    void heapifyUp(Node *node) {
        while (node->parent and node->parent->value < node->value) {
            // Hoán đổi giá trị của node với node cha nếu vi phạm tính chất heap
            std::swap(node->parent->value, node->value);

            node = node->parent; // Di chuyển node lên trên
        }
    }

    // Duy trì tính chất heap sau khi xóa phần tử
    void heapifyDown(Node* node) {
        while (node) {
            Node *largest = node; // Giả sử node hiện tại là lớn nhất
            if (node->left  and node->left->value  > largest->value) largest = node->left; // So sánh với con trái
            if (node->right and node->right->value > largest->value) largest = node->right; // So sánh với con phải
            
            if (largest == node) break; // Nếu không cần hoán đổi, thoát vòng lặp

            std::swap(largest->value, node->value); // Hoán đổi giá trị với con lớn hơn
            node = largest; // Di chuyển xuống node con
        }
    }
    
    // Tìm vị trí chèn tiếp theo trong cây
    Node* getInsertionPoint() const {
        Node* dummy = root; // Bắt đầu từ gốc
        int levelNodes = 1;
        while (dummy) {
            if (not dummy->left ) return dummy; // Nếu chưa có con trái, trả về
            if (not dummy->right) return dummy; // Nếu chưa có con phải, trả về

            dummy = dummy->left; // Tiếp tục duyệt xuống con trái
        }
        return nullptr; // Trường hợp không tìm thấy (không xảy ra)
    }
public:
    // Kiểm tra heap có rỗng hay không
    bool empty() const { return root == nullptr; }

    // Trả về kích thước của heap
    int size() const { return heapSize; }

    // Thêm một phần tử mới vào heap
    void push(int x) {
        Node *newNode = new Node(); newNode->value = x; // Tạo node mới

        if (empty()) {
            root = last = newNode; // Nếu heap rỗng, node mới là gốc
        } else {
            Node* parent = getInsertionPoint(); // Tìm vị trí chèn

            if (not parent->left) {
                parent->left  = newNode; // Chèn vào con trái
            } else {
                parent->right = newNode; // Chèn vào con phải
            }
            newNode->parent = parent; // Gán node cha
            last = newNode;           // Cập nhật node cuối cùng
        }
        heapSize++; // Tăng kích thước heap
        heapifyUp(newNode); // Duy trì tính chất heap
    }

    // Lấy giá trị lớn nhất (gốc của heap)
    int top() const {
        if (empty()) throw std::runtime_error("Heap is empty");
        
        return root->value;
    }

    // Xóa phần tử lớn nhất (gốc của heap)
    void pop() {
        if (empty()) throw std::runtime_error("Heap is empty");

        if (size() == 1) {
            // Nếu chỉ có một phần tử, xóa node gốc
            delete root;
            root = last = nullptr;
        } else {
            // Hoán đổi giá trị của gốc và node cuối cùng
            std::swap(root->value, last->value);

            // Xóa node cuối cùng
            Node *parent = last->parent;
            if (parent->right == last) {
                parent->right = nullptr;
            } else {
                parent->left  = nullptr;
            }
            delete last;

            // Tìm lại node cuối cùng mới
            Node *dummy = root;
            while (dummy->left or dummy->right) {
                if (dummy->right) {
                    dummy = dummy->right;
                } else {
                    dummy = dummy->left;
                }
            }
            last = dummy;

            // Duy trì tính chất heap
            heapifyDown(root);
        }

        heapSize--; // Giảm kích thước heap
    }

    // Xóa toàn bộ heap
    void remove() { 
        while (root != nullptr) pop(); // Liên tục xóa phần tử gốc
        heapSize = 0; // Đặt kích thước về 0
    }

    // Xây dựng heap từ một mảng
    void build(std::vector<int> arr) {
        remove();
        for (const int x : arr) push(x); // Thêm từng phần tử vào heap
    }
    
    // Hàm in toàn bộ heap
    void print() const {
        std::cout << "heap[] = { \n";
        if (empty()) std::cout << "Empty "; // Nếu heap rỗng
        
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