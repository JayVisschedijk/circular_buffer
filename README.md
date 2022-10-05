# Circular buffer library

https://github.com/JayVisschedijk/circular_buffer.git

-------------------------------
WHAT DOES IT DO
-------------------------------
Use this library to create a circular buffer. Will work with multiple threads.

- initializing the buffer
`struct circ_buf* buffer_init(size_t elementsize, size_t length, bool nonblocking)`
  `size_t elementsize` = specify the size of the elements you're gonna store in the buffer.
  `size_t length` = set the size of the buffer, a.k.a. the amount of elements you want to store.
  `bool nonblocking` = set to `false` if you want the pop function to wait for input if the buffer is empty. Set to `true` if you want the buffer to be nonblocking.
  Will return a `struct circ_buf*` with all needed data for the buffer. Store this in a variable you'll use as an argument for the other functions.

- freeing the malloc'd memory of the buffer
`void buffer_destroy(struct circ_buf* youneedthis)`
  `struct circ_buf* youneedthis` = the pointer to the variable you used to store the buffer data.

- pushing an element to the buffer
`void buffer_push(struct circ_buf* youneedthis, void* input)`
  `struct circ_buf* youneedthis` = the pointer to the variable you used to store the buffer data.
  `void* input` = pointer to the variable that holds the element you want to store in the buffer.

- popping an element from the buffer
`int buffer_pop(struct circ_buf* youneedthis, void* output)`
  `struct circ_buf* youneedthis` = the pointer to the variable you used to store the buffer data.
  `void* output` = pointer to the variable that you want to hold the element you popped from the buffer.
  Will return `0` when succesfully popped an element.
  If you've set the buffer to be blocking, this function will wait for input if the buffer is empty. (Make sure to do this in a seperate thread)
  If you've set the buffer to be nonblocking, this function will not wait for imput if the buffer is empty. Instead, it will return `-1`.

-------------------------------
DEPENDENCIES
-------------------------------
To use this library, you don't need other libraries.
To run the tests, you need the check library (https://github.com/libcheck/check).

-------------------------------
EXAMPLE
-------------------------------
```
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include "circularbuffer.h"

void* pop_thread(void* p);

int main(void)
{
  struct circ_buf* mybuffer;
  mybuffer = buffer_init(sizeof(int), 2, false);

  pthread_t pop;
  pthread_create(&pop, NULL, pop_thread, mybuffer);

  int a = 34;
  int b = 11;

  buffer_push(mybuffer, &a);
  sleep(1);
  buffer_push(mybuffer, &b);

  pthread_join(pop, NULL);

  buffer_destroy(mybuffer);
  return 0;
}

void* pop_thread(void* p)
{
  struct circ_buf* mybuffer = (struct circ_buf*) p;
  int output = 0;
  
  while (1)
  {
    buffer_pop(mybuffer, &output);
    printf("Popped element: %i\n", output);
  }
  pthread_exit(NULL);
}
```
-------------------------------
HOW TO COMPILE AND TESTS
-------------------------------
- run `cmake ./`
- run `make`
- optionally, to the tests, run `make test`

-------------------------------
\o/ greetings to my sensei [Rowan](https://github.com/rowanG077)