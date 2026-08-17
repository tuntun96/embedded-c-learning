#include<stdio.h>
#include<string.h>

int main(void)
{
    char charger_status[] = "CHARGING";
    
    printf(
      "EV Charger Status Monitor\n"
    );

    printf(
      "Current Status: %s\n",
      charger_status
    );

    printf(
      "First Character: %c\n",
      charger_status[0]
    );

    printf(
      "Last Character: %c\n",
      charger_status[strlen(charger_status) - 1U]
    );

    printf(
      "Status Length: %zu\n",
      strlen(charger_status)
    );




    return 0;
}