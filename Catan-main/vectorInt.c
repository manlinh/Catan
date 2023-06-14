#include "vectorInt.h"

void insert_vectorInt(vectorInt* vector, size_t _idx, int val) {
    if (vector->size == vector->capacity) {
        vector->capacity <<= 1;
        vector->data =
            (int*)realloc(vector->data, vector->capacity * sizeof(int));
    }
    for (size_t i = vector->size; i > _idx; i--) {
        vector->data[i] = vector->data[i - 1];
    }
    vector->data[_idx] = val;
    vector->size++;
}
int remove_vectorInt(vectorInt* vector, size_t _idx) {
    int val = vector->data[_idx];
    if (_idx >= vector->size || _idx < 0) {
        return val;
    }
    for (size_t i = _idx; i < vector->size - 1; i++) {
        vector->data[i] = vector->data[i + 1];
    }
    vector->size--;
    return val;
}
void set_vectorInt(vectorInt* vector, size_t _idx, int val) {
    if (_idx >= vector->size || _idx < 0) {
        return;
    }
    vector->data[_idx] = val;
}
int get_vectorInt(vectorInt* vector, size_t _idx) { return vector->data[_idx]; }
void push_vectorInt(vectorInt* vector, int val) {
    insert_vectorInt(vector, vector->size, val);
}
int pop_vectorInt(vectorInt* vector) {
    return remove_vectorInt(vector, vector->size - 1);
}
int back_vectorInt(vectorInt* vector) {
    return get_vectorInt(vector, vector->size - 1);
}
int front_vectorInt(vectorInt* vector) { return get_vectorInt(vector, 0); }
void clear_vectorInt(vectorInt* vector) { vector->size = 0; }
void free_vectorInt(vectorInt* vector) {
    free(vector->data);
    free(vector);
}
_Bool empty_vectorInt(vectorInt* vector) { return vector->size == 0; }
void resize_vectorInt(vectorInt* vector, size_t size) {
    vector->data = (int*)realloc(vector->data, size * sizeof(int));
    vector->capacity = size;
    if (vector->size > size) {
        vector->size = size;
    }
}
void swap_vectorInt(vectorInt* vector, size_t idx1, size_t idx2) {
    if (idx1 >= vector->size || idx2 >= vector->size) {
        return;
    }
    int tmp = vector->data[idx1];
    vector->data[idx1] = vector->data[idx2];
    vector->data[idx2] = tmp;
}
void sort_vectorInt(vectorInt* vector, int (*compare)(const int*, const int*)) {
    qsort(vector->data, vector->size, sizeof(int),
          (int (*)(const void*, const void*))compare);
}
vectorInt* create_vector_vectorInt() {
    vectorInt* vector = (vectorInt*)malloc(sizeof(vectorInt));
    vector->size = 0;
    vector->capacity = 16;
    vector->data = (int*)malloc(vector->capacity * sizeof(int));
    vector->insert = &insert_vectorInt;
    vector->remove = &remove_vectorInt;
    vector->set = &set_vectorInt;
    vector->get = &get_vectorInt;
    vector->push = &push_vectorInt;
    vector->pop = &pop_vectorInt;
    vector->back = &back_vectorInt;
    vector->front = &front_vectorInt;
    vector->clear = &clear_vectorInt;
    vector->free = &free_vectorInt;
    vector->empty = &empty_vectorInt;
    vector->resize = &resize_vectorInt;
    vector->swap = &swap_vectorInt;
    vector->sort = &sort_vectorInt;
    return vector;
}