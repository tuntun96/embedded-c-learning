#include<stdio.h>
#include<string.h>

int main(void)
{
    char charger_status[] = "CHARGING";
    
    printf(
      "EV Charger String Information\n"
    );

    printf(
      "Status: %s\n",
      charger_status
    );

    printf(
      "Visible Characters Count:  %zu\n",
      strlen(charger_status)
    );
    
    
    printf(
      "Total Array Size: %zu bytes\n",
      sizeof(charger_status)
    );

    return 0;
}