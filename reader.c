
#include <stdio.h>
#include <stdlib.h>

#include "reader.h"

#define INVALID_VALUE -1

int read_pair(const char *input_file, read_pair_cb read_pair) {
    FILE *f_input;
    char symb;
    char *value = NULL;
    size_t len = 0;
    int line_number = 0;
    int key;
    /* Step 1. Open */
    if ((f_input = fopen(input_file, "r")) == NULL) {
        fprintf(stderr, "Unable to open file %s to read\n", input_file);
        return 2;
    }
    /* Step 2. Reading */
    for(;;) {
        size_t size = fread(&symb, sizeof(char), 1, f_input);
        if ((size == 0) || (symb == EOF)) {
            break;
        }
        if(symb == '\n') {
            line_number++;
            if((line_number % 2) == 0) {
                // Line 2
                printf("debug: read pair: key = %u value = %s\n", key, value);
                read_pair(key, value);
                key = INVALID_VALUE;
            } else {
                // Line 1
                key = atoi(value);
                printf("debug: read key: key = %u <- %s\n", key, value);
            }
            // Release memory
            free(value);
            len = 0;
            value = NULL;
        } else {
            size_t new_len = len + 1;
            value = (char *)realloc(value, (new_len+1)*sizeof(char));
            value[new_len - 1] = symb;
            value[new_len] = '\0';
            len = new_len;
            printf("debug: %s\n", value);
        }
    }
    /* Step 3. Last pair check */
    if (key != INVALID_VALUE) {
        read_pair(key, value);
    }
    /* Step 4. Release memory */
    if (value != NULL) {
        free(value);
    }
    /* Step 5. Close */
    fclose(f_input);
    return 0;
}
