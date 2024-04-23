#include <stdio.h>
#include <stdint.h>

#include "binary_convert.h"

void uint8_to_bin(uint8_t entry_value)
{
    int rest = entry_value, 
        bit_tab[8] ={0};
    
    for(int i = 8; i != 0; i--)
    {
        bit_tab[i-1] = rest % 2;
        rest /= 2;
    }

    printf("Uint8_t Value %d to binary :\n", entry_value);
    for(int i = 0; i < 8; i++)
    {
        printf("%d", bit_tab[i]);
        if(i==3) printf(" ");
    }
    printf("\n\n");
    fflush(stdout);
}

void uint16_to_bin(uint16_t entry_value)
{
    int rest = entry_value, 
        bit_tab[16] ={0};
    
    for(int i = 16; i != 0; i--)
    {
        bit_tab[i-1] = rest % 2;
        rest /= 2;
    }

    printf("Uint16_t Value %d to binary :\n ", entry_value);
    for(int i = 0, space = 1; i < 16; i++, space++)
    {
        printf("%d", bit_tab[i]);
        if(space == 4) 
        {
            space = 0;
            printf(" ");
        }
    }
    printf("\n\n");
    fflush(stdout);
}

void uint32_to_bin(uint32_t entry_value)
{
    int rest = entry_value, 
        bit_tab[32] ={0};
    
    for(int i = 32; i != 0; i--)
    {
        bit_tab[i-1] = rest % 2;
        rest /= 2;
    }

    printf("Uint32_t Value %d to binary :\n", entry_value);
    for(int i = 0, space = 1; i < 32; i++, space++)
    {
        printf("%d", bit_tab[i]);
        if(space == 4) 
        {
            space = 0;
            printf(" ");
        }
    }
    printf("\n\n");
    fflush(stdout);
}