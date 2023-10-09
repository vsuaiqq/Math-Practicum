#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

#define MAX_STR_SIZE 32

int to_decimal(char* str, int base) 
{
    const int len = strlen(str);
    int res = 0, degree = len - 1;
    for (int i = 0; i < strlen(str); ++i) 
    {
        int digit = isdigit(str[i]) ? (str[i] - '0') : (str[i] - 55);
        res += digit * pow(base, degree);
        --degree;
    }
    return res;
}

int from_decimal(int x, int)

bool validate_input(char input_str[], int base) 
{
    for (int i = 0; i < strlen(input_str); ++i) 
    {
        char cur_symbol = input_str[i];
        if (!isalpha(cur_symbol) && !isdigit(cur_symbol)) return false;
        if (isalpha(cur_symbol)) 
        {
            if (cur_symbol < 'A' || cur_symbol > 'Z') return false;
        }
        if (isdigit(cur_symbol)) 
        {
            
        }

    }
    return true;
}

int main(int argc, char* argv[]) 
{
    int base;
    char input_num[MAX_STR_SIZE];
    if(!scanf("%d", &base)) 
    {
        printf("base must have int type\n");
        return 1;
    }
    if (base > 36 || base < 2) 
    {
        printf("base must be in range [2, 36]");
    }
    scanf("%s", input_num);
    while (strcmp(input_num, "Stop")) 
    {
        scanf("%s", input_num);
        switch (validate_input(input_num, base))
        {
            case true:
                break;
            case false:
                printf("incorrect value for current base\n");
                return 1;
        }
    }

    return 0;
}