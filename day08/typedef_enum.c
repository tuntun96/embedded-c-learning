#include<stdio.h>

typedef enum
{ 
    CHARGER_IDLE,
    CHARGER_CONNECTED,
    CHARGER_CHARGING,
    CHARGER_COMPLETE,
    CHARGER_FAULT
} Charger_State;

int main(void)
{
    Charger_State charger_state = CHARGER_CHARGING;

    printf(
      "EV Charger State Value: %d\n",
      charger_state
    );

    return 0;
    
}