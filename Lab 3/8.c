#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>

#define MAX_INT_LEN 10

typedef enum status_code 
{
    SUCCESS,
    FILE_ERROR,
    ALLOCATE_ERROR,
    AGRUMENTS_ERROR,
    INVALID_PARAMETER,
} status_code;

typedef enum cmd_type 
{
    ADD,
    SUB,
    MULT,
    DIV,
    MOD,
    EVAL,
    DIFF,
    CMPS,
    UNKNOWN,
} cmd_type;

typedef struct Polynom 
{
    int coef;
    int degree;
    struct Polynom* next_monom;
} Polynom;

int cmp_monom(const void* a, const void* b) 
{
    return ((Polynom*)a)->degree - ((Polynom*)b)->degree;
}

void print_error(const status_code st) 
{
    if (st == FILE_ERROR) 
    {
        printf("File error detected!\n");
    }
    if (st == ALLOCATE_ERROR) 
    {
        printf("Allocate error detected!\n");
    }
    if (st == AGRUMENTS_ERROR) 
    {
        printf("Invalid number of arguments detected!\n");
    }
    if (st == INVALID_PARAMETER) 
    {
        printf("Invalid parameter detected!\n");
    }
}

cmd_type get_cmd_type(const char* cmd) 
{
    if (!strcmp(cmd, "Add")) return ADD;
    if (!strcmp(cmd, "Sub")) return SUB;
    if (!strcmp(cmd, "Mult")) return MULT;
    if (!strcmp(cmd, "Div")) return DIV;
    if (!strcmp(cmd, "Mod")) return MOD;
    if (!strcmp(cmd, "Eval")) return EVAL;
    if (!strcmp(cmd, "Diff")) return DIFF;
    if (!strcmp(cmd, "Cmps")) return CMPS;
    return UNKNOWN;
}

void free_polynom(Polynom* polynom) 
{
    if (!polynom) return;
    Polynom* tmp = polynom;
    while (tmp->next_monom) 
    {
        Polynom* tmp_next = tmp->next_monom;
        free(tmp);
        tmp = NULL;
        tmp = tmp_next;
    }
    free(tmp);
    tmp = NULL;
}

void free_vse_che_nado(const int free_cnt, const int fclose_cnt, const int free_polynom_cnt, ...) 
{
    va_list ptr;
    va_start(ptr, free_polynom_cnt);
    for (int i = 0; i < free_cnt; ++i) 
    {
        void* ptr_to_free = va_arg(ptr, void*);
        free(ptr_to_free);
        ptr_to_free = NULL;
    }
    for (int i = 0; i < fclose_cnt; ++i) 
    {
        FILE* file_to_close = va_arg(ptr, FILE*);
        fclose(file_to_close);
    }
    for (int i = 0; i < free_polynom_cnt; ++i) 
    {
        void* polynom_to_free = va_arg(ptr, Polynom*);
        free_polynom(polynom_to_free);
    }
    va_end(ptr);
}

void print_polynom(Polynom* polynom) 
{
    if (!polynom) return;

    Polynom* tmp = polynom;
    while (tmp) 
    {
        if (tmp != polynom) 
        {
            if (tmp->coef > 0) printf("+");
        }
        if (abs(tmp->coef) != 1 && tmp->degree) printf("%d", tmp->coef);
        if (tmp->coef == -1) 
        {
            if (tmp->degree) printf("-");
            else printf("%d", tmp->coef);
        }
        if (tmp->degree) 
        {
            if (tmp->degree > 9) 
            {
                printf("x^[%d]", tmp->degree);
            } 
            else if (tmp->degree > 1) 
            {
                printf("x^%d", tmp->degree);
            } 
            else printf("x");
        } 
        else if (tmp->coef != -1) printf("%d", tmp->coef);
        tmp = tmp->next_monom;
    }

    printf("\n");
}

