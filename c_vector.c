#include "c_vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct c_vector_metadata {
  unsigned int _capacity;
  unsigned int _length;
  unsigned int _ele_size;
  free_ele _free;
  print_ele _print;
  comp_fun _cmp_fun;
};

struct c_vector {
  struct c_vector_metadata *metadata;
  char data[];
};

#define c_check_input_pointer(PTR, MSG, RET)                                   \
  if (!PTR) {                                                                  \
    fprintf(stderr, MSG "\n");                                                 \
    return RET;                                                                \
  }

#define c_check_input_index(IDX, MSG, MAX, RET)                                \
  if (index > MAX)                                                             \
    return RET;

#define c_vector_foreach(LIST, FUN)                                            \
  for (unsigned int i = 0; i < LIST->metadata->_length; i++) {                 \
    void *data = LIST->data;                                                   \
    unsigned int offset = get_offset(LIST, i);                                 \
    FUN(data + offset);                                                        \
  }

// private
inline static unsigned int get_offset(const struct c_vector *list,
                                      const unsigned int position) {
  return list->metadata->_ele_size * position;
}

static int init_metadata(struct c_vector *list,
                         const struct c_vector_input_init *args) {
  list->metadata = calloc(1, sizeof(*list->metadata));
  if (!list->metadata) {
    fprintf(stderr, "PANIC, not enough memory to alloc the list\n");
    return FAILED_RESIZING;
  }
  list->metadata->_capacity = args->capacity;
  list->metadata->_ele_size = args->ele_size;
  list->metadata->_free = args->free_fun;
  list->metadata->_print = args->print_fun;
  list->metadata->_cmp_fun = args->comp_fun;

  return EXIT_SUCCESS;
}

static int resize_list(struct c_vector **list) {
  char *data = NULL;
  unsigned int old_capacity = (*list)->metadata->_capacity;
  unsigned int new_capacity = old_capacity * 2;
  struct c_vector *new_list = realloc(
      (*list), sizeof(**list) + (new_capacity * (*list)->metadata->_ele_size));
  if (!new_list) {
    fprintf(stderr, "PANIC, not enough memory to alloc the list\n");
    return FAILED_RESIZING;
  }
  new_list->metadata->_capacity = new_capacity;
  *list = new_list;
  data = (*list)->data;
  memset(&data[old_capacity * new_list->metadata->_ele_size], 0,
         (new_capacity - old_capacity) * new_list->metadata->_ele_size);

  return EXIT_SUCCESS;
}

static int delete_shift(struct c_vector *list, unsigned int start_index) {
  unsigned int offset_i = get_offset(list, start_index);
  unsigned int offset_j = 0;
  char *data = list->data;
  for (unsigned int j = start_index + 1; j < list->metadata->_length; j++) {
    offset_j = get_offset(list, j);
    memcpy(&data[offset_i], &data[offset_j], list->metadata->_ele_size);
    start_index++;
    offset_i = get_offset(list, start_index);
  }
  return EXIT_SUCCESS;
}

static void *get_element(struct c_vector *list, const void *key) {
  char *data = list->data;
  unsigned int offset = 0;

  for (unsigned int i = 0; i < list->metadata->_length; i++) {
    offset = get_offset(list, i);
    if (list->metadata->_cmp_fun) {
      if (!list->metadata->_cmp_fun(&data[offset], key)) {
        return &data[offset];
      }
    } else if (!memcmp(&data[offset], key, list->metadata->_ele_size)) {
      return &data[offset];
    }
  }
  return NULL;
}

// public
c_vector_h c_vector_init(const struct c_vector_input_init *input_args) {
  c_check_input_pointer(input_args, "input args init", NULL);
  c_check_input_pointer(input_args->free_fun, "free_fun", NULL);
  c_check_input_pointer(input_args->print_fun, "print_fun", NULL);

  unsigned int ele_size = input_args->ele_size;
  int capacity = input_args->capacity;
  if (!ele_size) {
    fprintf(stderr, "FAILED: invalid element size, at least > 0, given %d\n",
            ele_size);
    return NULL;
  }

  unsigned int vec_cap = capacity < 0 ? DEFAULT_CAPACITY : capacity;
  struct c_vector *new_vector =
      calloc(1, sizeof(*new_vector) + (vec_cap * ele_size) + 100);

  if (!new_vector) {
    fprintf(stderr, "PANIC, not enough memory to alloc the list\n");
    return NULL;
  }

  init_metadata(new_vector, input_args);

  return new_vector;
}

