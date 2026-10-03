#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_vector.h"

StringVector* vector_create(size_t initial_capacity) {
  StringVector *vec = (StringVector *)malloc(sizeof(StringVector));
  if (vec == NULL) {
    return NULL;
  }

    if (initial_capacity > 0) {
        vec->data = (char **)malloc(initial_capacity * sizeof(char *));
      if (vec->data == NULL) {
          free(vec);
          return NULL;
      }
    } else {
        vec->data = NULL;
  }

  vec->capacity = initial_capacity;
  vec->size = 0;
  return vec;
}

int vector_push(StringVector *vec, const char *str) {
  if (vec == NULL || str == NULL) {
    return 0;
  }

  if (vec->size == vec->capacity) {
    size_t new_capacity = (vec->capacity == 0) ? 1 : vec->capacity * 2;
    char **new_data = (char **)realloc(vec->data, new_capacity * sizeof(char *));
    if (new_data == NULL) {
      return 0;
    }
    vec->data = new_data;
    vec->capacity = new_capacity;
  }

  char *copy = strdup(str);
  if (copy == NULL) {
    return 0;
  }

  vec->data[vec->size] = copy;
  vec->size++;
  return 1;
}

const char* vector_get(const StringVector *vec, size_t index) {
    if (vec == NULL || index >= vec->size) {
        return NULL;
    }
    return vec->data[index];
}

void vector_free(StringVector *vec) {
    if (vec == NULL) {
        return;
    }

    for (size_t i = 0; i < vec->size; i++) {
        free(vec->data[i]);
    }

    free(vec->data);

    free(vec);
}
