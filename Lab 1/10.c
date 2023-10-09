#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>

#define MAX_STR_SIZE 32
#define MIN_SS_BASE 2
#define MAX_SS_BASE 36


bool validate_base(int base) 
{
    return (base >= MIN_SS_BASE && base <= MAX_SS_BASE);
}




int main() 
{
    int base, max_input_decimal = INT_MIN;
    char input_str[MAX_STR_SIZE], max_input_str[MAX_STR_SIZE];
    bool is_negative, max_input_is_negative;

    if (!scanf("%d", &base)) 
    {
        printf("base must have int type\n");
        return 1;
    }

    if (!validate_base(base)) 
    {
        printf("base must be in range [2, 36]");
        return 1;
    }

    scanf("%s", input_str);
    while (strcmp(input_str, "Stop")) 
    {
        printf("%d\n", strlen(input_str));
        is_negative = (input_str[0] == '-') ? true : false;
        
        if (!validate_input(input_str, base, is_negative)) 
        {
            printf("invalid value for current base\n");
            return 1;
        }

        int input_decimal;
        switch (to_decimal(input_str, base, is_negative, &input_decimal))
        {
            case td_ok:
                printf("%d\n", input_decimal);
                break;
            case td_overflow:
                printf("overflow!\n");
                return 1;
        }
       
        if (iabs(input_decimal) > iabs(max_input_decimal))
        {
            max_input_decimal = input_decimal;
            max_input_is_negative = is_negative;
            strcpy(max_input_str, input_str);
        }

        scanf("%s", input_str);
    }

    return 0;
}