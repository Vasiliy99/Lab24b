
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
    while(fscanf(f_input, "%c", &symb) != EOF) {
        if(symb == '\n') {
            printf("debug: line_number = %u value = %s\n", line_number, value);
            line_number++;
            if((line_number % 2) == 0) {
                // Line 2
                printf("debug: value = %s\n", value);
                read_pair(key, value);
                key = INVALID_VALUE;
            } else {
                // Line 1
                key = atoi(value);
                printf("debug: key = %d\n", key);
            }
            // Release memory
            free(value);
            len = 0;
            value = NULL;
        } else {
            if (value == NULL) {
                len = 1;
                value = (char *)calloc(1, sizeof(char));
                value[len - 1] = symb;
            } else {
                len = len + 1;
                value = realloc(value, len*sizeof(char));
                value[len - 1] = symb;
            }
        }
    }
    /* Last pair check */
    if (key != INVALID_VALUE) {
        read_pair(key, value);
    }
    /* Step 3. Close */
    fclose(f_input);
    return 0;
}
