#include<stdio.h>
#include<stdint.h>

struct Charger_Status
{
    uint16_t voltage;
    uint16_t current;
    uint8_t temperature;
    uint8_t fault_status;
};

void print_charger_status(
  struct Charger_Status charger);

  int main(void)
  {
      struct Charger_Status gun_1 =
      {
        
          650U,
          120U,
          45U,
          0U
      };

      struct Charger_Status gun_2 =
      {
          680U,
          150U,
          50U,
          1U
      };

      printf(
        "EV Dual-Gun Charger Status\n"
      );
      printf("\n");

      printf(
        "Gun 1 Status\n"
      );
      print_charger_status(gun_1);
      printf("\n");

        printf(
        "Gun 2 Status\n"
      );
      print_charger_status(gun_2);
  }

  void print_charger_status(
    struct Charger_Status charger
  )
  { 
      printf(
        "Voltage: %u V\n",
        (unsigned int)charger.voltage
      );
      printf(
        "Current: %u A\n",
        (unsigned int)charger.current
      );
      printf(
        "Temperature: %u C\n",
        (unsigned int)charger.temperature
      );
      printf(
        "Fault Status: %u\n",
        (unsigned int)charger.fault_status
      );
      
  }

