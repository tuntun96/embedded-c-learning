#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 5U

typedef struct 
{
   uint8_t buffer[BUFFER_SIZE];
   volatile uint8_t head; // Producer / ISR owns head
   volatile uint8_t tail; // Consumer / main loop owns tail
}Ring_Buffer;

uint8_t RingBuffer_Put(Ring_Buffer *rb, uint8_t data);

uint8_t RingBuffer_Get(Ring_Buffer *rb, uint8_t *data);


int main(void)
{   
    Ring_Buffer rb = {0};
    uint8_t data;

    if(RingBuffer_Put(&rb, 10))
    {
        printf("Success\n");
    }
    else
    {
        printf("Failed\n");
    }

    if(RingBuffer_Put(&rb, 20))
    {
        printf("Success\n");
    }
    else
    {
        printf("Failed\n");
    }

    if(RingBuffer_Put(&rb, 30))
    {
        printf("Success\n");
    }
    else
    {
        printf("Failed\n");
    }

    if(RingBuffer_Put(&rb, 40))
    {
        printf("Success\n");
    }
    else
    {
        printf("Failed\n");
    }
     

    if(RingBuffer_Get(&rb, &data))
    {
        printf("Data: %u\n", data);
    }
    else
    {
      printf("Failed to get data\n");
    }

    if(RingBuffer_Get(&rb, &data))
    {
        printf("Data: %u\n", data);
    }
    else
    {
      printf("Failed to get data\n");
    }

     if(RingBuffer_Put(&rb, 50))
    {
        printf("Success\n");
    }
    else
    {
        printf("Failed\n");
    }

    if(RingBuffer_Put(&rb, 60))
    {
        printf("Success\n");
    }
    else
    {
        printf("Failed\n");
    }


    if(RingBuffer_Get(&rb, &data))
    {
        printf("Data: %u\n", data);
    }
    else
    {
      printf("Failed to get data\n");
    }

    if(RingBuffer_Get(&rb, &data))
    {
        printf("Data: %u\n", data);
    }
    else
    {
      printf("Failed to get data\n");
    }

    if(RingBuffer_Get(&rb, &data))
    {
        printf("Data: %u\n", data);
    }
    else
    {
      printf("Failed to get data\n");
    }

    if(RingBuffer_Get(&rb, &data))
    {
        printf("Data: %u\n", data);
    }
    else
    {
      printf("Failed to get data\n");
    }


    return 0;
}

uint8_t RingBuffer_Put(Ring_Buffer *rb, uint8_t data)
{ 
    uint8_t next_head = (rb->head + 1U) % BUFFER_SIZE;

    if((next_head) == (rb->tail))
    {
      return 0U;
    }

    else
    {
      rb->buffer[rb->head] = data;
      rb->head = next_head;
      return 1U;
    }
  
}

uint8_t RingBuffer_Get(Ring_Buffer *rb, uint8_t *data)
{
    if(rb->head == rb->tail)
    {
      return 0U;
    }

    else
    {
      *data = rb->buffer[rb->tail];
      rb->tail = (rb->tail + 1U) % BUFFER_SIZE;
      
      return 1U;
    
    }
    
}


uint8_t RingBuffer_IsEmpty(Ring_Buffer *rb)
{
    if(rb-> head == rb-> tail)
    {
        printf("Ring Buffer is Empty\n");
        return 1U;
    }
    return 0U;
}

void Ringbuffer_Flush(Ring_Buffer *rb)
{
        rb->head = 0U;
        rb->tail = 0U;
}

uint8_t RingBuffer_IsFull(Ring_Buffer *rb)
{
    uint8_t next_head = (rb->head + 1U) % BUFFER_SIZE;
    if(next_head == rb->tail)
    {
        printf("Ring Buffer is Full\n");
        return 1U;
    }
    return 0U;
}

uint16_t RingBuffer_GetCount(Ring_Buffer *rb)
{
    if(rb->head >= rb->tail)
    {
        return (rb->head - rb->tail);
    }
    else
    {
        return (BUFFER_SIZE - rb->tail + rb->head);
    }
}


