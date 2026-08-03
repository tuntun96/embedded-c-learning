#include<stdio.h>

int main(void)
{
    char charger_status[] = "READY";

    printf(
      "Original Status: %s\n", 
      charger_status
    );

    charger_status[0] = 'L';
    
    printf(
      "After First Update: %s\n",
      charger_status
    );


    charger_status[4] = '!';

    printf(
      "Final Status: %s\n",
      charger_status
    );

}