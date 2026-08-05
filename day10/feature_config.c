#include <stdio.h>
#include <stdint.h>

/* Function Configuration */
#define ENABLE_CAN 1
#define ENABLE_DISPLAY 1
#define ENABLE_RFID 0

int main (void)
{
    printf(
      "EV Charger Feature Configuration\n"
    );
    printf("\n");

#if ENABLE_CAN

    printf(
      "CAN Communication Enabled\n"
    );

#endif

#if ENABLE_DISPLAY

    printf(
      "Display Enabled\n"
    );

#endif


#if ENABLE_RFID
    printf(
      "RFID Enabled\n"
    );
#endif


    return 0;
}