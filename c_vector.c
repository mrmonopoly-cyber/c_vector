#include "c_vector.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct c_vector_metadata{
    unsigned int _capacity;
    unsigned int _length;
    unsigned int _ele_size;
    comp_fun _cmp;
    free_ele _free;
    print_ele _print;
};

struct c_vector{
    struct c_vector_metadata* metadata;
    /*
     * DATA:
     * ...
     * ...
     * ...
     */
};

#define c_vector_foreach(LIST, FUN) \
                        for(unsigned int i=0; i < LIST->metadata->_length;i++)\
                        { \
                            void* data = get_data(LIST); \
                            unsigned int offset = get_offset(LIST,i);\
                            FUN(data + offset);\
                        }

//private 


inline static void*
get_data(const c_vector* list){
    return &list[1];
}

inline static unsigned int
get_offset(const c_vector* list, const unsigned int position){
    return list->metadata->_ele_size * position;
}

inline static int
input_check_index(const c_vector* list, const unsigned int index){
    if (index >= list->metadata->_length) {
        fprintf(stderr, "ERROR: invalid index, given %d with maximum possible %d\n", 
                index, list->metadata->_length);
        return 0;
    }
    return 1;
}

inline static int
input_valid_pointer(const void *function, const char* function_name){
    if (!function) {
        fprintf(stderr, "FAILED: invalid pointer for %s, given NULL", function_name);
        return 0;
    }
    return 1;
}

static int 
init_metadata(c_vector* list, const struct c_vector_input_init* args){
    list->metadata = calloc(1, sizeof(*list->metadata));
    if (!list->metadata) {
        fprintf(stderr, "PANIC, not enough memory to alloc the list\n");
        return FAILED_RESIZING;
    }
    list->metadata->_capacity=args->capacity;
    list->metadata->_ele_size=args->ele_size;
    list->metadata->_cmp=args->found_f;
    list->metadata->_free=args->free_fun;
    list->metadata->_print=args->print_fun;

    return EXIT_SUCCESS;
}

static int 
resize_list(c_vector** list)
{
        void* data = NULL;
        unsigned int old_capacity = (*list)->metadata->_capacity;
        unsigned int new_capacity = old_capacity * 2;
        c_vector* new_list = realloc((*list), sizeof(*(*list)) + (new_capacity * (*list)->metadata->_ele_size));
        if (!new_list) {
            fprintf(stderr, "PANIC, not enough memory to alloc the list\n");
            return FAILED_RESIZING;
        }
        new_list->metadata->_capacity = new_capacity;
        *list = new_list;
        data = get_data((*list));
        memset(data + old_capacity, 0, new_capacity - old_capacity);

        return EXIT_SUCCESS;
}

//public
c_vector* 
c_vector_init(const struct c_vector_input_init* input_args)
{
    unsigned int ele_size = input_args->ele_size;
    int capacity = input_args->capacity;
    if(!ele_size){
        fprintf(stderr, "FAILED: invalid element size, at least > 0, given %d\n", ele_size);
        return NULL;
    }

    if (!input_valid_pointer(input_args->found_f, "found_f")) return NULL;
    if (!input_valid_pointer(input_args->free_fun, "free_fun")) return NULL;
    if (!input_valid_pointer(input_args->print_fun, "print_fun")) return NULL;

    unsigned int vec_cap = capacity < 0 ? DEFAULT_CAPACITY : capacity;
    c_vector* new_vector = calloc(1,sizeof(*new_vector)+ (vec_cap*ele_size) + 100);

    if (!new_vector) {
        fprintf(stderr, "PANIC, not enough memory to alloc the list\n");
        return NULL;
    }

    init_metadata(new_vector, input_args);

    return new_vector;
}