status_code insert_polynom(Polynom** polynom, const int coef, const int degree) 
{
    if (!coef) return INVALID_PARAMETER;

    Polynom* monom = (Polynom*)malloc(sizeof(Polynom));
    if (!monom) return ALLOCATE_ERROR;

    monom->coef = coef;
    monom->degree = degree;

    if (!*polynom) 
    {
        monom->next_monom = NULL;
        *polynom = monom;
        return SUCCESS;
    } 
    if (!(*polynom)->next_monom) 
    {
        if (cmp_monom(*polynom, monom) > 0) 
        {
            monom->next_monom = NULL;
            (*polynom)->next_monom = monom;
        } 
        else 
        {
            monom->next_monom = *polynom;
            *polynom = monom;
        }
        return SUCCESS;
    } 

    Polynom* tmp = *polynom;

    if (cmp_monom(tmp, monom) <= 0) 
    {
        monom->next_monom = tmp;
        *polynom = monom;
        return SUCCESS;
    }

    while (tmp->next_monom) 
    {
        if (cmp_monom(tmp->next_monom, monom) < 0) break;
        tmp = tmp->next_monom;
    }

    Polynom* tmp_next = tmp->next_monom;
    tmp->next_monom = monom;
    monom->next_monom = tmp_next;

    return SUCCESS;
}

status_code add(Polynom** res, Polynom* polynom_fst, Polynom* polynom_snd) 
{
    status_code st;

    Polynom* tmp_fst = polynom_fst;
    Polynom* tmp_snd = polynom_snd;

    if (!tmp_snd) 
    {
        while (tmp_fst) 
        {
            st = insert_polynom(res, tmp_fst->coef, tmp_fst->degree);
            if (st != SUCCESS) return st; 
            tmp_fst = tmp_fst->next_monom;
        }
        return SUCCESS;
    } 

    while (tmp_fst && tmp_snd) 
    {
        if (tmp_fst->degree == tmp_snd->degree) 
        {
            const int new_coef = tmp_fst->coef + tmp_snd->coef;
            if (new_coef) 
            {
                st = insert_polynom(res, new_coef, tmp_fst->degree);
                if (st != SUCCESS) return st;
            }
            tmp_fst = tmp_fst->next_monom;
            tmp_snd = tmp_snd->next_monom;
        }
        else if (tmp_fst->degree > tmp_snd->degree) 
        {
            st = insert_polynom(res, tmp_fst->coef, tmp_fst->degree);
            if (st != SUCCESS) return st;
            tmp_fst = tmp_fst->next_monom;
        }
        else
        {
            st = insert_polynom(res, tmp_snd->coef, tmp_snd->degree);
            if (st != SUCCESS) return st;
            tmp_snd = tmp_snd->next_monom;
        } 
    }

    while (tmp_fst) 
    {
        st = insert_polynom(res, tmp_fst->coef, tmp_fst->degree);
        if (st != SUCCESS) return st;
        tmp_fst = tmp_fst->next_monom;
    }

    while (tmp_snd) 
    {
        st = insert_polynom(res, tmp_snd->coef, tmp_snd->degree);
        if (st != SUCCESS) return st;
        tmp_snd = tmp_snd->next_monom;
    }

    return SUCCESS;
}

status_code copy(Polynom** polynom_dest, Polynom* polynom_src) 
{
    const status_code st = add(polynom_dest, polynom_src, NULL);
    if (st != SUCCESS) 
    {
        free_polynom(*polynom_dest);
    }
    return st;
}

status_code sub(Polynom** res, Polynom* polynom_fst, Polynom* polynom_snd) 
{
    status_code st;

    Polynom* tmp_fst = polynom_fst;
    Polynom* tmp_snd = polynom_snd;
    if (!tmp_snd) 
    {
        while (tmp_fst) 
        {
            st = insert_polynom(res, -tmp_fst->coef, tmp_fst->degree);
            if (st != SUCCESS) return st; 
            tmp_fst = tmp_fst->next_monom;
        }
    } 

    while (tmp_fst && tmp_snd) 
    {
        if (tmp_fst->degree == tmp_snd->degree) 
        {
            const int new_coef = tmp_fst->coef - tmp_snd->coef;
            if (new_coef) 
            {
                st = insert_polynom(res, new_coef, tmp_fst->degree);
                if (st != SUCCESS) return st;
            }
            tmp_fst = tmp_fst->next_monom;
            tmp_snd = tmp_snd->next_monom;
        }
        else if (tmp_fst->degree > tmp_snd->degree) 
        {
            st = insert_polynom(res, tmp_fst->coef, tmp_fst->degree);
            if (st != SUCCESS) return st;
            tmp_fst = tmp_fst->next_monom;
        } 
        else
        {
            st = insert_polynom(res, -tmp_snd->coef, tmp_snd->degree);
            if (st != SUCCESS) return st;
            tmp_snd = tmp_snd->next_monom;
        } 
    }

    while (tmp_fst) 
    {
        st = insert_polynom(res, tmp_fst->coef, tmp_fst->degree);
        if (st != SUCCESS) return st;
        tmp_fst = tmp_fst->next_monom;
    }

    while (tmp_snd) 
    {
        st = insert_polynom(res, tmp_snd->coef, tmp_snd->degree);
        if (st != SUCCESS) return st;
        tmp_snd = tmp_snd->next_monom;
    }

    return SUCCESS;
}

