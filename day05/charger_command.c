#include<string.h>
#include<stdio.h>

int main(void)
{
    char command[] = "STOP";
    if(strcmp(command, "START") == 0)
    {
        printf(
          "EV Charger Command: %s\n",
          command
        );
        printf(
          "Charging Session Started\n"
        );
    }
    else
    {
        printf(
          "Invalid Charger Command: %s\n",
          command
        );
    }

    return 0;
   
}