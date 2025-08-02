#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define heap_size 1000000  // 1 mb heap size
#define max_allocations 100 // maximum number of allocations

char main_memory[heap_size]; 

typedef struct metadata
{
    size_t block_size;   // size of the block
    char alloc_status;   // 'f' for free, 'a' for allocated
    struct metadata *next_block;
    void *allocated_address; // store allocated address
} mem_block;

mem_block *heap_start = (void *)main_memory;

void initialize_heap()
{
    heap_start->block_size = heap_size - sizeof(mem_block);
    heap_start->alloc_status = 'f';
    heap_start->next_block = NULL;
    heap_start->allocated_address = NULL;
    printf("memory pool created\n");
    printf("heap start address: %p\n", heap_start);
    printf("heap end address: %p\n", main_memory + heap_size - 1);
}

void *allocate_memory(size_t requested_size)
{
    mem_block *current_block = heap_start;
    void *return_ptr = NULL;

    if (requested_size <= 0 || requested_size > heap_size - sizeof(mem_block))
    {
        printf("invalid size request or not enough memory\n");
        return NULL;
    }

    while (current_block != NULL && (current_block->block_size < requested_size || current_block->alloc_status == 'a'))
    {
        current_block = current_block->next_block;
    }

    if (current_block == NULL)
    {
        printf("out of memory: not enough space for %lu bytes\n", (unsigned long)requested_size);
        return NULL;
    }

    if (current_block->block_size == requested_size)
    {
        current_block->alloc_status = 'a';
        return_ptr = (void *)((char *)current_block + sizeof(mem_block));
        current_block->allocated_address = return_ptr; // store address
    }
    else
    {
        mem_block *new_block = (void *)((char *)current_block + sizeof(mem_block) + requested_size);
        new_block->block_size = current_block->block_size - requested_size - sizeof(mem_block);
        new_block->next_block = current_block->next_block;
        new_block->alloc_status = 'f';
        new_block->allocated_address = NULL;

        current_block->block_size = requested_size;
        current_block->next_block = new_block;
        current_block->alloc_status = 'a';
        return_ptr = (void *)((char *)current_block + sizeof(mem_block));
        current_block->allocated_address = return_ptr; // store address
    }

    return return_ptr;
}

void merge_free_blocks()
{
    mem_block *current_block = heap_start;

    while (current_block->next_block != NULL)
    {
        if (current_block->alloc_status == 'f' && current_block->next_block->alloc_status == 'f')
        {
            current_block->block_size += current_block->next_block->block_size + sizeof(mem_block);
            current_block->next_block = current_block->next_block->next_block;
        }
        else
        {
            current_block = current_block->next_block;
        }
    }
}

void free_memory(void *ptr_to_free)
{
    if (ptr_to_free < (void *)main_memory || ptr_to_free >= (void *)(main_memory + heap_size))
    {
        printf("invalid pointer: address is outside the heap\n");
        return;
    }

    mem_block *current_block = heap_start;

    while (current_block != NULL)
    {
        if (current_block->allocated_address == ptr_to_free)
        {
            current_block->alloc_status = 'f';
            current_block->allocated_address = NULL; // clear stored address
            merge_free_blocks();
            printf("memory freed successfully at %p\n", ptr_to_free);
            return;
        }

        current_block = current_block->next_block;
    }

    printf("invalid pointer: memory block not found\n");
}

void display_heap_status()
{
    mem_block *current_block = heap_start;

    printf("\nheap status:\n");
    printf("address\t\tsize\tstatus\n");

    while (current_block != NULL)
    {
        printf("%p\t%lu\t%c\t%d\n",
               (void *)current_block->allocated_address,
               (unsigned long)current_block->block_size,
               current_block->alloc_status,sizeof(mem_block));
        current_block = current_block->next_block;
    }
}

int main()
{
    initialize_heap();

    int user_choice;
    void *allocated_blocks[max_allocations]; // array to track allocated blocks
    int num_allocated = 0;                   // number of currently allocated blocks
    size_t requested_size;

    while (1)
    {
        printf("\n1. allocate memory\n");
        printf("2. free memory\n");
        printf("3. display heap\n");
        printf("4. exit\n");
        printf("enter your choice: ");
        scanf("%d", &user_choice);

        switch (user_choice)
        {
        case 1:
            if (num_allocated >= max_allocations)
            {
                printf("maximum allocations reached. cannot allocate more memory.\n");
                break;
            }

            printf("enter size to allocate (in bytes): ");
            scanf("%lu", (unsigned long *)&requested_size);
            allocated_blocks[num_allocated] = allocate_memory(requested_size);

            if (allocated_blocks[num_allocated] != NULL)
            {
                num_allocated++;
                printf("memory allocated at address: %p\n", allocated_blocks[num_allocated - 1]);
            }
            break;

        case 2:
            if (num_allocated == 0)
            {
                printf("no memory allocated to free\n");
                break;
            }

            printf("enter the address of the block to free: ");
            void *ptr_to_free;
            scanf("%p", &ptr_to_free);
            int block_found = 0;

            for (int i = 0; i < num_allocated; i++)
            {
                if (allocated_blocks[i] == ptr_to_free)
                {
                    free_memory(allocated_blocks[i]);

                    for (int j = i; j < num_allocated - 1; j++)
                    {
                        allocated_blocks[j] = allocated_blocks[j + 1];
                    }

                    num_allocated--;
                    block_found = 1;
                    break;
                }
            }

            if (!block_found)
            {
                printf("no block found at address %p\n", ptr_to_free);
            }
            break;

        case 3:
            display_heap_status();
            break;

        case 4:
            printf("exiting...\n");
            exit(0); 

        default:
            printf("invalid choice. please try again.\n");
        }
    }

    return 0;
}