status_code mult(Polynom** res, Polynom* polynom_fst, Polynom* polynom_snd) 
{
    if (!polynom_fst && !polynom_snd) return INVALID_PARAMETER;

    status_code st;

    Polynom* tmp_fst = polynom_fst;
    Polynom* summator = NULL;
    while (tmp_fst) 
    {
        Polynom* tmp_snd = polynom_snd;
        Polynom* tmp = NULL;
        while (tmp_snd) 
        {
            st = insert_polynom(&tmp, tmp_fst->coef * tmp_snd->coef, tmp_fst->degree + tmp_snd->degree);
            if (st != SUCCESS) 
            {
                free_polynom(tmp);
                return st;
            }
            tmp_snd = tmp_snd->next_monom;
        }

        Polynom* polynom_sum = NULL;
        st = add(&polynom_sum, tmp, summator);
        if (st != SUCCESS) 
        {
            free_vse_che_nado(0, 0, 3, tmp, summator, polynom_sum);
            return st;
        }

        free_vse_che_nado(0, 0, 2, tmp, summator);

        summator = polynom_sum;
        tmp_fst = tmp_fst->next_monom;
    }

    *res = summator;

    return SUCCESS;
}

status_code div_n_mod(Polynom** div_res, Polynom** mod_res, Polynom* polynom_fst, Polynom* polynom_snd) 
{
    if (!polynom_snd) return INVALID_PARAMETER;

    status_code st;

    Polynom* polynom_fst_cpy = NULL;
    st = copy(&polynom_fst_cpy, polynom_fst);
    if (st != SUCCESS) return st;

    while (polynom_fst_cpy->degree >= polynom_snd->degree) 
    {
        const int new_coef = polynom_fst_cpy->coef / polynom_snd->coef;
        if (!new_coef) 
        {
            (*mod_res) = polynom_fst_cpy;
            return SUCCESS;
        }

        Polynom* polynom_div = NULL;
        st = insert_polynom(&polynom_div, new_coef, polynom_fst_cpy->degree - polynom_snd->degree);
        if (st != SUCCESS) 
        {
            free_polynom(polynom_fst_cpy);
            return st;
        }
        st = insert_polynom(div_res, new_coef, polynom_fst_cpy->degree - polynom_snd->degree);
        if (st != SUCCESS) 
        {
            free_polynom(polynom_fst_cpy);
            return st;
        }

        Polynom* polynom_mult = NULL;
        st = mult(&polynom_mult, polynom_div, polynom_snd);
        if (st != SUCCESS) 
        {
            free_vse_che_nado(0, 0, 2, polynom_fst_cpy, polynom_div);
            return st;
        }

        Polynom* tmp = NULL;
        st = sub(&tmp, polynom_fst_cpy, polynom_mult);
        if (st != SUCCESS) 
        {
            free_vse_che_nado(0, 0, 3, polynom_fst_cpy, polynom_div, polynom_mult);
            return st;
        }

        free_vse_che_nado(0, 0, 3, polynom_fst_cpy, polynom_div, polynom_mult);

        polynom_fst_cpy = tmp;

        if (!polynom_fst_cpy) break;
    }

    (*mod_res) = polynom_fst_cpy;

    return SUCCESS;
}

