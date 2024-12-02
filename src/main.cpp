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

    // Hàm tạo node mới
    Node* createNode(int value) {
        Node* node = new Node();
        node->value = value;
        node->left = node->right = node->parent = nullptr;
        return node;
    }

    // Hàm chuyển đổi số thành chuỗi biểu diễn nhị phân
    std::string binaryRepresentation(int number) {
        std::string result = "";
        while (number > 0) {
            char bit = (number % 2) + '0'; // Lấy bit cuối cùng và chuyển thành ký tự
            result = bit + result;         // Ghép vào đầu chuỗi
            number /= 2;                   // Chia lấy nguyên
        }
        return result;
    }

    // Hàm tìm điểm chèn tiếp theo
    Node* findInsertionPoint() {
        std::string path = binaryRepresentation(heapSize + 1); // Biểu diễn nhị phân của (size + 1)
        path.erase(0, 1); // Loại bỏ bit đầu tiên (gốc không cần duyệt)
        
        Node* dummy = root;
        for (size_t i = 0; i < path.length() - 1; i++) { // Duyệt đến nút cha của vị trí chèn
            if (path[i] == '0') {
                dummy = dummy->left;
            } else {
                dummy = dummy->right;
            }
        }
        return dummy;
    }

    // Hàm xóa node cuối cùng
    void deleteLastNode() {
        Node* parent = last->parent;
        if (parent->right == last) {
            parent->right = nullptr; // Gỡ liên kết với con phải
        } else {
            parent->left = nullptr;  // Gỡ liên kết với con trái
        }
        delete last; // Giải phóng bộ nhớ cho node cuối cùng
    }

    // Hàm tìm node cuối cùng
    Node* findLastNode() {
        std::string path = binaryRepresentation(heapSize); // Biểu diễn nhị phân của size
        path.erase(0, 1); // Loại bỏ bit đầu tiên (gốc không cần duyệt)

        Node* dummy = root;
        for (size_t i = 0; i < path.length(); ++i) { // Duyệt theo chuỗi nhị phân
            if (path[i] == '0') {
                dummy = dummy->left;
            } else {
                dummy = dummy->right;
            }
        }
        return dummy;
    }
public:
    // Kiểm tra heap có rỗng hay không
    bool empty() const { return root == nullptr; }

    // Trả về kích thước của heap
    int size() const { return heapSize; }

    // Hàm thêm phần tử mới vào heap
    void push(int x) {
        Node* newNode = createNode(x); // Tạo node mới
        if (root == nullptr) {
            root = last = newNode; // Nếu heap rỗng, node mới là gốc
        } else {
            Node* parent = findInsertionPoint(); // Tìm vị trí chèn
            if (parent->left == nullptr) {
                parent->left = newNode; // Chèn vào con trái
            } else {
                parent->right = newNode; // Chèn vào con phải
            }
            newNode->parent = parent; // Gán node cha
        }
        last = newNode; // Cập nhật node cuối
        heapSize++; // Tăng kích thước heap
        heapifyUp(newNode); // Duy trì tính chất heap
    }

    // Lấy giá trị lớn nhất (gốc của heap)
    int top() const {
        if (empty()) throw std::runtime_error("Heap is empty");
        
        return root->value;
    }

    // Hàm xóa phần tử lớn nhất (gốc của heap)
    void pop() {
        if (root == nullptr) {
            throw std::runtime_error("Heap is empty");
        }
        if (heapSize == 1) {
            // Nếu chỉ có một phần tử
            delete root;
            root = last = nullptr;
            heapSize--;
        } else {
            // Hoán đổi giá trị giữa gốc và node cuối cùng
            std::swap(root->value, last->value);

            // Xóa node cuối cùng
            deleteLastNode();
            heapSize--;

            if (heapSize > 1) {
                // Tìm lại node cuối cùng
                last = findLastNode();
            } else {
                // Nếu chỉ còn một node, nó là node cuối cùng
                last = root;
            }

            // Duy trì tính chất heap
            heapifyDown(root);
        }
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
        std::cout << "heap[] = { ";
        if (empty()) {
            std::cout << "Empty }\n"; // Nếu heap rỗng
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
    pq.remove();
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