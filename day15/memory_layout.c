#include <stdio.h>
#include <stdint.h>

uint16_t global_init = 100U;
uint16_t global_uninit;

static uint16_t static_init = 200U;
static uint16_t static_uninit;

const char charger_name[] = "EV Charger";

void charger_function(void)
{
    uint16_t local_voltage = 750U;

    printf("Local Voltage = %u V\n",
    (unsigned int)local_voltage);
}

uint8_t buffer1[100];
static uint8_t buffer2[200];



uint8_t ram_buffer[300];

uint8_t initialized_buffer[300] = {1U, 2U, 3U};

const uint8_t lookup_table[300] = {1U, 2U, 3U};



int main(void)
{
    charger_function();

    printf("Global Init  = %u\n",
      (unsigned int)global_init);

    printf("Global Uninit  = %u\n",
      (unsigned int)global_uninit);

    printf("Static Init  = %u\n",
      (unsigned int)static_init);

    printf("Static Uninit  = %u\n",
      (unsigned int)static_uninit);
    
    printf("Charger Name  = %s\n",
      charger_name);

    return 0;      
    
}