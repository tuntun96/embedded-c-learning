#include<stdint.h>
#include<stdio.h>

int main(void)
{
    uint8_t retry_count = 0U;
    printf("Initial retry count = %u\n", retry_count);
    retry_count++;

    printf("After increment = %u\n", retry_count);
    retry_count++;

    printf("After second increment = %u\n", retry_count);

    if (retry_count > 0U)
    {
      retry_count--;
    }

    printf("After decrement = %u\n", retry_count);

    return 0;


}