#pragma once

#include <stdint.h>
#include <stdlib.h>

#if !defined(__GNUC__) && !defined(__clang__)
#error "unsupported compiler, at the current time only GNU/GCC and CLANG are supported"
#endif

#if __STDC_VERSION__ >= 201100L
#define C_VECTOR_STATIC_ASSERT(EXP, MSG) _Static_assert((EXP), #MSG)
#else
#define C_VECTOR_STATIC_ASSERT(EXP, MSG)            \
  do{                                               \
    char MSG[(EXP) ? 1 : -1];                       \
    (void)MSG;                                      \
  }while(0)
#endif


#if __STDC_VERSION__ >= 199900L
#define C_VECTOR_TYPE_CHECK(x,y)                                                            \
  do{                                                                                       \
    __typeof__ (x) ele_1 = (x);                                                             \
    __typeof__ (y) ele_2 = (y);                                                             \
    C_VECTOR_STATIC_ASSERT(                                                                 \
      __builtin_types_compatible_p(__typeof__(&ele_1), __typeof__(ele_2)), invalid_type);   \
  }while(0)
#else
  #define C_VECTOR_TYPE_CHECK(x,y)
#endif


#define C_VECTOR_APPEND_RAW(x,y)    x ## y
#define C_VECTOR_APPEND_2(x,y)      C_VECTOR_APPEND_RAW(x,y)

#define C_VECTOR_TEMPLATE(TYPE)                           \
  struct C_VECTOR_APPEND_2(CVector_, TYPE##_){            \
    int capacity;                                         \
    int size;                                             \
    TYPE* data;                                           \
}

#define C_VECTOR_LENGTH(self)       (self).size
#define C_VECTOR_GET(self, i)       (self).data[(i)]
#define C_VECTOR_CLEAR(self)        (self).size = 0
#define C_VECTOR_CAPACITY(self)     (self).capacity
#define C_VECTOR_SIZE(self)         (self).size
#define C_VECTOR_ELE_SIZE(self)     sizeof(*(self).data)


#define C_VECTOR_PUSH(self, ele)                                                              \
  do{                                                                                         \
    C_VECTOR_TYPE_CHECK((ele), (self).data);                                                  \
    if ((self).size >= (self).capacity)                                                       \
    {                                                                                         \
      (self).capacity = !(self).capacity ? (1) : ((self).capacity * 2);                       \
      (self).data = realloc((self).data, (self).capacity * C_VECTOR_ELE_SIZE((self)));        \
    }                                                                                         \
    (self).data[(self).size++] = (ele);                                                       \
  }while(0)

#define C_VECTOR_FREE(self)               \
  do{                                     \
    if((self).data) free((self).data);    \
    self.data=NULL;                       \
    self.size=0;                          \
    self.capacity=0;                      \
  }while(0)

#ifdef C_VECTOR_NO_PREFIX
  #define PUSH(self, ele) C_VECTOR_PUSH(self, ele)
  #define LENGTH(self)    C_VECTOR_LENGTH(self)
  #define GET(self, i)    C_VECTOR_GET(self, i)
  #define CLEAR(self)     C_VECTOR_CLEAR(self)
  #define CAPACITY(self)  C_VECTOR_CAPACITY(self)
  #define SIZE(self)      C_VECTOR_SIZE(self)
  #define ELE_SIZE(self)  C_VECTOR_ELE_SIZE(self)
  #define FREE(self)      C_VECTOR_FREE(self)
#endif /* C_VECTOR_NO_PREFIX */
