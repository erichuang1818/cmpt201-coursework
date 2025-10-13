#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct header {
  uint64_t size;
  struct header *next;
  int id;
};

void initialize_block(struct header *block, uint64_t size, struct header *next,
                      int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}

int find_first_fit(struct header *free_list_ptr, uint64_t size) {
  // TODO: Implement first fit
  struct header *current = free_list_ptr;

  while (current != NULL) {

    if (current->size >= size) {
      return current->id;
    }
    current = current->next;
  }

  return -1;
}

int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  int best_fit_id = -1;
  // TODO: Implement best fit

  uint64_t smallest_good_size = UINT64_MAX;

  struct header *current = free_list_ptr;

  while (current != NULL) {
    if (current->size >= size && current->size < smallest_good_size) {
      smallest_good_size = current->size;
      best_fit_id = current->id;
    }
    current = current->next;
  }

  return best_fit_id;
}

int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  int worst_fit_id = -1;
  // TODO: Implement worst fit
  uint64_t largest_good_size = 0;

  struct header *current = free_list_ptr;

  while (current != NULL) {
    if (current->size >= size && current->size > largest_good_size) {
      largest_good_size = current->size;
      worst_fit_id = current->id;
    }
    current = current->next;
  }

  return worst_fit_id;
}

int main(void) {

  struct header *free_block1 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block2 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block3 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block4 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block5 = (struct header *)malloc(sizeof(struct header));

  initialize_block(free_block1, 6, free_block2, 1);
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  // TODO: Print out the IDs
  printf("The ID for First-Fit algorithm is: %d\n", first_fit_id);
  printf("The ID for Best-Fit algorithm is: %d\n", best_fit_id);
  printf("The ID for Worst-Fit algorithm is: %d\n", worst_fit_id);

  free(free_block1);
  free(free_block2);
  free(free_block3);
  free(free_block4);
  free(free_block5);

  return 0;
}

/////////////////////////////////////////////////////////////////////////////
/* part 2: pseudocode for coalescing
 * 1. find position for a new_freed_block in the address
 * 2. insert newly_freed_block at the correct position
 * 3. set prev = block before the newly_freed_block
 * 4. set current = newly_freed_block
 * 5. set next = block after newly_freed_block
 * 6. coalesce w/ next block
 *    if (current_address + current.size == next_address){
 *      current.size += next.size
 *      current.next = next.next
 *      remove next from list
 *    }
 *  7. coalesce with previous block
 *    if (prev_address + prev.size == current_address){
 *      prev.size += current.size
 *      prev.next = current.next
 *      remove current from list
 *    }
 */
