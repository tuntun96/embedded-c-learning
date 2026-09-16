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

uint8_t CircularBuffer_Put(Circular_Buffer *cb, uint8_t data);
uint8_t CircularBuffer_Get(Circular_Buffer *cb, uint8_t *data);



int main(void)
{
  Circular_Buffer cb = {0};
  uint8_t data;

  if(CircularBuffer_Put(&cb, 10))
  {
    printf("Data inserted\n");
  }
  else
  {
    printf("Buffer Full\n");
  }

  if(CircularBuffer_Put(&cb, 20))
  {
    printf("Data inserted\n");
  }
  else
  {
    printf("Buffer Full\n");
  }
 
  if(CircularBuffer_Put(&cb, 30))
  {
    printf("Data inserted\n");
  }
  else
  {
    printf("Buffer Full\n");
  }

  if(CircularBuffer_Put(&cb, 40))
  {
    printf("Data inserted\n");
  }
  else
  {
    printf("Buffer Full\n");
  }

  if(CircularBuffer_Put(&cb, 50))
  {
    printf("Data inserted\n");
  }
  else
  {
    printf("Buffer Full\n");
  }

  if(CircularBuffer_Put(&cb, 60))
  {
    printf("Data inserted\n");
  }
  else
  {
    printf("Buffer Full\n");
  }

  if(CircularBuffer_Put(&cb, 70))
  {
    printf("Data inserted\n");
  }
  else
  {
    printf("Buffer Full\n");
  }
 

  if(CircularBuffer_Get(&cb, &data))
  {
    printf("Received = %u\n", data);
  }
  else
  {
    printf("Buffer Empty\n");
  }

  if(CircularBuffer_Get(&cb, &data))
  {
    printf("Received = %u\n", data);
  }
  else
  {
    printf("Buffer Empty\n");
  }
  
 
  if(CircularBuffer_Get(&cb, &data))
  {
    printf("Received = %u\n", data);
  }
  else
  {
    printf("Buffer Empty\n");
  }

  
  if(CircularBuffer_Get(&cb, &data))
  {
    printf("Received = %u\n", data);
  }
  else
  {
    printf("Buffer Empty\n");
  }

  if(CircularBuffer_Get(&cb, &data))
  {
    printf("Received = %u\n", data);
  }
  else
  {
    printf("Buffer Empty\n");
  }
  
    
  return 0;
}

uint8_t CircularBuffer_Put(Circular_Buffer *cb, uint8_t data)
{

  cb->buffer[cb->head] = data;
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

uint8_t CircularBuffer_Get(Circular_Buffer *cb, uint8_t *data)
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