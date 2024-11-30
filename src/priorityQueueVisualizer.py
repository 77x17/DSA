import tkinter as tk
from tkinter import messagebox
import heapq

class MaxHeapVisualizer:
    def __init__(self, root):
        self.heap = []  # The underlying heap (negated for max-heap)
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
            self.heap = [-num for num in numbers]  # Negate values for max-heap behavior
            heapq.heapify(self.heap)
            self.build_entry.delete(0, tk.END)
            self.display_heap()
        except ValueError:
            messagebox.showerror("Invalid Input", "Please enter a valid array of integers separated by spaces.")

    def push(self):
        """Push a value into the max-heap."""
        try:
            value = int(self.push_entry.get())
            heapq.heappush(self.heap, -value)  # Negate the value for max-heap
            self.push_entry.delete(0, tk.END)
            self.display_heap()
        except ValueError:
            messagebox.showerror("Invalid Input", "Please enter a valid integer.")

    def pop(self):
        """Pop the largest value from the max-heap."""
        if self.heap:
            heapq.heappop(self.heap)  # Negated back to positive
            self.display_heap()
        else:
            messagebox.showinfo("Heap Empty", "The heap is empty!")

    def display_heap(self):
        """Visualize the heap as a tree."""
        self.canvas.delete("all")
        if not self.heap:
            self.status_label.config(text="Heap Status: []")
            return
        
        # Update heap status (convert negated values back to positive)
        current_heap = [-x for x in self.heap]
        self.status_label.config(text=f"Heap Status: {current_heap}")
        
        # Draw the heap as a tree
        tree_width = 50
        tree_height = 50
        level_gap = 70
        
        def draw_node(value, x, y):
            """Draw a single node."""
            self.canvas.create_oval(x - 20, y - 20, x + 20, y + 20, fill="lightblue")
            self.canvas.create_text(x, y, text=str(value), font=("Arial", 12, "bold"))
        
        def draw_tree(index, x, y, dx):
            """Recursively draw the heap as a tree."""
            if index >= len(current_heap):
                return
            draw_node(current_heap[index], x, y)
            
            left_child = 2 * index + 1
            right_child = 2 * index + 2
            
            if left_child < len(current_heap):
                # Draw left line
                self.canvas.create_line(x, y + 20, x - dx, y + level_gap - 20, arrow=tk.LAST)
                draw_tree(left_child, x - dx, y + level_gap, dx // 2)
            
            if right_child < len(current_heap):
                # Draw right line
                self.canvas.create_line(x, y + 20, x + dx, y + level_gap - 20, arrow=tk.LAST)
                draw_tree(right_child, x + dx, y + level_gap, dx // 2)

        # Start drawing the heap
        draw_tree(0, 400, 50, 150)

    def clear(self):
        """Clear the heap."""
        self.heap = []
        self.display_heap()

# Main program
if __name__ == "__main__":
    root = tk.Tk()
    root.title("Max Heap Visualizer")
    app = MaxHeapVisualizer(root)
    root.mainloop()
