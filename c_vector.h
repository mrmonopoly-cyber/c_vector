#ifndef __C_VECTOR__
#define __C_VECTOR__

#include <stdint.h>

typedef void *c_vector_h;
typedef int (*comp_fun)(const void *ele_list, const void *key);
typedef void (*free_ele)(void *ele);
typedef void (*print_ele)(const void *ele);

#define DEFAULT_CAPACITY 10

enum C_VECTOR_ERRORS {
  NULL_REFERENCE_LIST = -1,
  NULL_REFERENCE_ELEMENT = -2,
  INVALID_INDEX_VALUE = -3,
  FAILED_RESIZING = -4,

};

struct c_vector_input_init {
  uint8_t capacity;
  uint8_t ele_size;
  comp_fun comp_fun;
  print_ele print_fun;
  free_ele free_fun;
};

c_vector_h c_vector_init(const struct c_vector_input_init *input_args);

uint8_t c_vector_free(c_vector_h list);

uint8_t c_vector_insert_in(c_vector_h *list, const void *ele,
                           const unsigned int index);

const void *c_vector_push(c_vector_h *list, const void *ele);

void *c_vector_find(c_vector_h list, const void *ele);

void *c_vector_get_at_index(c_vector_h list, const unsigned int index);

uint8_t c_vector_delete_ele(c_vector_h list, const void *ele);

uint8_t c_vector_delete_ele_at_index(c_vector_h list, const unsigned int index);

void c_vector_to_string(c_vector_h list);

uint8_t c_vector_length(const c_vector_h list);
uint8_t c_vector_capacity(const c_vector_h list);
uint8_t c_vector_ele_size(const c_vector_h list);

uint8_t c_vector_clear(const c_vector_h list);

#endif //!__C_VECTOR__
