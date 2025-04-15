#ifndef __C_VECTOR__
#define __C_VECTOR__

#include <stdint.h>

/* Forward declaration of the vector handle */
typedef void c_vector_h;
typedef uint16_t EleSizeUnit_t;

/* Function pointer types for customizing element operations */

/**
 * Comparison function to find an element.
 * @param ele_list A pointer to an element from the list.
 * @param key A pointer to the key to compare the element with.
 * @return 0 if they are equal, non-zero otherwise.
 */
typedef int (*comp_fun)(const void *ele_list, const void *key);

/**
 * Function to free a dynamically allocated element.
 * @param ele A pointer to the element to be freed.
 */
typedef void (*free_ele)(void *ele);

/**
 * Function to print an element.
 * @param ele A pointer to the element to be printed.
 */
typedef void (*print_ele)(const void *ele);

/**
 * Function to populate or construct an object in place.
 * @param obj_ele A pointer to the element to be populated.
 * @param args A pointer to arguments used for populating the element.
 */
typedef void (*object_populate)(void* obj_ele, const void *args);

/* Default capacity of the vector */
#define DEFAULT_CAPACITY 10

/* Error codes for various operations in the vector */
enum C_VECTOR_ERRORS {
    NULL_REFERENCE_LIST = -1,   /* Null reference to the list */
    NULL_REFERENCE_ELEMENT = -2,/* Null reference to an element */
    INVALID_INDEX_VALUE = -3,   /* Invalid index passed to the function */
    FAILED_RESIZING = -4,       /* Resizing the vector's capacity failed */
};

/**
 * Structure used to initialize the vector.
 * Contains customizable options such as comparison, printing, freeing, and populating functions.
 */
struct c_vector_input_init {
    uint8_t capacity;           /* Initial capacity of the vector */
    EleSizeUnit_t ele_size;           /* Size of each element in the vector */
    comp_fun comp_fun;          /* Pointer to the comparison function, if NULL a bit by bit comparison will be used */
    print_ele print_fun;        /* Pointer to the function to print elements, if NULL c_vector_print will do nothing */
    free_ele free_fun;          /* Pointer to the function to free elements, only pass this function if you have nested pointer to free */
    object_populate populate_fun; /* Pointer to the function to populate an element, if NULL c_vector_emplace_back will do nothing */
};

/**
 * Initializes a new vector with the given input arguments.
 * @param input_args A pointer to a struct defining initialization parameters.
 * @return A handle to the newly created vector, or NULL on failure.
 */
c_vector_h *c_vector_init(const struct c_vector_input_init *input_args);

/**
 * Frees the memory occupied by the vector and its elements.
 * @param list A pointer to the vector handle.
 * @return 0 on success, or an appropriate error code on failure.
 */
uint8_t c_vector_free(c_vector_h *list);

/**
 * Inserts an element at a specific index in the vector.
 * @param list A pointer to the vector handle.
 * @param ele A pointer to the element to be inserted.
 * @param index The position at which to insert the element (0-based).
 * @return 0 on success, or an appropriate error code on failure.
 */
uint8_t c_vector_insert_in(c_vector_h **list, const void *ele, uint8_t index);

/**
 * Pushes an element to the back of the vector (appends).
 * @param list A pointer to the vector handle.
 * @param ele A pointer to the element to be pushed.
 * @return A pointer to the newly pushed element, or NULL on failure.
 */
const void *c_vector_push(c_vector_h **list, const void *ele);

/**
 * Constructs an element at the back of the vector using the provided arguments.
 * @param list A pointer to the vector handle.
 * @param args A pointer to the arguments used for element construction.
 * @return A pointer to the newly constructed element, or NULL on failure.
 */
const void *c_vector_emplace_back(c_vector_h **list, const void *args);

/**
 * Finds an element in the vector using the comparison function.
 * @param list A pointer to the vector handle.
 * @param ele A pointer to the element to search for.
 * @return A pointer to the found element, or NULL if not found.
 */
void *c_vector_find(c_vector_h *list, const void *ele);

/**
 * Retrieves an element at the specified index.
 * @param list A pointer to the vector handle.
 * @param index The index of the element to retrieve (0-based).
 * @return A pointer to the element, or NULL if the index is out of bounds.
 */
void *c_vector_get_at_index(c_vector_h *list, const uint8_t index);

/**
 * Deletes an element from the vector using the comparison function.
 * @param list A pointer to the vector handle.
 * @param ele A pointer to the element to delete.
 * @return 0 on success, or an appropriate error code on failure.
 */
uint8_t c_vector_delete_ele(c_vector_h *list, const void *ele);

/**
 * Deletes an element at the specified index from the vector.
 * @param list A pointer to the vector handle.
 * @param index The index of the element to delete (0-based).
 * @return 0 on success, or an appropriate error code on failure.
 */
uint8_t c_vector_delete_ele_at_index(c_vector_h *list, const uint8_t index);

/**
 * Converts the vector to a string and prints it using the print function.
 * @param list A pointer to the vector handle.
 */
void c_vector_to_string(c_vector_h *list);

/**
 * Returns the current number of elements in the vector.
 * @param list A pointer to the vector handle.
 * @return The number of elements in the vector.
 */
uint8_t c_vector_length(const c_vector_h *list);

/**
 * Returns the current capacity of the vector.
 * @param list A pointer to the vector handle.
 * @return The capacity of the vector.
 */
uint8_t c_vector_capacity(const c_vector_h *list);

/**
 * Returns the size of each element in the vector.
 * @param list A pointer to the vector handle.
 * @return The size of elements in the vector (in bytes).
 */
EleSizeUnit_t c_vector_ele_size(const c_vector_h *list);

/**
 * Clears all elements from the vector.
 * @param list A pointer to the vector handle.
 * @return 0 on success, or an appropriate error code on failure.
 */
uint8_t c_vector_clear(const c_vector_h *list);

#endif /* __C_VECTOR__ */