status_code eval(double* res, Polynom* polynom, const double point) 
{
    *res = 0;

    if (!polynom) return SUCCESS;

    Polynom* tmp = polynom;
    while (tmp) 
    {
        *res += (tmp->coef * pow(point, tmp->degree));
        tmp = tmp->next_monom;
    }

    return SUCCESS;
}

status_code diff(Polynom** res, Polynom* polynom) 
{
    if (!polynom) return SUCCESS;

    status_code st;

    Polynom* tmp = polynom;
    while (tmp) 
    {
        const int new_degree = tmp->degree - 1;
        if (new_degree != -1) 
        {
            st = insert_polynom(res, tmp->coef * tmp->degree, new_degree);
            if (st != SUCCESS) return st;
        }
        tmp = tmp->next_monom;
    }

    return SUCCESS;
}

status_code cmps(Polynom** res, Polynom* polynom_outside, Polynom* polynom_inside) 
{
    if (!polynom_outside) return SUCCESS;
    if (!polynom_inside) return INVALID_PARAMETER;

    status_code st;

    Polynom* summator = NULL;
    Polynom* tmp_res = NULL;
    Polynom* tmp_outside = polynom_outside;
    while (tmp_outside) 
    {
        Polynom* polynom_inside_cpy = NULL;
        st = copy(&polynom_inside_cpy, polynom_inside);
        if (st != SUCCESS) 
        {
            free_vse_che_nado(0, 0, 2, tmp_res, summator);
            return st;
        }

        for (int i = 0; i < tmp_outside->degree - 1; i++) 
        {
            st = mult(&tmp_res, polynom_inside_cpy, polynom_inside);
            if (st != SUCCESS) 
            {
                free_vse_che_nado(0, 0, 3, tmp_res, summator, polynom_inside_cpy);
                return st;
            }
            free_polynom(polynom_inside_cpy);
            polynom_inside_cpy = tmp_res;
        }

        Polynom* polynom_mult = NULL;
        if (!tmp_outside->degree) 
        {
            st = insert_polynom(&tmp_res, tmp_outside->coef, tmp_outside->degree);
            if (st != SUCCESS) 
            {
                free_vse_che_nado(0, 0, 3, tmp_res, summator, polynom_inside_cpy);
                return st;
            }
            st = add(&polynom_mult, tmp_res, summator);
            if (st != SUCCESS) 
            {
                free_vse_che_nado(0, 0, 3, tmp_res, summator, polynom_inside_cpy);
                return st;
            }
            free_polynom(tmp_res);
        } 
        else 
        {
            if (tmp_outside->degree == 1) 
            {
                tmp_res = polynom_inside_cpy;
                tmp_res->coef *= tmp_outside->coef;
            }
            st = add(&polynom_mult, tmp_res, summator);
            if (st != SUCCESS) 
            {
                free_vse_che_nado(0, 0, 3, tmp_res, summator, polynom_inside_cpy);
                return st;
            }
        }

        free_vse_che_nado(0, 0, 2, polynom_inside_cpy, summator);

        summator = polynom_mult;
        tmp_res = NULL;
        tmp_outside = tmp_outside->next_monom;
    }

    *res = summator;

    return SUCCESS;
}

status_code do_command(Polynom** polynom_res, double* double_res, const char* cmd, Polynom* first, Polynom* second, const double point) 
{
    if (!cmd) return INVALID_PARAMETER;

    status_code st;

    Polynom* tmp = NULL;

    switch (get_cmd_type(cmd))
    {
        case ADD:
            return add(polynom_res, first, second);
        case SUB:
            return sub(polynom_res, first, second);
        case MULT:
            return mult(polynom_res, first, second);
        case DIV:
            st = div_n_mod(polynom_res, &tmp, first, second);
            free_polynom(tmp);
            tmp = NULL;
            return st;
        case MOD:
            st = div_n_mod(&tmp, polynom_res, first, second);
            free_polynom(tmp);
            tmp = NULL;
            return st;
        case EVAL:
            return eval(double_res, first, point);
        case DIFF:
            return diff(polynom_res, second);
        case CMPS:
            return cmps(polynom_res, first, second);
        case UNKNOWN:
            return INVALID_PARAMETER;
    }

    return SUCCESS;
}

