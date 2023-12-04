#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>

typedef enum status_code 
{
    success,
    allocate_error,
    file_error,
    invalid_parameter,
} status_code;

typedef struct Tree 
{
    char* word;
    int count;
    struct Tree* left;
    struct Tree* right;
} Tree;

typedef struct Node 
{
    char* word;
    int len;
    struct Node* next;
} Node;

typedef struct List 
{
    Node* head;
} List;

void free_tree(Tree* root) 
{
    if (!root) return;
    if (root->word) free(root->word);
    if (root->left) free_tree(root->left);
    if (root->right) free_tree(root->right);
    free(root);
}

void free_list(List* list) 
{
    if (!list) return;
    Node* tmp = list->head;
    Node* tmp_prev;
    while (tmp) 
    {
        tmp_prev = tmp;
        tmp = tmp->next;
        if (tmp_prev->word) free(tmp_prev->word);
        free(tmp_prev);
    }
    free(list);
}

status_code add_word_to_list(List** list, const char* word, const int length) 
{
    if (!(*list)) 
    {
        *list = (List*)malloc(sizeof(List));
        if (!*list) return allocate_error;
    }

    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return allocate_error;
    new_node->word = (char*)malloc(sizeof(char) * (length + 1));
    if (!new_node->word) return allocate_error;
    strcpy(new_node->word, word);
    new_node->len = length;
    new_node->next = NULL;

    if (!(*list)->head) 
    {
        (*list)->head = new_node;
        return success;
    }

    Node* tmp = (*list)->head;
    while (tmp->next && (tmp->next->len >= length)) tmp = tmp->next;

    if (tmp == (*list)->head) 
    {
        new_node->next = (*list)->head;
        (*list)->head = new_node;
        return success;
    }

    new_node->next = tmp->next;
    tmp->next = new_node;

    return success;
}

status_code add_word(const char* word, Tree** root, bool add_to_list, List** list) 
{
    if (!(*root)) 
    {
        *root = (Tree*)malloc(sizeof(Tree));
        if (!(*root)) return allocate_error;
        (*root)->word = (char*)malloc(sizeof(char) * (strlen(word) + 1));
        strcpy((*root)->word, word);
        (*root)->count = 1;
        (*root)->left = NULL, (*root)->right = NULL;
        if (add_to_list) return add_word_to_list(list, word, strlen(word));
        return success;
    }
    const int cmp = strcmp((*root)->word, word);
    if (!cmp) 
    {
        ++(*root)->count;
        return success;
    }
    else if (cmp < 0) 
    {
        return add_word(word, &(*root)->right, add_to_list, list);
    }
    else 
    {
        return add_word(word, &(*root)->left, add_to_list, list);
    }
}

status_code read_data(const char* input_file_path, char* seps[], const int num_of_seps, Tree** root, List** list) 
{
    for (int i = 2; i < num_of_seps; ++i) 
    {
        if (strlen(seps[i]) > 1) return invalid_parameter;
    }

    FILE* input_file = fopen(input_file_path, "r");
    if (!input_file) return file_error;

    char* word = (char*)malloc(sizeof(char) * 32);
    if (!word) 
    {
        fclose(input_file);
        return allocate_error;
    }

    int word_len = 0, word_cap = 32;
    bool is_sep = false;
    char c;
    while ((c = fgetc(input_file)) != EOF) 
    {
        for (int i = 2; i < num_of_seps; ++i) 
        { 
            if (c == seps[i][0]) 
            {
                is_sep = true;
                while (c == seps[i][0] && c != EOF) 
                {
                    c = fgetc(input_file);
                }
            }
        }

        if (word_len && is_sep) 
        {
            word[word_len] = '\0';
            if (add_word(word, root, true, list) == allocate_error) 
            {
                fclose(input_file);
                free(word);
                free_tree(*root);
                free_list(*list);
                return allocate_error;
            }
            free(word);
            word = NULL;
            word_len = 0, word_cap = 32;
            word = (char*)malloc(sizeof(char) * 32);
            if (!word) 
            {
                fclose(input_file);
                free_tree(*root);
                free_list(*list);
                return allocate_error;
            }
            is_sep = false;
        }

        word[word_len] = c;
        ++word_len;
        if (word_len >= word_cap) 
        {
            word_cap *= 2;
            char* tmp = (char*)realloc(word, word_cap);
            if (!tmp) 
            {
                fclose(input_file);
                free(word);
                free_tree(*root);
                free_list(*list);
                return allocate_error;
            }
            word = tmp;
        }
    }

    if (word_len) 
    {
        word[word_len] = '\0';
        if (add_word(word, root, true, list) == allocate_error) 
        {
            fclose(input_file);
            free(word);
            free_tree(*root);
            free_list(*list);
            return allocate_error;
        }
        free(word);
        word = NULL;
    }
    return success;
}

