# C Vector Library

![License](https://img.shields.io/github/license/mrmonopoly-cyber/c_vector) ![Language](https://img.shields.io/badge/language-C-blue) ![Version](https://img.shields.io/badge/version-2.4.2-green)

`c_vector` is a dynamic array (vector) implementation in C that supports various operations such as insertion, deletion, search, and memory management for generic data types. The library allows customization of key behaviors like comparison, element construction, and destruction, making it a versatile tool for managing dynamic collections of elements in C.

## Features

- **Dynamic Sizing**: Automatically grows as elements are added.
- **Custom Element Handling**: Define custom functions for element comparison, printing, and freeing.
- **Generic**: Supports elements of any type via user-defined size and handling functions.
- **Efficient**: Supports random access and insertion at any position.
- **Error Handling**: Built-in error codes for robust usage.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
  - [Initialization](#initialization)
  - [Basic Operations](#basic-operations)
  - [Custom Functions](#custom-functions)
- [API Reference](#api-reference)
- [Error Codes](#error-codes)
- [Contributing](#contributing)
- [License](#license)

---

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/mrmonopoly-cyber/c_vector.git
   ```
2. Build using the provided Makefile:
You can compile the library using the provided Makefile.\
The Makefile includes two primary build options: release and debug.\
    - Release Build (Optimized for performance):\
   ```bash
   make release 
   ```

   ```bash
   make 
   ```

    - Debug Build (Optimized for performance):\
   ```bash
   make debug 
   ```

## Usage

### Initialization

To create and initialize a vector, you must provide a configuration using the `c_vector_input_init` structure. This structure allows you to define initial capacity, element size, and custom behavior for comparison, printing, freeing, and populating elements.

Example:

```c
#include "c_vector.h"

int compare_elements(const void *a, const void *b) {
    // Custom comparison function for your element type
    return *(int *)a - *(int *)b;
}

void print_element(const void *ele) {
    printf("%d\n", *(int *)ele);
}

int main() {
    struct c_vector_input_init init_params = {
        .capacity = DEFAULT_CAPACITY,
        .ele_size = sizeof(int),
        .comp_fun = compare_elements,
        .print_fun = print_element,
        .free_fun = NULL,       // No need to free integers
        .populate_fun = NULL    // Not using populate in this case
    };
    
    c_vector_h *vector = c_vector_init(&init_params);
    // Continue using the vector...
    c_vector_free(vector);
}
```

### Basic Operations

- **Inserting Elements**:
    ```c
    int value = 42;
    c_vector_push(&vector, &value);
    ```

- **Finding an Element**:
    ```c
    int key = 42;
    int *found = (int *)c_vector_find(vector, &key);
    ```

- **Accessing Elements**:
    ```c
    int *element = (int *)c_vector_get_at_index(vector, 0);
    ```

- **Deleting an Element**:
    ```c
    int key = 42;
    c_vector_delete_ele(vector, &key);
    ```

- **Print All Elements**:
    ```c
    c_vector_to_string(vector);
    ```

### Custom Functions

You can define custom functions for handling elements when initializing the vector. This makes the vector versatile for handling various data types.

- **Comparison (`comp_fun`)**: Used to find and compare elements.
- **Freeing (`free_ele`)**: Handles the memory freeing of complex data types.
- **Printing (`print_ele`)**: Prints each element when calling `c_vector_to_string`.
- **Populate (`object_populate`)**: Creates/constructs objects directly in the vector.

## API Reference

### Vector Initialization

```c
c_vector_h *c_vector_init(const struct c_vector_input_init *input_args);
```

Initializes the vector with the specified initial parameters. Returns a handle to the vector or `NULL` if initialization fails.

### Insertion Functions

- **Push Element**: 
  ```c
  const void *c_vector_push(c_vector_h **list, const void *ele);
  ```
  Adds an element to the end of the vector.

- **Insert at Index**: 
  ```c
  uint8_t c_vector_insert_in(c_vector_h **list, const void *ele, uint8_t index);
  ```
  Inserts an element at a specific index, shifting the elements to the right.

### Search Functions

- **Find Element**:
  ```c
  void *c_vector_find(c_vector_h *list, const void *ele);
  ```
  Searches for an element using the custom comparison function.

- **Get by Index**:
  ```c
  void *c_vector_get_at_index(c_vector_h *list, const uint8_t index);
  ```
  Retrieves an element at the specified index.

### Deletion Functions

- **Delete Element**:
  ```c
  uint8_t c_vector_delete_ele(c_vector_h *list, const void *ele);
  ```
  Deletes an element from the vector.

- **Delete at Index**:
  ```c
  uint8_t c_vector_delete_ele_at_index(c_vector_h *list, const uint8_t index);
  ```
  Removes an element from the specified index.

### Utility Functions

- **Print Vector**:
  ```c
  void c_vector_to_string(c_vector_h *list);
  ```
  Prints the contents of the vector using the custom print function.

- **Vector Length**:
  ```c
  uint8_t c_vector_length(const c_vector_h *list);
  ```
  Returns the number of elements in the vector.

- **Vector Capacity**:
  ```c
  uint8_t c_vector_capacity(const c_vector_h *list);
  ```
  Returns the total capacity of the vector.

### Memory Management

- **Free Vector**:
  ```c
  uint8_t c_vector_free(c_vector_h *list);
  ```
  Frees the vector and its elements using the `free_fun` (if provided).

- **Clear Vector**:
  ```c
  uint8_t c_vector_clear(const c_vector_h *list);
  ```
  Clears all elements from the vector without freeing the vector itself.

## Error Codes

The following error codes are returned by various functions when operations fail:

- `NULL_REFERENCE_LIST (-1)`: A null reference to the list was encountered.
- `NULL_REFERENCE_ELEMENT (-2)`: A null reference to an element was passed.
- `INVALID_INDEX_VALUE (-3)`: The index provided is invalid.
- `FAILED_RESIZING (-4)`: The vector could not resize during an operation.

## Contributing

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Make your changes and commit.
4. Push to your branch and create a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.
