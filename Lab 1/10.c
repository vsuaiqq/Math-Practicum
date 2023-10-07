#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_STR_SIZE 256

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
        printf("base must have int type. in range [2, 36]\n");
        return 1;
    }
    while (strcmp(input_num, "Stop")) 
    {
        scanf("%s", input_num);
        switch (validate_input(input_num, base))
        {
            case true:
                break;
            case false:
                printf("\n");
                return 1;
        }
    }

    return 0;
}