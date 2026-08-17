#include<stdio.h>
#include<stdint.h>

struct Charger_Status
{
    uint16_t voltage;
    uint16_t current;
    uint8_t temperature;
    uint8_t fault_status;
};

void update_charger_status(
    struct Charger_Status *charger_pointer
);

void charger_status_display(
  struct Charger_Status *charger_pointer
);

int main(void)
{
    struct Charger_Status charger =
    {
      650U,
      120U,
      45U,
      0U
    };

    struct Charger_Status *charger_pointer;    
    charger_pointer = &charger;

    printf(
      "EV Charger Status Before Update\n"
    );
    printf("\n");
    charger_status_display(charger_pointer);
    printf("\n");    

    update_charger_status(charger_pointer);
    printf(
      "EV Charger Status After Update\n"
    );
     printf("\n"); 
    charger_status_display(charger_pointer);

    return 0;

}

void update_charger_status(
  struct Charger_Status *charger_pointer
)
{
  charger_pointer->current = 150U;
  charger_pointer->fault_status = 1U;
}

void charger_status_display(struct Charger_Status *charger_pointer
)
{
  printf(
      "Voltage: %u V\n",
      (unsigned int)charger_pointer->voltage
    );
    printf(
      "Current: %u A\n",
      (unsigned int)charger_pointer->current
    );
    printf(
      "Temperature: %u C\n",
      (unsigned int)charger_pointer->temperature
    );
    printf(
      "Fault Status: %u\n",
      (unsigned int)charger_pointer->fault_status
    );

}