int tree_depth(Tree* root) 
{
    if (!root) return 0;
    const int left_depth = tree_depth(root->left);
    const int right_depth = tree_depth(root->right);
    return (left_depth > right_depth) ? left_depth + 1 : right_depth + 1;
}

void print_stats(Tree* root) 
{
    if (!root) return;
    printf("%d {%s}\n", root->count, root->word);
    print_stats(root->left);
    print_stats(root->right);
}

int word_count(Tree* root, const char* word) 
{
    if (!root) return -1;
    if (!strcmp(root->word, word)) return root->count;
    return word_count(root->left, word);
    return word_count(root->right, word);
}

void print_max_len_word(List* list) 
{
    if (!list) return;
    if (!list->head) return;
    printf("Max length word is {%s}\n", list->head->word);
}

void print_min_len_word(List* list) 
{
    if (!list) return;
    if (!list->head) return;
    Node* tmp = list->head;
    while (tmp->next) tmp = tmp->next;
    printf("Min length word is {%s}\n", tmp->word);
}

void print_max_count(List* list, const int n) 
{
    if (!list) return;
    if (!list->head) return;
    Node* tmp = list->head;
    for (int i = 0; i < n; ++i) 
    {
        if (!tmp) return;
        printf("%d {%s}\n", tmp->len, tmp->word);
        tmp = tmp->next;
    }
}

void print_tree(Tree* root, FILE* stream, int spaces) 
{
    if (!root) return;
    for (int i = 0; i < spaces; ++i) fputc('\t', stream);
    fprintf(stream, "%d {%s}\n", root->count, root->word);
    print_tree(root->left, stream, spaces + 1);
    print_tree(root->right, stream, spaces + 1);
}

status_code get_tree_from_file(FILE* file, Tree** root) 
{
    bool got_count = false, is_word_starts = false;

    int count = 0, i = 0;

    char count_str[BUFSIZ];

    char* word = (char*)malloc(sizeof(char) * BUFSIZ);
    if (!word) return allocate_error;

    char c = fgetc(file);
    while (c != EOF) 
    {
        if (!got_count && isdigit(c)) 
        {
            count_str[i] = c;
            ++i;
        }
        if (i && !got_count && !isdigit(c)) 
        {
            i = 0;
            count = atoi(count_str);
            count_str[i] = '\0';
            got_count = true;
            c = fgetc(file);
        }
        if (got_count) 
        {
            if (c == '{') 
            {
                is_word_starts = true;
                c = fgetc(file);
                continue;
            }
            if (is_word_starts) 
            {
                if (c == '}') 
                {
                    c = fgetc(file);
                    if (c != '\n') 
                    {
                        fseek(file, -2, SEEK_CUR);
                        c = fgetc(file);
                        word[i] = c;
                        i++;
                        c = fgetc(file);
                        continue;
                    }
                    fseek(file, -1, SEEK_CUR);
                    c = fgetc(file);
                    word[i] = '\0';

                    for (int i = 0; i < count; ++i) 
                    {
                        if (add_word(word, root, false, NULL) == allocate_error) 
                        {
                            free(word);
                            return allocate_error;
                        }
                    }

                    free(word);
                    word = NULL;
                    word = (char*)malloc(sizeof(char) * BUFSIZ);
                    if (!word) return allocate_error;

                    i = 0;
                    got_count = false, is_word_starts = false;
                } 
                else 
                {
                    word[i] = c;
                    ++i;
                }
            }
        }
        c = fgetc(file);
    }
    if (word) free(word);
    return success;
}

