#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if !defined(__GNUC__) && !defined(__clang__)
#error "unsupported compiler, at the current time only GNU/GCC and CLANG are supported"
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201100L
#define C_VECTOR_STATIC_ASSERT(EXP, MSG) _Static_assert((EXP), #MSG)
#else
#define C_VECTOR_STATIC_ASSERT(EXP, MSG)                                                        \
  do{                                                                                           \
    char MSG[(EXP) ? 1 : -1];(void)MSG;                                                         \
  }while(0)
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199900L
#define C_VECTOR_TYPE_CHECK(self, x)                                                            \
  do{                                                                                           \
    __typeof__ (x) ele_1 = (x);                                                                 \
    C_VECTOR_STATIC_ASSERT(                                                                     \
      __builtin_types_compatible_p(__typeof__(&ele_1), __typeof__(self.data)), invalid_type);   \
  }while(0)
#else
  #define C_VECTOR_TYPE_CHECK(self, x)
#endif

#define C_VECTOR_APPEND_RAW(x,y)    x ## y
#define C_VECTOR_APPEND_2(x,y)      C_VECTOR_APPEND_RAW(x,y)

#define C_VECTOR_TEMPLATE(TYPE)                                                                 \
  struct C_VECTOR_APPEND_2(CVector_, TYPE##_){                                                  \
    size_t capacity;                                                                            \
    size_t size;                                                                                \
    TYPE* data;                                                                                 \
}

#define C_VECTOR_INIT()             {0,0, NULL}
#define C_VECTOR_LENGTH(self)       (self).size
#define C_VECTOR_GET(self, i)       (self).data[(i)]
#define C_VECTOR_CLEAR(self)        (self).size = 0
#define C_VECTOR_CAPACITY(self)     (self).capacity
#define C_VECTOR_SIZE(self)         (self).size
#define C_VECTOR_ELE_SIZE(self)     sizeof(*(self).data)

#define _REALLOC_VEC(pp_data, capacity, ele_size, out)                                          \
  do{                                                                                           \
    if((capacity) > (SIZE_MAX / ele_size)){                                                     \
      (out) = 0;                                                                                \
    }else{                                                                                      \
      void* tmp = realloc((*pp_data), capacity * ele_size);                                     \
      if (tmp) {                                                                                \
        *pp_data = tmp;                                                                         \
        (out) = 1;                                                                              \
      }else{                                                                                    \
        (out) = 0;                                                                              \
      }                                                                                         \
    }\
  }while(0)

#define C_VECTOR_RESERVE(self, CAPACITY)                                                        \
  do{                                                                                           \
    int ok = 0;                                                                                 \
    _REALLOC_VEC((void**)&(self).data,(CAPACITY), C_VECTOR_ELE_SIZE((self)), ok);               \
    if(ok) (self).capacity = (CAPACITY);                                                        \
  }while(0)

#define C_VECTOR_RESIZE(self, SIZE)                                                             \
  do{                                                                                           \
    if (SIZE >= (self).capacity) {                                                              \
      int ok = 0;                                                                               \
      _REALLOC_VEC((void**)&(self).data,(SIZE), C_VECTOR_ELE_SIZE((self)), ok);                 \
      if(ok){                                                                                   \
        (self).capacity= (SIZE);                                                                \
        (self).size= (SIZE);                                                                    \
      }                                                                                         \
    }else (self).size= (SIZE);                                                                  \
  }while(0)

#define C_VECTOR_PUSH(self, ele)                                                                \
  do{                                                                                           \
    C_VECTOR_TYPE_CHECK((self), (ele));                                                         \
    if ((self).size >= (self).capacity)                                                         \
    {                                                                                           \
      int ok = 0;                                                                               \
      const size_t new_c = !(self).capacity ? (1) : ((self).capacity * 2);                      \
      _REALLOC_VEC((void**)&(self).data, new_c, C_VECTOR_ELE_SIZE((self)), ok);                 \
      if(!ok) break;                                                                            \
      (self).capacity = new_c;                                                                  \
    }                                                                                           \
    (self).data[(self).size++] = (ele);                                                         \
  }while(0)

#define C_VECTOR_FREE(self)                                                                     \
  do{                                                                                           \
    if((self).data) free((self).data);                                                          \
    self.data=NULL;                                                                             \
    self.size=0;                                                                                \
    self.capacity=0;                                                                            \
  }while(0)

#define C_VECTOR_SHRINK_TO_FIT(self)                                                            \
  do{                                                                                           \
    if ((self).size < ((self).capacity)) {                                                      \
      C_VECTOR_RESERVE((self), (self).size);                                                    \
    }                                                                                           \
  }while(0)

#define C_VECTOR_REMOVE_AT(self, i)                                                             \
  do{                                                                                           \
    size_t j = (i);                                                                             \
    if ((self).size > 0 && j < (self).size){                                                    \
      (self).data[j] = C_VECTOR_GET((self),C_VECTOR_SIZE((self))-1);                            \
      (self).size--;                                                                            \
    }                                                                                           \
  }while(0)

#ifdef C_VECTOR_NO_PREFIX
  #define RESERVE(self, capacity)     C_VECTOR_RESERVE(self, capacity)
  #define PUSH(self, ele)             C_VECTOR_PUSH(self, ele)
  #define LENGTH(self)                C_VECTOR_LENGTH(self)
  #define GET(self, i)                C_VECTOR_GET(self, i)
  #define CLEAR(self)                 C_VECTOR_CLEAR(self)
  #define CAPACITY(self)              C_VECTOR_CAPACITY(self)
  #define SIZE(self)                  C_VECTOR_SIZE(self)
  #define ELE_SIZE(self)              C_VECTOR_ELE_SIZE(self)
  #define SHRINK_TO_FIT(self)         C_VECTOR_SHRINK_TO_FIT(self)
  #define REMOVE_AT(self, i)          C_VECTOR_REMOVE_AT(self, i)
  #define FREE(self)                  C_VECTOR_FREE(self)
#endif /* C_VECTOR_NO_PREFIX */
