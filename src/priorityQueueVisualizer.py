import tkinter as tk
from tkinter import messagebox
import time


class MaxHeapVisualizer:
    def __init__(self, root):
        self.heap = []  # Max-heap array
        self.canvas = tk.Canvas(root, width=800, height=400, bg="white")
        self.canvas.pack()

        # Notes for users
        note_label = tk.Label(root, text="Instructions:", font=("Arial", 12, "bold"))
        note_label.pack(anchor="w", padx=10, pady=(10, 0))
        instructions = ("1. Use the 'Build Heap' input to enter an array of integers separated by spaces (e.g., '10 20 30').\n"
                        "2. Use the 'Push' input to add a single integer to the heap.\n"
                        "3. Click 'Pop Max' to remove the largest element from the heap.\n"
                        "4. Click 'Clear' to reset the heap.")
        instruction_label = tk.Label(root, text=instructions, justify="left", anchor="w", font=("Arial", 10))
        instruction_label.pack(anchor="w", padx=10, pady=(0, 10))

        # Input and buttons for building the heap
        build_frame = tk.Frame(root)
        build_frame.pack(pady=5)
        build_label = tk.Label(build_frame, text="Build Heap (Array):")
        build_label.pack(side=tk.LEFT)
        self.build_entry = tk.Entry(build_frame, width=30)
        self.build_entry.pack(side=tk.LEFT, padx=5)
        build_button = tk.Button(build_frame, text="Build Heap", command=self.build_heap)
        build_button.pack(side=tk.LEFT, padx=5)

        # Input and buttons for pushing values
        push_frame = tk.Frame(root)
        push_frame.pack(pady=5)
        push_label = tk.Label(push_frame, text="Push (Single Integer):")
        push_label.pack(side=tk.LEFT)
        self.push_entry = tk.Entry(push_frame, width=10)
        self.push_entry.pack(side=tk.LEFT, padx=5)
        push_button = tk.Button(push_frame, text="Push", command=self.push)
        push_button.pack(side=tk.LEFT, padx=5)

        # Other controls
        control_frame = tk.Frame(root)
        control_frame.pack(pady=5)
        pop_button = tk.Button(control_frame, text="Pop Max", command=self.pop)
        pop_button.pack(side=tk.LEFT, padx=5)
        clear_button = tk.Button(control_frame, text="Clear", command=self.clear)
        clear_button.pack(side=tk.LEFT, padx=5)

        # Heap status
        self.status_label = tk.Label(root, text="Heap Status: []", anchor="w")
        self.status_label.pack(fill=tk.X, padx=10, pady=5)

    def build_heap(self):
        """Build the max-heap from an array of numbers."""
        try:
            numbers = list(map(int, self.build_entry.get().split()))
            self.heap = []
            for num in numbers:
                self.push_to_heap(num)
            self.build_entry.delete(0, tk.END)
            self.display_heap()
        except ValueError:
            messagebox.showerror("Invalid Input", "Please enter a valid array of integers separated by spaces.")

    def push(self):
        """Push a value into the max-heap."""
        try:
            value = int(self.push_entry.get())
            self.push_entry.delete(0, tk.END)
            self.push_to_heap(value)
            self.display_heap()
        except ValueError:
            messagebox.showerror("Invalid Input", "Please enter a valid integer.")

    def pop(self):
        """Pop the largest value from the max-heap."""
        if self.heap:
            self.pop_from_heap()
            self.display_heap()
        else:
            messagebox.showinfo("Heap Empty", "The heap is empty!")

    def clear(self):
        """Clear the heap."""
        self.heap = []
        self.display_heap()

    def push_to_heap(self, value):
        """Custom push operation for max-heap with animation."""
        self.heap.append(value)  # Thêm nút mới vào cuối heap
        self.display_heap()  # Hiển thị heap ngay lập tức sau khi thêm
        self.canvas.update()  # Cập nhật giao diện
        time.sleep(1)  # Tạm dừng để người dùng quan sát

        # Tiến hành hoán đổi nếu cần để duy trì tính chất max-heap
        index = len(self.heap) - 1
        while index > 0:
            parent = (index - 1) // 2
            if self.heap[index] > self.heap[parent]:
                self.swap_with_animation(index, parent)  # Gọi hàm hoán đổi có hiệu ứng
                index = parent
            else:
                break

    def pop_from_heap(self):
        """Custom pop operation for max-heap with animation."""
        if len(self.heap) == 1:
            self.heap.pop()
            return
        self.swap_with_animation(0, len(self.heap) - 1)
        max_value = self.heap.pop()
        index = 0
        while True:
            left = 2 * index + 1
            right = 2 * index + 2
            largest = index

            if left < len(self.heap) and self.heap[left] > self.heap[largest]:
                largest = left
            if right < len(self.heap) and self.heap[right] > self.heap[largest]:
                largest = right
            if largest == index:
                break
            self.swap_with_animation(index, largest)
            index = largest

    def swap_with_animation(self, index1, index2):
        """Animate the swap between two nodes."""
        self.heap[index1], self.heap[index2] = self.heap[index2], self.heap[index1]
        self.display_heap()
        self.canvas.update()
        time.sleep(1)

    def display_heap(self):
        """Visualize the heap as a tree."""
        self.canvas.delete("all")
        if not self.heap:
            self.status_label.config(text="Heap Status: []")
            return

        # Update heap status
        self.status_label.config(text=f"Heap Status: {self.heap}")

        # Draw the heap as a tree
        level_gap = 70
        node_radius = 20
        canvas_width = 800
        canvas_height = 400

        def draw_node(value, x, y):
            """Draw a single node."""
            self.canvas.create_oval(x - node_radius, y - node_radius, x + node_radius, y + node_radius, fill="lightblue")
            self.canvas.create_text(x, y, text=str(value), font=("Arial", 12, "bold"))

        def draw_tree(index, x, y, dx):
            """Recursively draw the heap as a tree."""
            if index >= len(self.heap):
                return
            draw_node(self.heap[index], x, y)

            left_child = 2 * index + 1
            right_child = 2 * index + 2

            if left_child < len(self.heap):
                self.canvas.create_line(x, y + node_radius, x - dx, y + level_gap - node_radius, arrow=tk.LAST)
                draw_tree(left_child, x - dx, y + level_gap, dx // 2)

            if right_child < len(self.heap):
                self.canvas.create_line(x, y + node_radius, x + dx, y + level_gap - node_radius, arrow=tk.LAST)
                draw_tree(right_child, x + dx, y + level_gap, dx // 2)

        # Start drawing the heap
        draw_tree(0, canvas_width // 2, 50, 150)


# Main program
if __name__ == "__main__":
    root = tk.Tk()
    root.title("Max Heap Visualizer with Animation")
    app = MaxHeapVisualizer(root)
    root.mainloop()