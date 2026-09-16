#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 5U

typedef struct
{
    uint8_t buffer[BUFFER_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
    uint8_t count;
} Circular_Buffer;

void UART_Receive_Simulate(Circular_Buffer *cb, uint8_t received_data);
uint8_t CircularBuffer_Put(Circular_Buffer *cb, uint8_t data);
uint8_t CircularBuffer_Get(Circular_Buffer *cb, uint8_t *data);

int main(void)
{ 
    Circular_Buffer cb = {0};
    uint8_t data;
   
    UART_Receive_Simulate(&cb, 10);
    UART_Receive_Simulate(&cb, 20);
    UART_Receive_Simulate(&cb, 30);
    UART_Receive_Simulate(&cb, 40);


    printf("Count = %u\n", cb.count);
    printf("Head  = %u\n", cb.head);
    printf("Tail  = %u\n", cb.tail);


    if(CircularBuffer_Get(&cb, &data))
    {
        printf("Data = %u\n", data);
    }
    else
    {
        printf("Buffer is empty\n");
    }

    if(CircularBuffer_Get(&cb, &data))
    {
        printf("Data = %u\n", data);
    }
    else
    {
        printf("Buffer is empty\n");
    }

    UART_Receive_Simulate(&cb, 50);
    UART_Receive_Simulate(&cb, 60);

    if(CircularBuffer_Get(&cb, &data))
    {
        printf("Data = %u\n", data);
    }
    else
    {
        printf("Buffer is empty\n");
    }

    if(CircularBuffer_Get(&cb, &data))
    {
        printf("Data = %u\n", data);
    }
    else
    {
        printf("Buffer is empty\n");
    }

    if(CircularBuffer_Get(&cb, &data))
    {
        printf("Data = %u\n", data);
    }
    else
    {
        printf("Buffer is empty\n");
    }
    if(CircularBuffer_Get(&cb, &data))
    {
        printf("Data = %u\n", data);
    }
    else
    {
        printf("Buffer is empty\n");
    }

    UART_Receive_Simulate(&cb, 10);
    UART_Receive_Simulate(&cb, 20);
    UART_Receive_Simulate(&cb, 30);
    UART_Receive_Simulate(&cb, 40);
    UART_Receive_Simulate(&cb, 50);
    UART_Receive_Simulate(&cb, 60);

    
    if(CircularBuffer_Get(&cb, &data))
    {
        printf("Data = %u\n", data);
    }
    else
    {
        printf("Buffer is empty\n");
    }

    if(CircularBuffer_Get(&cb, &data))
    {
        printf("Data = %u\n", data);
    }
    else
    {
        printf("Buffer is empty\n");
    }

    UART_Receive_Simulate(&cb, 50);
    UART_Receive_Simulate(&cb, 60);

    if(CircularBuffer_Get(&cb, &data))
    {
        printf("Data = %u\n", data);
    }
    else
    {
        printf("Buffer is empty\n");
    }

    if(CircularBuffer_Get(&cb, &data))
    {
        printf("Data = %u\n", data);
    }
    else
    {
        printf("Buffer is empty\n");
    }

    if(CircularBuffer_Get(&cb, &data))
    {
        printf("Data = %u\n", data);
    }
    else
    {
        printf("Buffer is empty\n");
    }
    if(CircularBuffer_Get(&cb, &data))
    {
        printf("Data = %u\n", data);
    }
    else
    {
        printf("Buffer is empty\n");
    }

    if(CircularBuffer_Get(&cb, &data))
    {
        printf("Data = %u\n", data);
    }
    else
    {
        printf("Buffer is empty\n");
    }






}

void UART_Receive_Simulate(Circular_Buffer *cb, uint8_t received_data)
{
    CircularBuffer_Put(cb, received_data);
}

uint8_t CircularBuffer_Put(Circular_Buffer *cb, uint8_t data)
{
    if (cb->count == BUFFER_SIZE)
    {
        return 0U;
    }

    cb->buffer[cb->head] = data;
    cb->head = (cb->head + 1U) % BUFFER_SIZE;
    cb->count++;

    return 1U;
}

uint8_t CircularBuffer_Get(Circular_Buffer *cb, uint8_t *data)
{
    if (cb->count == 0U)
    {
        return 0U;
    }

    *data = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1U) % BUFFER_SIZE;
    cb->count--;

    return 1U;
}