const void*
c_vector_push(c_vector** list, const void* ele)
{
    if (!input_valid_pointer(list,"list")) return NULL;
    if (!input_valid_pointer((*list),"(*list)")) return NULL;
    if (!input_valid_pointer(ele,"ele")) return NULL;
    
    if ((*list)->metadata->_length == (*list)->metadata->_capacity) {
        resize_list(list);
    }
    
    unsigned int offset = get_offset(*list, (*list)->metadata->_length);
    void* data = get_data((*list));
    memcpy(data + offset, ele, (*list)->metadata->_ele_size);
    (*list)->metadata->_length++;

    return data + offset;
}

int 
c_vector_insert_in(c_vector** list, const void* ele, const unsigned int index)
{
    if (!input_valid_pointer(list,"list")) return NULL_REFERENCE_LIST;
    if (!input_valid_pointer(ele,"ele")) return NULL_REFERENCE_ELEMENT;
    
    if (index > (*list)->metadata->_capacity) {
        resize_list(list);
    }
    
    unsigned int offset = (index * (*list)->metadata->_ele_size);
    void* data = get_data(*list);
    memcpy(data + offset, ele, (*list)->metadata->_ele_size);
    return EXIT_SUCCESS;
}

int
c_vector_find(const c_vector* list, const void* ele, void* o_result)
{
    if (!input_valid_pointer(list,"list")) return NULL_REFERENCE_LIST;
    if (!input_valid_pointer(ele,"ele")) return NULL_REFERENCE_ELEMENT;

    o_result=NULL;
    void *data = get_data(list);
    unsigned int ele_size = list->metadata->_ele_size;
    void *list_ele = 0;

    for (unsigned int i=0; i<list->metadata->_length; i++) {
        list_ele = data + (i*ele_size);
        if (list->metadata->_cmp(list_ele, ele)) {
            o_result = list_ele;
        }
    }
    return EXIT_SUCCESS;
}

void* 
c_vector_get_at_index(const c_vector* list, const unsigned int index)
{
    if (!input_valid_pointer(list,"list")) return NULL;
    if (!input_check_index(list, index)) return NULL;
    
    void *data = get_data(list);
    unsigned int offset = get_offset(list, index);

    return data + offset;
}

int
c_vector_delete_ele(c_vector* list, const void *ele)
{
    if (!input_valid_pointer(list,"list")) return NULL_REFERENCE_LIST;
    if (!input_valid_pointer(ele,"ele")) return NULL_REFERENCE_ELEMENT;
    
    unsigned int i =0;
    void *data = get_data(list);
    unsigned int offset = 0;

    for (; i<list->metadata->_length; i++) {
        offset = get_offset(list, i);
        if (list->metadata->_cmp(data + offset, ele)) {
            list->metadata->_free(data + offset);
            memset(data + offset,0, list->metadata->_ele_size);
            list->metadata->_length--;
            return EXIT_SUCCESS;
        }
    }
    return EXIT_SUCCESS;
}

int
c_vector_delete_ele_at_index(c_vector* list, const unsigned int index)
{
    if (!input_valid_pointer(list, "list")) return NULL_REFERENCE_LIST;
    if (!input_check_index(list, index)) return INVALID_INDEX_VALUE;

    void* data = get_data(list);
    unsigned int offset = get_offset(list, index);
    list->metadata->_free(data + offset);
    memset(data+offset, 0, list->metadata->_ele_size);

    return EXIT_SUCCESS;
}

int 
c_vector_free(c_vector* list){
    if (!input_valid_pointer(list,"list")) return NULL_REFERENCE_LIST; 
    c_vector_foreach(list, list->metadata->_free);
    free(list->metadata);
    free(list);
    return EXIT_SUCCESS;
}

void c_vector_to_string(const c_vector* list){
    c_vector_foreach(list, list->metadata->_print);
}

unsigned int c_vector_length(const c_vector *list){
    if (!input_valid_pointer(list, "list")) return NULL_REFERENCE_LIST;
        
    return list->metadata->_length;
}

unsigned int c_vector_capacity(const c_vector* list){
    if (!input_valid_pointer(list, "list")) return NULL_REFERENCE_LIST;
        
    return list->metadata->_capacity;
}
