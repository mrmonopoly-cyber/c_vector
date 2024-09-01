#include "c_vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct c_vector_metadata {
  unsigned int _capacity;
  unsigned int _length;
  unsigned int _ele_size;
  comp_fun _cmp;
  free_ele _free;
  print_ele _print;
};

struct c_vector {
  struct c_vector_metadata *metadata;
  char data[];
};

#define c_check_input_pointer(PTR,MSG) \
    if (!PTR) {printf("invalid input: "MSG); return EXIT_FAILURE;}

#define c_vector_foreach(LIST, FUN)                                            \
  for (unsigned int i = 0; i < LIST->metadata->_length; i++) {                 \
    const void *data = LIST->data;                                                   \
    unsigned int offset = get_offset(LIST, i);                                 \
    FUN(data + offset);                                                        \
  }

// private
inline static unsigned int get_offset(const c_vector *list,
                                      const unsigned int position) {
  return list->metadata->_ele_size * position;
}

static int init_metadata(c_vector *list,
                         const struct c_vector_input_init *args) {
  list->metadata = calloc(1, sizeof(*list->metadata));
  if (!list->metadata) {
    fprintf(stderr, "PANIC, not enough memory to alloc the list\n");
    return FAILED_RESIZING;
  }
  list->metadata->_capacity = args->capacity;
  list->metadata->_ele_size = args->ele_size;
  list->metadata->_cmp = args->found_f;
  list->metadata->_free = args->free_fun;
  list->metadata->_print = args->print_fun;

  return EXIT_SUCCESS;
}

static int resize_list(c_vector **list) {
  void *data = NULL;
  unsigned int old_capacity = (*list)->metadata->_capacity;
  unsigned int new_capacity = old_capacity * 2;
  c_vector *new_list = realloc(
      (*list), sizeof(**list) + (new_capacity * (*list)->metadata->_ele_size));
  if (!new_list) {
    fprintf(stderr, "PANIC, not enough memory to alloc the list\n");
    return FAILED_RESIZING;
  }
  new_list->metadata->_capacity = new_capacity;
  *list = new_list;
  data = (*list)->data;
  memset(data + (old_capacity * new_list->metadata->_ele_size), 0,
         (new_capacity - old_capacity) * new_list->metadata->_ele_size);

  return EXIT_SUCCESS;
}

// public
int
c_vector_init(const struct c_vector_input_init* input_args, c_vector** o_res)
{
  unsigned int ele_size = input_args->ele_size;
  int capacity = input_args->capacity;
  if (!ele_size) {
    fprintf(stderr, "FAILED: invalid element size, at least > 0, given %d\n",
            ele_size);
    return EXIT_FAILURE;
  }

  c_check_input_pointer(input_args->found_f, "found_f");
  c_check_input_pointer(input_args->free_fun, "free_fun");
  c_check_input_pointer(input_args->print_fun, "print_fun");

  unsigned int vec_cap = capacity < 0 ? DEFAULT_CAPACITY : capacity;
  c_vector *new_vector =
      calloc(1, sizeof(*new_vector) + (vec_cap * ele_size) + 100);

  if (!new_vector) {
    fprintf(stderr, "PANIC, not enough memory to alloc the list\n");
    return EXIT_FAILURE;
  }

  init_metadata(new_vector, input_args);

  (*o_res) = new_vector;

  return EXIT_SUCCESS;
}

int c_vector_push(c_vector** list, const void* ele, void** o_res)
{
  c_check_input_pointer(list, "vector root pointer");
  c_check_input_pointer(list, "vector pointer");
  c_check_input_pointer(list, "vector element to push");

  if ((*list)->metadata->_length == (*list)->metadata->_capacity) {
    resize_list(list);
  }

  unsigned int offset = get_offset(*list, (*list)->metadata->_length);
  void *data = (*list)->data;
  memcpy(data + offset, ele, (*list)->metadata->_ele_size);
  (*list)->metadata->_length++;

  if (o_res) {
      (*o_res) = data + offset;
  }
  return EXIT_SUCCESS;
}

int c_vector_insert_in(c_vector **list, const void *ele,
                       const unsigned int index) {
  c_check_input_pointer(list, "vector pointer");
  c_check_input_pointer(list, "vector element to insert");

  if (index > (*list)->metadata->_capacity)
    resize_list(list);

  unsigned int offset = (index * (*list)->metadata->_ele_size);
  void *data = (*list)->data;
  memcpy(data + offset, ele, (*list)->metadata->_ele_size);
  return EXIT_SUCCESS;
}

int c_vector_find(const c_vector *list, const void *ele, const void **o_result) {
  c_check_input_pointer(list, "vector pointer");
  c_check_input_pointer(list, "vector element to find");
  c_check_input_pointer(o_result, "output ptr");

  o_result = NULL;
  const void *data = list->data;
  unsigned int ele_size = list->metadata->_ele_size;
  const void *list_ele = 0;

  for (unsigned int i = 0; i < list->metadata->_length; i++) {
    list_ele = data + (i * ele_size);
    if (list->metadata->_cmp(list_ele, ele)) {
      *o_result = list_ele;
    }
  }
  return EXIT_SUCCESS;
}

int
c_vector_get_at_index(const c_vector* list, const unsigned int index, const void** o_res)
{
  c_check_input_pointer(list, "vector pointer");
  c_check_input_pointer(o_res, "out ptr");

  if (list->metadata->_length < index) {
    printf("invalid input: vector length\n");
    return EXIT_FAILURE;
  }

  const void *data = list->data;
  unsigned int offset = get_offset(list, index);

  *o_res = data + offset;
  return EXIT_SUCCESS;
}

int c_vector_delete_ele(c_vector *list, const void *ele) {
  c_check_input_pointer(list, "vector pointer");
  c_check_input_pointer(ele, "vector element to delete");

  unsigned int i = 0;
  void *data = list->data;
  unsigned int offset = 0;

  for (; i < list->metadata->_length; i++) {
    offset = get_offset(list, i);
    if (list->metadata->_cmp(data + offset, ele)) {
      list->metadata->_free(data + offset);
      memset(data + offset, 0, list->metadata->_ele_size);
      list->metadata->_length--;
      return EXIT_SUCCESS;
    }
  }
  return EXIT_SUCCESS;
}

int c_vector_delete_ele_at_index(c_vector *list, const unsigned int index) {
  c_check_input_pointer(list, "vector pointer");

  if (list->metadata->_length < index) {
    printf("invalid input: vector length\n");
    return EXIT_FAILURE;
  }

  void *data = list->data;
  unsigned int offset = get_offset(list, index);
  list->metadata->_free(data + offset);
  memset(data + offset, 0, list->metadata->_ele_size);

  return EXIT_SUCCESS;
}

int c_vector_free(c_vector *list) {
  c_check_input_pointer(list, "vector pointer");

  c_vector_foreach(list, list->metadata->_free);
  free(list->metadata);
  free(list);
  return EXIT_SUCCESS;
}

void c_vector_to_string(const c_vector *list) {
  c_vector_foreach(list, list->metadata->_print);
}

void c_vector_clear(c_vector *list)
{
    list->metadata->_length = 0;
}

unsigned int c_vector_length(const c_vector *list) {
  c_check_input_pointer(list, "vector pointer");
    return NULL_REFERENCE_LIST;

  return list->metadata->_length;
}

unsigned int c_vector_capacity(const c_vector *list) {
  c_check_input_pointer(list, "vector pointer");

  return list->metadata->_capacity;
}

unsigned int c_vector_ele_size(const c_vector *list) {
  c_check_input_pointer(list, "vector pointer");

  return list->metadata->_ele_size;
}