void print_menu() 
{
    printf("*--------------------------------------------------------------------------*\n");
    printf("| Available commands:                                                      |\n");
    printf("| [stats] - how many times each word from the file occurs in the file      |\n");
    printf("| [word_count] <word> - number of occurrences of the word in the text      |\n");
    printf("| [max_count] <N> - first N most occurence words in the text               |\n");
    printf("| [max_len] - get word with max length                                     |\n");
    printf("| [min_len] - get word with min length                                     |\n");
    printf("| [depth] - get depth of BST                                               |\n");
    printf("| [print] <file_path> - print BST to <file_path> and get it back to stdout |\n");
    printf("| [exit] - finish the programm                                             |\n");
    printf("*--------------------------------------------------------------------------*\n");
}

status_code interaction(Tree* root, List* list) 
{
    while (true) 
    {
        print_menu();
        printf("Enter command here: ");
        char cmd[BUFSIZ];
        scanf("%s", cmd);
        if (!strcmp("stats", cmd)) 
        {
            print_stats(root);
        } 
        else if (!strcmp("word_count", cmd)) 
        {
            char word[BUFSIZ];
            scanf("%s", word);
            const int res = word_count(root, word);
            if (res == -1) printf("Doesn\'t found!\n");
            else printf("%d occurences\n", word_count(root, word));
        }
        else if (!strcmp("max_count", cmd)) 
        {
            int n;
            if (scanf("%d", &n) != 1) 
            {
                printf("N must be integer!\n");
            }
            else if (n < 0) 
            {
                printf("N must be non-negative!\n"); 
            }
            else
            {
                print_max_count(list, n);
            }
        }
        else if (!strcmp("max_len", cmd)) 
        {
            print_max_len_word(list);
        }
        else if (!strcmp("min_len", cmd)) 
        {
            print_min_len_word(list);
        }
        else if (!strcmp("depth", cmd)) 
        {
            printf("Depth is %d\n", tree_depth(root));
        }
        else if (!strcmp("print", cmd)) 
        {
            char file_path[BUFSIZ];
            scanf("%s", file_path);
            FILE* output_file = fopen(file_path, "w");
            if (!output_file) 
            {
                free_tree(root);
                return file_error;
            }
            print_tree(root, output_file, 0);
            fclose(output_file);
            FILE* input_file = fopen(file_path, "r");
            if (!input_file) 
            {
                free_tree(root);
                return file_error;
            }
            Tree* new_root = NULL;
            if (get_tree_from_file(input_file, &new_root) == allocate_error) return allocate_error;
            fclose(input_file);
            print_tree(new_root, stdout, 0);
            free_tree(new_root);
        }
        else if (!strcmp("exit", cmd)) break;
        else printf("Invalid command!\n");
    }
    return success;
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        printf("Invalid number of arguments!\n");
        return 1;
    }

    const char* file_path = argv[1];

    Tree* root = NULL;
    List* list = NULL;
    switch (read_data(file_path, argv, argc, &root, &list))
    {
        case success:
            break;
        case allocate_error:
            printf("Allocate error detected!\n");
            return 1;
        case file_error:
            printf("File error detected!\n");
            return 1;
        case invalid_parameter:
            printf("Invalid parameter detected!\n");
            return 1;
    }

    if (interaction(root, list) == allocate_error) 
    {
        printf("Allocate error detected!\n");
        return 1;
    }

    free_tree(root);
    free_list(list);

    return 0;
}