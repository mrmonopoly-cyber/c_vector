#include <stdio.h>
#include <stdlib.h>

#include "c_vector.h"

//test
int found_char(const void* list_ele, const void* ele){
    int* e_1 = (int *) list_ele;
    int* e_2 = (int *) ele;

    return *e_1 == *e_2;
}

void free_char(){
}

void print_char(const void* ele){
    printf("%d,",*(int*)ele);
}

int main(void)
{
    struct c_vector_input_init args = {
        .capacity = -1,
        .ele_size = sizeof(int),
        .found_f = found_char,
        .free_fun = free_char,
        .print_fun = print_char,
    };
    c_vector* t=c_vector_init(&args);

    int a = 'a';
    for (char i=0; i<13; i++) {
        printf("inserting %d\n",a);
        c_vector_push(&t, &a);
        a++;
    }
    
    a--;
    c_vector_delete_ele(t,&a);
    c_vector_to_string(t);
    printf("\n");

    c_vector_free(t);
    return EXIT_SUCCESS;
}
