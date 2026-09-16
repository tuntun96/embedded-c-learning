#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 5U

typedef struct
{
    uint8_t buffer[BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} CircularBuffer;


uint8_t CircularBuffer_PutOverwrite(CircularBuffer *cb, uint8_t data);
uint8_t CircularBuffer_Get(CircularBuffer *cb, uint8_t *data);

int main(void)
{
    CircularBuffer cb = {0}; 
    uint8_t data; 

    CircularBuffer_PutOverwrite(&cb, 10);
    CircularBuffer_PutOverwrite(&cb, 20);
    CircularBuffer_PutOverwrite(&cb, 30);
    CircularBuffer_PutOverwrite(&cb, 40);
    CircularBuffer_PutOverwrite(&cb, 50);
    CircularBuffer_PutOverwrite(&cb, 60);
    CircularBuffer_PutOverwrite(&cb, 70);
  

    //data = CircularBuffer_Get(&cb, &data);
    printf("After overwrite\n");

    if(CircularBuffer_Get(&cb, &data))
    {
      printf("Get = %u\n", data);
    }
    if(CircularBuffer_Get(&cb, &data))
    {
      printf("Get = %u\n", data);
    }
    if(CircularBuffer_Get(&cb, &data))
    {
      printf("Get = %u\n", data);
    }
    if(CircularBuffer_Get(&cb, &data))
    {
      printf("Get = %u\n", data);
    }
    if(CircularBuffer_Get(&cb, &data))
    {
      printf("Get = %u\n", data);
    }

    


    return 0;
}

uint8_t CircularBuffer_PutOverwrite(CircularBuffer *cb, uint8_t data)
{
    cb->buffer[cb->head] =  data;

    cb->head = (cb->head + 1U) % BUFFER_SIZE;

    if(cb->count == BUFFER_SIZE)
    {
        cb->tail = (cb->tail + 1U) % BUFFER_SIZE;
    }
    else
    {
        cb->count++;
    }
    return 1U;
}

uint8_t CircularBuffer_Get(CircularBuffer *cb, uint8_t *data)
{
   

    if(cb->count == 0U)
    {
      return 0U;
    }

    *data = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1U) % BUFFER_SIZE;
    cb->count--;

    return 1U;

}