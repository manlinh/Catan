#ifndef _VECTORINT_H
#define _VECTORINT_H
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <regex.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

typedef struct vectorInt {
    int* data;
    size_t size;
    size_t capacity;
    void (*insert)(struct vectorInt* THIS, size_t idx, int elm);
    int (*remove)(struct vectorInt* THIS, size_t idx);
    void (*set)(struct vectorInt* THIS, size_t idx, int elm);
    int (*get)(struct vectorInt* THIS, size_t idx);
    void (*push)(struct vectorInt* THIS, int elm);
    int (*pop)(struct vectorInt* THIS);
    int (*back)(struct vectorInt* THIS);
    int (*front)(struct vectorInt* THIS);
    void (*clear)(struct vectorInt* THIS);
    void (*free)(struct vectorInt* THIS);
    void (*resize)(struct vectorInt* THIS, size_t size);
    _Bool (*empty)(struct vectorInt* THIS);
    void (*swap)(struct vectorInt* THIS, size_t idx1, size_t idx2);
    void (*sort)(struct vectorInt* THIS,
                 int (*compare)(const int*, const int*));
} vectorInt;
vectorInt* create_vector_vectorInt();

#endif