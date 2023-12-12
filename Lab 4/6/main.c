#include "utils.h"
#include "stack.h"
#include "tree.h"
#include "task.h"

int main(int argc, char *argv[]) 
{
    if (argc < 2) 
    {
        print_error(ARGUMENTS_ERROR);
        return ARGUMENTS_ERROR;
    }
    
    const status_code st = solve(argv[1]);

    print_error(st);

    return st;
}