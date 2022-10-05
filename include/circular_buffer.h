#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

struct circ_buf {
    size_t read_ptr;
    size_t write_ptr;
    size_t elementsize;
    size_t length;
    size_t filled;
    bool nonblocking;
    pthread_mutex_t lock;
    pthread_cond_t convar;
    void* buffer;
};

struct circ_buf* buffer_init(size_t elementsize, size_t length, bool nonblocking);
void buffer_destroy(struct circ_buf* youneedthis);
void buffer_push(struct circ_buf* youneedthis, void* input);
int buffer_pop(struct circ_buf* youneedthis, void* output);