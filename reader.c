
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
            line_number++;
            if((line_number % 2) == 0) {
                // Line 2
                read_pair(key, value);
                key = INVALID_VALUE;
            } else {
                // Line 1
		value[len]='\0';
                key = atoi(value);
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
    /* Last pair check */
    if (key != INVALID_VALUE) {
        read_pair(key, value);
    }
    /* Step 3. Close */
    fclose(f_input);
    return 0;
}
