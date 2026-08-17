#include<stdio.h>

typedef enum
{
    CHARGER_IDLE,
    CHARGER_CONNECTED,
    CHARGER_CHARGING,
    CHARGER_COMPLETE,
    CHARGER_FAULT
}Charger_State;

int main(void)
{
    Charger_State charger_state = CHARGER_IDLE;
    
    printf(
      "EV Charger State Machine\n"
    );
    printf("\n");

    printf(
      "Initial State: IDLE\n"
    );
    printf("\n");

    charger_state = CHARGER_CONNECTED;
    printf(
      "Vehicle Connected\n"
    );    
    printf(
      "Current State: CONNECTED\n"
    );
    printf("\n");

    charger_state = CHARGER_CHARGING;
    printf(
      "Authorization Successful\n"
    );
    printf(
      "Current State: CHARGING\n"
    );

    return 0;
}