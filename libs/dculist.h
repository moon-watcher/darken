#pragma once

typedef struct de_libs_dculist
{
    void **list;
    unsigned int freePos; // Next free position in entities array
    unsigned int size;

    //
    unsigned int allocatedObjects; // Number of allocated entities
    unsigned int objectSize;

    void (*update_f)(void *const);
    void (*remove_f)(void *const);
} de_libs_dculist;

void de_libs_dculist_init(de_libs_dculist *const list, unsigned int size, unsigned int objectSize);
void *de_libs_dculist_add(de_libs_dculist *const list);
void de_libs_dculist_iterator(de_libs_dculist *const list, void (*callback)(void *const));
void de_libs_dculist_remove(de_libs_dculist *const list, void *const data, void (*callback)(void *const));
void de_libs_dculist_end(de_libs_dculist *const list, void (*callback)(void *const));
void de_libs_dculist_reset(de_libs_dculist *const list, void (*callback)(void *const));