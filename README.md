# Low-Level Memory Allocator

This project implements a basic custom memory allocator in C, providing a simplified approach to memory management within a pre-defined heap space. It allows users to allocate and free memory blocks and observe the heap's status.

## Features

* **Fixed-Size Memory Pool:** A `main_memory` array acts as a fixed-size heap (1 MB by default) for all allocations.
* **Initialization:** Initializes the entire memory pool as one large free block at startup.
* **Memory Allocation (`allocate_memory`):**
    * Implements a **First-Fit** strategy to find the first available free block that can accommodate the requested size.
    * If a free block is larger than the requested size, it splits the block, allocating the requested portion and leaving the remainder as a new free block.
    * Stores the allocated address within the block's metadata.
* **Memory Deallocation (`free_memory`):**
    * Marks a previously allocated block as free.
    * Clears the stored allocated address in the metadata.
* **Free Block Merging (`merge_free_blocks`):**
    * Automatically merges adjacent free blocks into a larger single free block to reduce fragmentation. This is called after every `free_memory` operation.
* **Heap Status Display (`display_heap_status`):** Provides a visual representation of the heap, showing the address, size, and allocation status of each block.
* **Interactive Menu:** A command-line interface allows users to perform allocation, deallocation, and heap status display operations.

## Concepts Used

### Custom Memory Management

This project demonstrates fundamental concepts of custom memory management, mimicking simplified versions of what functions like `malloc` and `free` do behind the scenes. It manages a fixed-size memory array as its "heap" and manually tracks free and allocated regions.

### Metadata Block (`mem_block` struct)

Each managed memory block (whether free or allocated) is preceded by a `mem_block` structure. This metadata contains crucial information about the block:
* `block_size`: The size of the usable memory area following the metadata.
* `alloc_status`: A character (`'f'` for free, `'a'` for allocated) indicating the block's current status.
* `next_block`: A pointer to the next `mem_block` in the linked list of blocks that make up the heap. This forms a linked list of all memory blocks (both free and allocated) within the heap.
* `allocated_address`: A pointer to the actual memory address returned to the user for use, which is located immediately after the `mem_block` metadata itself.

### First-Fit Allocation Strategy

When a memory allocation request comes in, the `allocate_memory` function traverses the linked list of `mem_block`s starting from `heap_start`. It searches for the *first* free block (`alloc_status == 'f'`) that is large enough to satisfy the `requested_size`.

### Memory Merging

To combat external fragmentation (where free memory is scattered in small, unusable chunks), the `merge_free_blocks` function is called after every deallocation. This function iterates through the heap and combines contiguous free blocks into a single larger free block, making them available for future allocations.

## Input Format

The program operates via an interactive command-line menu. User input is taken directly from standard input (`stdin`).

* **Menu Choices:** Users enter an integer (1-4) corresponding to their desired operation:
    * `1`: Allocate memory.
    * `2`: Free memory.
    * `3`: Display heap status.
    * `4`: Exit the program.
* **Allocation Size:** When allocating memory, the user is prompted to enter the `size_t` value (in bytes) they wish to allocate.
* **Address to Free:** When freeing memory, the user is prompted to enter the hexadecimal memory address (`%p` format) of the block they wish to free. This address should be one that was previously returned by `allocate_memory`.

## Output Format

All program output is printed to the standard output (`stdout`).

* **Initialization Messages:**
    * "memory pool created"
    * "heap start address: %p"
    * "heap end address: %p"
* **Allocation Messages:**
    * "memory allocated at address: %p" upon successful allocation.
    * Error messages like "invalid size request or not enough memory" or "out of memory: not enough space for %lu bytes" for failed allocations.
* **Deallocation Messages:**
    * "memory freed successfully at %p" upon successful freeing.
    * Error messages like "invalid pointer: address is outside the heap" or "invalid pointer: memory block not found" for invalid free attempts.
* **Heap Status Display (`display_heap_status`):**
    * Begins with a header: "heap status:", "address", "size", "status".
    * Each line represents a memory block, showing:
        * Its allocated address (actual start of data for the user).
        * Its `block_size` (the usable size).
        * Its `alloc_status` (`'f'` for free, `'a'` for allocated).
        * The `sizeof(mem_block)` (which is 24 bytes on common 64-bit systems, as `size_t` is 8 bytes, `char` is 1 byte, and two pointers are 8 bytes each, plus padding).
* **General Messages:**
    * Menu prompts.
    * "maximum allocations reached. cannot allocate more memory."
    * "no memory allocated to free"
    * "invalid choice. please try again."
    * "exiting..." when the program terminates.

## How to Compile and Run

### Prerequisites

You need a C compiler installed on your system (e.g., GCC for Linux/Windows, Clang for macOS).

### Steps

1.  **Clone or Download the Repository:**
    ```bash
    git clone [https://github.com/your-username/custom-memory-allocator.git](https://github.com/your-username/custom-memory-allocator.git)
    cd custom-memory-allocator
    ```
    (Replace `your-username/custom-memory-allocator.git` with your actual repository URL and `custom-memory-allocator` with your chosen repository name.)

2.  **Ensure `heap_management.c` is Present:**
    * Make sure the C source file (`heap_management.c`) is located in the repository directory.

3.  **Compile the Code:**
    * Open your terminal or command prompt.
    * Navigate to the directory where you cloned/downloaded the project.
    * Compile the C file (`heap_management.c`) using a command like GCC:
        ```bash
        gcc heap_management.c -o mem_allocator
        ```
        * `-o mem_allocator`: Specifies the output executable name as `mem_allocator`.

4.  **Run the Executable:**
    * Execute the compiled program from your terminal:
        ```bash
        ./mem_allocator
        ```
    * On Windows, you might need to type `mem_allocator.exe`.

5.  **Interact with the System:**
    * The program will display an interactive menu in your terminal.
    * Follow the prompts to allocate, free, and display the status of memory blocks.
