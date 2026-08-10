#include <stdio.h>
#include <stdint.h>


/* Seperate memory */
struct chargerInfo1
{
  uint16_t voltage;
  uint16_t current;
  uint16_t tempearture;
};

/* Shared Memory */
union chargerInfo2
{
  uint16_t voltage;
  uint16_t current;
  uint16_t tempearture;
};

int main(void)
{   
    /* Structure declaration */    
    struct chargerInfo1 data1;

    /* Union declaration */    

    union chargerInfo2 data2;
    
    /* Structure assignement */
    data1.voltage = 650U;
    data1.current = 120U;
    data1.tempearture = 45U;   

    printf(
      "Size of Structure: %zu Bytes\n",
      sizeof(struct chargerInfo1)
    );
    printf("\n");

    printf(
      "Size of Union: %zu Bytes\n",
      sizeof(union chargerInfo2)      
    );
    printf("\n");

    printf(
      "Structure Data\n"
    );
    printf("\n");

    printf(
      "Structure Voltage: %u V\n",
      (unsigned int)data1.voltage
    );
    printf(
      "Structure Current: %u A\n",
      (unsigned int)data1.current
    );
    printf(
      "Structure Temperature: %u C\n",
      (unsigned int)data1.tempearture
    );
    printf("\n");

    printf(
      "Union Data\n"
    );
    printf("\n");

    
    data2.voltage = 650U;
    printf(
      "Union Voltage: %u V\n",
      (unsigned int)data2.voltage
    );

    data2.current = 120U;
    printf(
      "Union Current: %u A\n",
      (unsigned int)data2.current
    );

    data2.tempearture = 45U;
    printf(
      "Union Temperature: %u C\n",
      (unsigned int)data2.tempearture
    );

    printf("Union Voltage: %u V\n",
       (unsigned int)data2.voltage);

    printf("Union Current: %u A\n",
          (unsigned int)data2.current);

    printf("Union Temperature: %u C\n",
          (unsigned int)data2.tempearture);

    return 0;
}