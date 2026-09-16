#include <stdio.h>
#include <stdint.h>

/* defination for max */
#define BUFFER_SIZE 5U

/* Circular Buffer Structure */
typedef struct 
{
    uint8_t buffer[BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
}Circular_Buffer;

void CircularBuffer_Put(Circular_Buffer *cb, uint8_t data);
uint8_t CircularBuffer_Get(Circular_Buffer *cb, uint8_t *data);

int main(void)
{
  Circular_Buffer cb = {0};
 
  CircularBuffer_Put(&cb, 10);
  CircularBuffer_Put(&cb, 20);
  CircularBuffer_Put(&cb, 30);
  CircularBuffer_Put(&cb, 40);
  CircularBuffer_Put(&cb, 50);
  CircularBuffer_Put(&cb, 60);

  printf("Count = %u\n", cb.count);
  printf("Head  = %u\n", cb.head);
  printf("Tail  = %u\n", cb.tail);

  printf("Buffer = %u %u %u %u %u\n",
          cb.buffer[0],
          cb.buffer[1],
          cb.buffer[2],
          cb.buffer[3],
          cb.buffer[4]
  );
  
  uint8_t data = CircularBuffer_Get(&cb, &data);
  printf("Get = %u\n",data);

  data = CircularBuffer_Get(&cb, &data);
  printf("Get = %u\n",data);

  data = CircularBuffer_Get(&cb, &data);
  printf("Get = %u\n",data);

  printf("Count = %u\n", cb.count);
  printf("Tail  = %u\n", cb.tail);

  CircularBuffer_Put(&cb, 60);
  CircularBuffer_Put(&cb, 70);

  printf("\nNew Get\n");
  data = CircularBuffer_Get(&cb, &data);
  printf("Get = %u\n",data);

  data = CircularBuffer_Get(&cb, &data);
  printf("Get = %u\n",data);

  data = CircularBuffer_Get(&cb, &data);
  printf("Get = %u\n",data);

  data = CircularBuffer_Get(&cb, &data);
  printf("Get = %u\n",data, &data);

  data = CircularBuffer_Get(&cb, &data);
  printf("Get = %u\n",data);



 
  
  return 0;
}

void CircularBuffer_Put(Circular_Buffer *cb, uint8_t data)
{
  if(cb->count == BUFFER_SIZE)
  {
    return;
  }

  cb->buffer[cb->head] = data;
  cb->head = (cb->head + 1U) % BUFFER_SIZE;
  cb->count++;

}

uint8_t CircularBuffer_Get(Circular_Buffer *cb, uint8_t *data)
{
    uint8_t data;

    if(cb->count == 0U)
    {
      return 0U;
    }

    *data = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1U) % BUFFER_SIZE;
    cb->count--;

    return data;

}