status_code create_polynom_pair(Polynom** polynom_fst, Polynom** polynom_snd, char* expr) 
{
    if (!expr) return INVALID_PARAMETER;

    const int expr_len = strlen(expr);

    if (expr[0] != '(' || expr[expr_len - 2] != ')' || expr[expr_len - 1] != ';') return INVALID_PARAMETER;

    status_code st;

    bool got_coef = false, got_degree = false, got_alpha = false, is_negative = false;
    int polynom_number = 1, coef, degree, index = 0;
    char coef_str[MAX_INT_LEN + 1], degree_str[MAX_INT_LEN + 1];
    for (int i = 1; i < expr_len - 2; ++i) 
    {
        const char c = expr[i];

        if (c == ',') polynom_number = 2;

        if (c == 'x') 
        {
            if (!got_coef) 
            {
                coef = 1;
                if (is_negative) 
                {
                    coef *= -1, is_negative = false;
                }
                got_coef = true;
            }
            if (expr[i + 1] != '^') 
            {
                degree = 1, got_degree = true;
            }
            if (got_alpha) return INVALID_PARAMETER;
            got_alpha = true;
        }

        if (isdigit(c)) 
        {
            if (!got_coef) 
            {
                coef_str[index] = c;
                ++index;
                if (!isdigit(expr[i + 1])) 
                {
                    coef_str[index] = '\0';
                    coef = atoi(coef_str);
                    if (is_negative) 
                    {
                        coef *= -1, is_negative = false;
                    }
                    coef_str[0] = '\0', index = 0, got_coef = true;
                }
                if (!isdigit(expr[i + 1]) && expr[i + 1] != 'x') 
                {
                    degree = 0, got_degree = true, got_alpha = true;
                }
            } 
            else 
            {
                if (!got_degree) 
                {
                    degree_str[index] = c;
                    ++index;
                    if (!isdigit(expr[i + 1])) 
                    {
                        degree_str[index] = '\0';
                        degree = atoi(degree_str);
                        if (is_negative) 
                        {
                            coef *= -1, is_negative = false;
                        }
                        degree_str[0] = '\0', index = 0, got_degree = true;
                    }
                }
            }
        }

        if (c == '-') is_negative = true;

        if (got_coef && got_degree && got_alpha) 
        {
            switch (polynom_number)
            {
                case 1:
                    st = insert_polynom(polynom_fst, coef, degree);
                    if (st != SUCCESS) return st;
                    break;
                case 2:
                    st = insert_polynom(polynom_snd, coef, degree);
                    if (st != SUCCESS) return st;
                    break;
                default: break;
            }
            got_coef = false, got_degree = false, got_alpha = false;
        }
    }

    return SUCCESS;
}

