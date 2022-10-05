#include <string.h>
#include "circular_buffer.h"

struct circ_buf* buffer_init(size_t elementsize, size_t length, bool nonblocking)
{
    struct circ_buf* youneedthis;
    youneedthis = (struct circ_buf*)malloc(sizeof(struct circ_buf) + (elementsize * length));
    if (youneedthis == NULL)
    {
        return NULL;
    }
    (*youneedthis).read_ptr = 0;
    (*youneedthis).write_ptr = 0;
    (*youneedthis).elementsize = elementsize;
    (*youneedthis).length = length;
    (*youneedthis).filled = 0;
    (*youneedthis).nonblocking = nonblocking;
    (*youneedthis).lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    (*youneedthis).convar = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    (*youneedthis).buffer = youneedthis + 1;
    
    return youneedthis;
}

void buffer_destroy(struct circ_buf* youneedthis)
{
    free(youneedthis);
}

void buffer_push(struct circ_buf* youneedthis, void* input)
{
    pthread_mutex_lock(&((*youneedthis).lock));
    
    memcpy((*youneedthis).buffer + ((*youneedthis).write_ptr * (*youneedthis).elementsize), input, (*youneedthis).elementsize);

    (*youneedthis).write_ptr = ((*youneedthis).write_ptr + 1) % (*youneedthis).length;
    (*youneedthis).filled += 1;

    if ((*youneedthis).filled > (*youneedthis).length)
    {
        (*youneedthis).filled = (*youneedthis).length;
        (*youneedthis).read_ptr = (*youneedthis).write_ptr;
    }

    if ((*youneedthis).nonblocking == false)
    {
	    pthread_cond_signal(&(*youneedthis).convar);
    }

    pthread_mutex_unlock(&(*youneedthis).lock);
} 

int buffer_pop(struct circ_buf* youneedthis, void* output)
{
    int ret = 0;
    pthread_mutex_lock(&((*youneedthis).lock));
    
    if ((*youneedthis).filled == 0 && (*youneedthis).nonblocking == false)
    {
        while ((*youneedthis).filled == 0)
        {
            pthread_cond_wait(&(*youneedthis).convar, &(*youneedthis).lock);
        }
    }

    if ((*youneedthis).filled > 0)
    {
        memcpy(output, (*youneedthis).buffer + ((*youneedthis).read_ptr * (*youneedthis).elementsize), (*youneedthis).elementsize);

        (*youneedthis).read_ptr = ((*youneedthis).read_ptr + 1) % (*youneedthis).length;

        (*youneedthis).filled -= 1;
    }
    else
    {
        ret = -1;
    }

    pthread_mutex_unlock(&(*youneedthis).lock);
    return ret;
}