const void *c_vector_push(c_vector_h *list, const void *ele) {
  c_check_input_pointer(list, "vector root pointer", NULL);
  struct c_vector *list_a = *list;
  c_check_input_pointer(*list, "vector pointer", NULL);
  c_check_input_pointer(list, "vector element to push", NULL);

  if (list_a->metadata->_length == list_a->metadata->_capacity) {
    resize_list(&list_a);
    *list = list_a;
  }

  unsigned int offset = get_offset(list_a, list_a->metadata->_length);
  char *data = list_a->data;
  memcpy(&data[offset], ele, list_a->metadata->_ele_size);
  list_a->metadata->_length++;

  return &data[offset];
}

int c_vector_insert_in(c_vector_h *list, const void *ele,
                       const unsigned int index) {
  c_check_input_pointer(list, "vector pointer", EXIT_FAILURE);
  struct c_vector *list_a = *list;
  c_check_input_pointer(ele, "vector element to insert", EXIT_FAILURE);

  if (index > list_a->metadata->_capacity)
    resize_list(&list_a);

  unsigned int offset = (index * list_a->metadata->_ele_size);
  char *data = list_a->data;
  memcpy(&data[offset], ele, list_a->metadata->_ele_size);
  return EXIT_SUCCESS;
}

void *c_vector_find(c_vector_h list, const void *ele) {
  c_check_input_pointer(list, "vector pointer", NULL);
  c_check_input_pointer(ele, "vector element to find", NULL);
  return get_element(list, ele);
}

void *c_vector_get_at_index(c_vector_h list, const unsigned int index) {
  c_check_input_pointer(list, "vector pointer", NULL);
  struct c_vector *list_a = list;
  c_check_input_index(index, "vector length", list_a->metadata->_length, NULL);

  char *data = list_a->data;
  unsigned int offset = get_offset(list_a, index);

  return &data[offset];
}

int c_vector_delete_ele(c_vector_h list, const void *ele) {
  struct c_vector *list_a = list;
  c_check_input_pointer(list_a, "vector pointer", EXIT_FAILURE);
  c_check_input_pointer(ele, "vector element to delete", EXIT_FAILURE);

  unsigned int i = 0;
  char *data = list_a->data;
  unsigned int offset_i = 0;

  for (; i < list_a->metadata->_length; i++) {
    offset_i = get_offset(list_a, i);
    if (get_element(list_a, ele)) {
      list_a->metadata->_free(&data[offset_i]);
      memset(&data[offset_i], 0, list_a->metadata->_ele_size);
      delete_shift(list, i);
      list_a->metadata->_length--;
      return EXIT_SUCCESS;
    }
  }
  return EXIT_FAILURE;
}

int c_vector_delete_ele_at_index(c_vector_h list, const unsigned int index) {
  c_check_input_pointer(list, "vector pointer", EXIT_FAILURE);
  struct c_vector *list_a = list;
  c_check_input_index(index, "vector length", list_a->metadata->_length,
                      EXIT_FAILURE);

  char *data = list_a->data;
  unsigned int offset = get_offset(list, index);
  list_a->metadata->_free(&data[offset]);
  memset(&data[offset], 0, list_a->metadata->_ele_size);
  delete_shift(list, index);
  list_a->metadata->_length--;

  return EXIT_SUCCESS;
}

int c_vector_free(c_vector_h list) {
  struct c_vector *list_a = list;
  c_check_input_pointer(list_a, "vector pointer", EXIT_FAILURE);
  c_vector_foreach(list_a, list_a->metadata->_free);
  free(list_a->metadata);
  free(list);
  return EXIT_SUCCESS;
}

void c_vector_to_string(c_vector_h list) {
  struct c_vector *list_a = list;
  c_check_input_pointer(list, "vector pointer", );
  c_vector_foreach(list_a, list_a->metadata->_print);
}

unsigned int c_vector_length(const c_vector_h list) {
  struct c_vector *list_a = list;
  c_check_input_pointer(list_a, "vector pointer", EXIT_FAILURE);

  return list_a->metadata->_length;
}

unsigned int c_vector_capacity(const c_vector_h list) {
  struct c_vector *list_a = list;
  c_check_input_pointer(list_a, "vector pointer", EXIT_FAILURE);

  return list_a->metadata->_capacity;
}

unsigned int c_vector_ele_size(const c_vector_h list) {
  struct c_vector *list_a = list;
  c_check_input_pointer(list_a, "vector pointer", EXIT_FAILURE);

  return list_a->metadata->_ele_size;
}

unsigned int c_vector_clear(const c_vector_h list) {
  struct c_vector *list_a = list;
  c_check_input_pointer(list_a, "vector pointer", EXIT_FAILURE);
  list_a->metadata->_length = 0;

  return EXIT_SUCCESS;
}
