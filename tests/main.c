#define C_VECTOR_NO_PREFIX
#include "c_vector.h"
#include <stdio.h>
#include <stdint.h>

typedef struct{
  int a;
  char c;
  float d;
}Obj;

typedef C_VECTOR_TEMPLATE(float) VectorFloat;
typedef C_VECTOR_TEMPLATE(Obj) VectorObj;

int main(void)
{
  VectorFloat vf = {0};
  VectorObj vo = {0};
  int i=0;

  PUSH(vf, 2.0f);
  // PUSH(vf, 3.0f);
  // PUSH(vf, 4.0f);
  // PUSH(vf, 6.0f);

  // PUSH(vo, ((Obj){.a = 1, .c = 'f', .d = 0.1f}));

  for (i=0; i<LENGTH(vf); i++) {
    printf("%f\n", GET(vf,i));
  }

  CLEAR(vf);

  for (i=0; i<LENGTH(vf); i++) {
    printf("%f\n", GET(vf,i));
  }

  FREE(vf);
  FREE(vo);

  return 0;
}
