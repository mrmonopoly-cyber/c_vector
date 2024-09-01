#pragma once

typedef struct c_vector c_vector;
typedef int (*comp_fun)(const void* list_ele, const void* key);
typedef void (*free_ele)(const void* ele);
typedef void (*print_ele)(const void* ele);

#define DEFAULT_CAPACITY 10


enum ERRORS{
    NULL_REFERENCE_LIST = -1,
    NULL_REFERENCE_ELEMENT = -2,
    INVALID_INDEX_VALUE = -3,
    FAILED_RESIZING = -4,

};

struct c_vector_input_init{
    int capacity;
    comp_fun found_f;
    unsigned int ele_size;
    print_ele print_fun;
    free_ele free_fun;
};

int 
c_vector_init(const struct c_vector_input_init* input_args, c_vector** o_res);

int 
c_vector_free(c_vector* list);

int 
c_vector_insert_in(c_vector** list, const void* ele, const unsigned int index);

int
c_vector_push(c_vector** list, const void* ele, void** o_res);

int
c_vector_find(const c_vector* list, const void* ele, const void* o_result);

int
c_vector_get_at_index(const c_vector* list, const unsigned int index, const void** o_res);

int
c_vector_delete_ele(c_vector* list, const void *ele);

int
c_vector_delete_ele_at_index(c_vector* list, const unsigned int index);

void c_vector_to_string(const c_vector* list);

unsigned int c_vector_length(const c_vector* list);
unsigned int c_vector_capacity(const c_vector* list);
unsigned int c_vector_ele_size(const c_vector* list);
