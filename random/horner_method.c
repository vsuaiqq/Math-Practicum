#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

/* O(n), n - длина исходной строки

инициализируем рез = 0

итерируемся с 0(исходное число неотриц), иначе с 1 по всем символам исходной строки
    если isdigit(чар) => последняя цифра равна чар - '0'
    если 'a' <= 'чар' <= 'z' => чар - 'a' + 10
    если 'A' <= 'чар' <= 'Z' => чар - 'A' + 10
    рез += 
        последняя цифра * 
        старое основание ^ (длина исходной строки - текущая позиция - 1(исходное число неотриц), иначе 2)
*/

int to_decimal(const char* str, const int old_base) 
{
    const bool is_negative = str[0] == '-';
    int res = 0;
    for (int i = (int)is_negative; i < strlen(str); ++i) 
    {
        const int digit = isdigit(str[i]) ? (str[i] - '0') : (str[i] >= 'A' && str[i] <= 'Z') ? (str[i] - 'A' + 10) : (str[i] - 'a' + 10);
        res += digit * pow(old_base, strlen(str) - i - 1);
    }
    return is_negative ? -res : res;
}

/* Линия, O(size), size - длина новой строки

инициализируем строку результата

если num == 0 => size = 1
если num > 0 => log(num) / log(основ новой сс) + 1
если num < 0 => log(num) / log(основ новой сс) + 2

если num == 0 => 
    заполняем и возвращаем
если num < 0 =>
    записываем минус в начало строки

инициализируем позицию в строке, как size - 1
Будем сразу заполнять с конца, чтобы потом не переварачивать строку

пока num != 0
    остаток = num % основ новой сс
    num /= основ новой сс
    str[pos--] = (remainder < 10) ? (remainder + '0') : (remainder + 55);

возвращаем строку

*/

char* from_decimal(int num, const int new_base) 
{
    const bool is_negative = num < 0;
    if (is_negative) 
    {
        num = -num;
    }
    int size = !num ? 1 : log(num) / log(new_base) + is_negative + 1;
    char* str = (char*)malloc(sizeof(char) * (size + 1));
    if (!str) return NULL;
    if (!num) 
    {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }
    if (is_negative) 
    {
        str[0] = '-';
    }
    int pos = size - 1;
    while (num) 
    {
        const int remainder = num % new_base;
        num /= new_base;
        str[pos--] = (remainder < 10) ? (remainder + '0') : (remainder + 55);
    }
    str[size] = '\0';
    return str;
}

int main() 
{
    printf("%d\n", to_decimal("101", 2));
    printf("%d\n", to_decimal("z", 36));
    printf("%s\n", from_decimal(8, 2));
    printf("%s\n", from_decimal(7, 2));
    printf("%s\n", from_decimal(-10, 17));

    return 0;
}