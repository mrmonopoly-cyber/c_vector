#include "../c_vector.h" // Include your library header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Example comparison function for integers
int compare_ints(const void *a, const void *b) { return *(int *)a - *(int *)b; }

// Example free function for integers
void free_int(void *a) {
  // No dynamic memory allocated, so nothing to free
}

// Example print function for integers
void print_int(const void *a) { printf("%d\n", *(int *)a); }

// Test the initialization of the vector
void test_init() {
  struct c_vector_input_init input_args = {0};
  input_args.capacity = 5;
  input_args.ele_size = sizeof(int);
  input_args.free_fun = free_int;
  input_args.print_fun = print_int;
  input_args.comp_fun = compare_ints;

  c_vector_h *vec = c_vector_init(&input_args);
  if (vec == NULL) {
    printf("Initialization failed!\n");
    return;
  }

  printf("Initialization passed!\n");
  c_vector_free(vec);
}

// Test insertion and retrieval of elements
void test_insert_and_get() {
  struct c_vector_input_init input_args = {0};
  input_args.capacity = 5;
  input_args.ele_size = sizeof(int);
  input_args.free_fun = free_int;
  input_args.print_fun = print_int;
  input_args.comp_fun = compare_ints;

  c_vector_h *vec = c_vector_init(&input_args);

  int value = 10;
  c_vector_push(&vec, &value);

  int *retrieved = (int *)c_vector_get_at_index(vec, 0);
  if (retrieved && *retrieved == value) {
    printf("Insertion and retrieval passed!\n");
  } else {
    printf("Insertion and retrieval failed!\n");
  }

  c_vector_free(vec);
}

// Test deletion of elements
void test_delete() {
  struct c_vector_input_init input_args = {0};
  input_args.capacity = 5;
  input_args.ele_size = sizeof(int);
  input_args.free_fun = free_int;
  input_args.print_fun = print_int;
  input_args.comp_fun = compare_ints;

  c_vector_h *vec = c_vector_init(&input_args);

  int value1 = 10, value2 = 20, value3 = 30;
  c_vector_push(&vec, &value1);
  c_vector_push(&vec, &value2);
  c_vector_push(&vec, &value3);
  printf("================================\n");

  c_vector_to_string(vec);

  printf("================================\n");
  c_vector_delete_ele(vec, &value1);
  c_vector_to_string(vec);
  printf("================================\n");
  int *retrieved = (int *)c_vector_get_at_index(vec, 0);
  if (retrieved && *retrieved == value2) {
    printf("Deletion passed!\n");
  } else {
    printf("Deletion failed!\n");
  }

  c_vector_free(vec);
}

// Test vector resizing
void test_resize() {
  struct c_vector_input_init input_args = {0};
  input_args.capacity = 2;
  input_args.ele_size = sizeof(int);
  input_args.free_fun = free_int;
  input_args.print_fun = print_int;
  input_args.comp_fun = compare_ints;

  c_vector_h *vec = c_vector_init(&input_args);

  int value1 = 10, value2 = 20, value3 = 30;
  c_vector_push(&vec, &value1);
  c_vector_push(&vec, &value2);
  c_vector_push(&vec, &value3); // Should trigger resize

  c_vector_to_string(vec);

  if (c_vector_capacity(vec) > 2) {
    printf("Resize passed!\n");
  } else {
    printf("Resize failed!\n");
  }

  c_vector_free(vec);
}

// Test finding elements
void test_find() {
  struct c_vector_input_init input_args = {0};
  input_args.capacity = 5;
  input_args.ele_size = sizeof(int);
  input_args.free_fun = free_int;
  input_args.print_fun = print_int;
  input_args.comp_fun = compare_ints;

  c_vector_h *vec = c_vector_init(&input_args);

  int value1 = 10, value2 = 20;
  c_vector_push(&vec, &value1);
  c_vector_push(&vec, &value2);

  int *found = (int *)c_vector_find(vec, &value2);
  if (found && *found == value2) {
    printf("Find passed!\n");
  } else {
    printf("Find failed!\n");
  }

  c_vector_free(vec);
}

// Test clearing the vector
void test_clear() {
  struct c_vector_input_init input_args = {0};
  input_args.capacity = 5;
  input_args.ele_size = sizeof(int);
  input_args.free_fun = free_int;
  input_args.print_fun = print_int;
  input_args.comp_fun = compare_ints;

  c_vector_h *vec = c_vector_init(&input_args);

  int value1 = 10, value2 = 20;
  c_vector_push(&vec, &value1);
  c_vector_push(&vec, &value2);

  c_vector_clear(vec);
  if (c_vector_length(vec) == 0) {
    printf("Clear passed!\n");
  } else {
    printf("Clear failed!\n");
  }

  c_vector_free(vec);
}

// Main function to run all tests
int main() {
  test_init();
  test_insert_and_get();
  test_delete();
  test_resize();
  test_find();
  test_clear();

  return 0;
}
