#include<stdio.h>

enum Charger_State
{
    IDLE,
    CONNECTED,
    CHARGING,
    COMPLETE,
    FAULT
};

int main(void)
{
    enum Charger_State charger_state;
    charger_state = CHARGING;

    printf(
      "Charger State Value: %d\n",
      charger_state
    );


    return 0;
}