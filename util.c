
#include <stdio.h>
#include <stdlib.h>

/**
 * Ввод строки
 *
 * Note: Программист в ответе за выделенную память функцией `input_text`
 */
char *input_text(const char *prompt) {
    char ch = '\0';
    int len = 0;
    char *value = NULL;
    printf("%s\n", prompt);
    for (;;) {
        fread(&ch, sizeof(char), 1, stdin);
        if (ch == '\n') {
            break;
        }
        size_t new_len = len + 1;
        value = (char *)realloc(value, (new_len+1) * sizeof(char));
        value[len] = ch;
        value[len+1] = '\0';
        len = new_len;
    }
    //printf("debug: user input: value = %s len = %u\n", value, len);
    return value;
}

/**
 * Ввод числа
 *
**/
int input_number(const char *prompt) {
    int value;
    char *str = input_text(prompt);
    value = atoi(str);
    free(str);
    return value;
}
