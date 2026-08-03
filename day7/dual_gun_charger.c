#include<stdio.h>
#include<stdint.h>

struct Charger_Status
{
    uint16_t voltage;
    uint16_t current;
    uint8_t temperature;
    uint8_t fault_status;
};

int main(void)
{
    struct Charger_Status gun_1;
    struct Charger_Status gun_2;
    
    /*Gun 1 data*/
    gun_1.voltage = 650U;
    gun_1.current = 120U;
    gun_1.temperature = 45U;
    gun_1.fault_status = 0U;

    /*Gun 2 data*/
    gun_2.voltage = 680;
    gun_2.current = 150U;
    gun_2.temperature = 50U;
    gun_2.fault_status = 1U;

    printf(
      "EV Dual-Gun Charger Status\n"
    );
    printf("\n");

    printf(
      "Gun 1 Status\n"
    );
    printf(
      "Voltage: %u V\n",
      (unsigned int)gun_1.voltage
    );
    printf(
      "Current: %u A\n",
      (unsigned int)gun_1.current
    );
    printf(
      "Temperature: %u C\n",
      (unsigned int)gun_1.temperature
    );
    printf(
      "Fault Status: %u\n",
      (unsigned int)gun_1.fault_status
    );
    printf("\n");

    printf(
      "Gun 2 Status\n"
    );
    printf(
      "Voltage: %u V\n",
      (unsigned int)gun_2.voltage
    );
    printf(
      "Current: %u A\n",
      (unsigned int)gun_2.current
    );
    printf(
      "Temperature: %u C\n",
      (unsigned int)gun_2.temperature
    );
    printf(
      "Fault Status: %u \n",
      (unsigned int)gun_2.fault_status
    );

    

    return 0;


}