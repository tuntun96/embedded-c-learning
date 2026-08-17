#include<stdio.h>
#include<stdint.h>

#define CURRENT_SAMPLE_COUNT 5U

int main(void)
{
    uint16_t current_samples[CURRENT_SAMPLE_COUNT] =
    {
        100U,
        120U,
        110U,
        130U,
        140U
    };

    uint8_t index;
    uint32_t total_current = 0U;
    uint16_t average_current =0U;
    uint16_t maximum_current;
    uint16_t minimum_current;

    printf("EV Charger Current Analyzer\n");
    for(index = 0; index < CURRENT_SAMPLE_COUNT; index++)
    {
        total_current += current_samples[index];
        printf("Sample %u: %u A\n", index, current_samples[index]);
    }

    average_current = (uint16_t)(total_current / CURRENT_SAMPLE_COUNT);
    printf("Total Current: %lu A\n", total_current);
    printf(
    "Average Current: %u A\n",
    (unsigned int)average_current
    );

    maximum_current = current_samples[0];
    for(index = 1U; index < CURRENT_SAMPLE_COUNT; index++)
    {
        if(current_samples[index] > maximum_current)
        {
            maximum_current = current_samples[index];
        }
    }
    printf("Maximum Current: %u A\n", maximum_current);

    minimum_current = current_samples[0];
    for(index = 1U; index < CURRENT_SAMPLE_COUNT; index++)
    {
        if(current_samples[index] < minimum_current)
        {
            minimum_current = current_samples[index];
        }
    }
    printf("Minimum Current: %u A\n", minimum_current);

    return 0;
}