status_code solve(const char* path) 
{
    FILE* input = fopen(path, "r");
    if (!input) return FILE_ERROR;

    status_code st;

    char* cmd = (char*)malloc(sizeof(char) * BUFSIZ);
    if (!cmd) 
    {
        fclose(input);
        return ALLOCATE_ERROR;
    }
    
    Polynom* summator = NULL;

    char c;
    int line_index = 0, cmd_index = 0;
    bool got_comm = false;

    char* line = NULL;
    int getline_res;
    while ((getline_res = getline(&line, &(size_t){0}, input)) != -1) 
    {
        if (!strcmp(line, "\n") || !strcmp(line, "\r\n")) 
        {
            free(line), line = NULL;
            continue;
        }

        line_index = 0;
        c = line[line_index];

        while (isspace(c)) 
        {
            c = line[line_index];
            if (isspace(c)) ++line_index;
        }

        if (c == '%' || c == '\0') 
        {
            line_index = 0, free(line), line = NULL;
            continue;
        }
        if (got_comm || c == '[') 
        {
            got_comm = true;
            while (got_comm) 
            {
                c = line[line_index++];
                if (c == ']') 
                {
                    c = line[++line_index];
                    got_comm = false;
                    break;
                }
                if (c == '\n') break;
            }
        }
        if (got_comm) 
        {
            line_index = 0, free(line), line = NULL;
            continue;
        }

        while (isspace(c)) 
        {
            c = line[line_index];
            if (isspace(c)) ++line_index;
        }

        cmd_index = 0;

        while(isalpha(c)) 
        {
            c = line[line_index];
            if (isalpha(c)) 
            {
                cmd[cmd_index++] = c;
                ++line_index;
            }
        }
        cmd[cmd_index] = '\0';

        if (cmd[0] == '\0' && c == '%') 
        {
            free_vse_che_nado(2, 0, 0, line, cmd);
            line_index = 0;
            cmd = (char*)malloc(sizeof(char) * BUFSIZ);
            if (!cmd) 
            {
                return ALLOCATE_ERROR;
            }
            continue;
        }

        cmd_index = 0;

        char* expr = (char*)malloc(sizeof(char) * BUFSIZ);
        if (!expr) 
        {
            free_vse_che_nado(2, 1, 0, line, cmd, input);
            return INVALID_PARAMETER;
        }

        if (c != '(') 
        {
            if (c == '%') 
            {
                free_vse_che_nado(3, 1, 0, line, cmd, expr, input);
                return INVALID_PARAMETER;
            }
            if (c == '[') 
            {
                got_comm = true;
                while (got_comm) 
                {
                    c = line[line_index++];
                    if (c == ']') 
                    {
                        got_comm = false;
                        break;
                    }
                    if (c == '\n') break;
                }
            }
        }
        if (c == '(') 
        {
            while (true) 
            {
                c = line[line_index++];
                if (isspace(c) || c == '%' || c == '\n' || c == '\0') 
                {
                    expr[cmd_index] = '\0';
                    break;
                }
                expr[cmd_index++] = c;
            }
        } 
        else 
        {
            if (c == '\0') 
            {
                free_vse_che_nado(3, 0, 0, line, cmd, expr);
                line_index = 0;
                cmd = (char*)malloc(sizeof(char) * BUFSIZ);
                if (!cmd) return ALLOCATE_ERROR;
                continue;
            }
            free_vse_che_nado(3, 1, 0, line, cmd, expr, input);
            return INVALID_PARAMETER;
        }

        Polynom* polynom_fst = NULL;
        Polynom* polynom_snd = NULL;
        double point;
        if (strcmp(cmd, "Eval")) 
        {
            st = create_polynom_pair(&polynom_fst, &polynom_snd, expr);
            if (st != SUCCESS) 
            {
                free_vse_che_nado(2, 1, 3, cmd, expr, input, polynom_fst, polynom_snd, summator);
                break;
            }
        } 
        else 
        {
            if (sscanf(expr, "(%lf)", &point) != 1) 
            {
                free_vse_che_nado(2, 1, 3, cmd, expr, input, polynom_fst, polynom_snd, summator);
                break;
            }
        }

        Polynom* polynom_res = NULL;
        double double_res;
        if (!polynom_snd) 
        {
            st = do_command(&polynom_res, &double_res, cmd, summator, polynom_fst, point);
            if (st != SUCCESS) 
            {
                free_vse_che_nado(2, 1, 2, cmd, expr, input, polynom_fst, polynom_snd);
                break;
            }
        } 
        else 
        {
            st = do_command(&polynom_res, &double_res, cmd, polynom_fst, polynom_snd, point);
            if (st != SUCCESS) 
            {
                free_vse_che_nado(2, 1, 2, cmd, expr, input, polynom_fst, polynom_snd);
                break;
            }
            free_vse_che_nado(0, 0, 2, polynom_fst, polynom_snd);
        }

        if (get_cmd_type(cmd) == EVAL) 
        {
            printf("%.2lf\n", double_res);
        }

        free_polynom(summator), summator = NULL;

        summator = polynom_res;

        print_polynom(summator);
        
        if (line) free(line), line = NULL;
        if (cmd) 
        {
            free(cmd);
            cmd = (char*)malloc(sizeof(char) * BUFSIZ);
            if (!cmd) return ALLOCATE_ERROR;
        }

        free(expr);
        expr = NULL;
    }

    if (summator) free_polynom(summator);
    if (line) free(line);
    if (cmd) free(cmd);

    fclose(input);

    return got_comm ? INVALID_PARAMETER : SUCCESS;
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        print_error(AGRUMENTS_ERROR);
        return AGRUMENTS_ERROR;
    }

    const status_code st = solve(argv[1]);

    print_error(st);

    return st;
}