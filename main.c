
#include "lib.h"

struct app {
    bool         running;     /* Application running */
    time_t       start0;
    time_t       stop0;
    struct knot *Elist;       /* Tree root           */
};

static struct app app;

struct knot *elist_new(char * info4) {
    struct knot *result = calloc(1, sizeof(struct knot));
    result->root->info = strdup(info4);
    result->root->n_inf = strlen(info4);
    return result;
}

int func4plus(const char *input_file) {
    FILE * f_input;
    int k_or_i=0;
    int key4=0;
    char * info4 = calloc(2, sizeof(char));
    int s_of_i = 1;
    int ready=0;
    char symb;
//    char * T_Null = {'\0'};
//    char * symb_str = calloc(1, sizeof(char));
    if ((f_input = fopen(input_file, "r")) == NULL) {
        fprintf(stderr, "Unable to open file %s to read\n", input_file);
        return 2;
    }

    while(fscanf(f_input, "%c", &symb) != EOF) {
//        printf("value = %u\n", symb);

        if(symb == '\n')
        {
            k_or_i++;
            if(k_or_i % 2 == 0)
            {
		printf("key: %d\n", key4);
		printf("info: ");
		for(int l=0; l<strlen(info4); l++)
		{
			printf("%c", info4[l]);
		}
		printf("\n");
                if (ready==0)
                {
                    ready=1;
                    app.Elist->root = elist_new();
                    app.Elist->root->key = key4;
                    app.Elist->root->color = 0;
                    app.Elist->root->right = app.Elist;
                    app.Elist->root->left = app.Elist;
                    app.Elist->root->root = NULL;
                }
                else
                {
                    adding_knot(app.Elist, key4, info4);
                }
                s_of_i = 1;
                info4 = realloc(info4, s_of_i*sizeof(char));
            }
        }
        else {
            if (k_or_i%2==0) {
                int znach = (int)(symb);
                key4 = key4*10 + znach - 48;
//                printf("value = %u\n", key4);
            }
            if (k_or_i%2==1) {
                s_of_i++;
//                symb_str[0] = symb;
                info4 = realloc(info4, s_of_i*sizeof(char));
                info4[s_of_i-2] = symb;
            }
        }
    }
    adding_knot(app.Elist, key4, info4);
//    printf("Last: key = %u value = %u\n", key4, info4);
//    free(symb_str);
    fclose(f_input);
    return 0;
}

/**
 * Выход
 *
**/
void func0() {
    app.running = false;
}

/**
 * Ввод числа
**/
int input_number(const char *prompt) {
    int value;
    printf("%s\n", prompt);
    scanf("%d", &value);
    return value;
}

/**
 * Ввод строки
 *
 * Note: Программист в ответе за выделенную память функцией `input_text`
 */
char *input_text(const char *prompt) {
    char *value = calloc(512, sizeof(char));
    int lenn = 512;
    printf("%s\n", prompt);
    scanf("%s", value);
    return value;
}

/**
 * Добавление элемента
 */
void func1() {
    int key1 = input_number("Input key of your knot");
    char *info1 = input_text("Input info in your knot");
    adding_knot(app.Elist, key1, info1);
    free(info1);
}

/**
 * Удаление элемента
**/
void func2() {
    int key2 = input_number("Input key of deleatable element");
    time_t start2 = time(NULL);
    del_knot(app.Elist, key2);
    time_t stop2 = time(NULL);
    double duration2 = stop2 - start2;
    printf("Time for deleat element: %10.f seconds\n", duration2);
}

/**
 * Обход
 *
**/
void func3() {
    time_t start3 = time(NULL);
    OBXOD(app.Elist->root, app.Elist);
    time_t stop3 = time(NULL);
    double duration3 = stop3 - start3;
    printf("Time of checking: %10.f seconds\n", duration3);
}

void func4() {
    char *input_file = input_text("Enter source filename");
    time_t start4 = time(NULL);
    if (input_file == NULL) {
        perror("Missed file");
        return;
    }
    func4plus(input_file);
    time_t stop4 = time(NULL);
    double duration4 = stop4 - start4;
    printf("Time of downloading: %10.f seconds\n", duration4);
}

void func5() {
    printf("--- Show ---\n");
    Show_graph(app.Elist);
}

void func6() {
    int key7 = input_number("Input key");
    find_by_near_key(app.Elist, key7);
}

void func7() {
    int key7 = input_number("Input key of findouble element");
    find_by_key(app.Elist, key7);
}

int prompt() {
    Show_possibilities();
    int comm = input_number("");
    return comm;
}

void elist_reset(struct knot *Elist) {
    Elist->info = NULL;
    Elist->key = 0;
    Elist->n_inf = 0;
    Elist->root = NULL;
    Elist->prev = NULL;
    Elist->color = 0;
    Elist->right = NULL;
    Elist->left = NULL;
}

void init() {

    app.running = true;
    app.start0 = time(NULL);

    app.Elist = calloc(1, sizeof(struct knot));
    elist_reset(app.Elist);

}

void fini() {

    /* Освобождение памяти */
    time_t start0 = time(NULL);
    if(app.Elist->root) {
        while (del_knot(app.Elist, app.Elist->root->key) != 25);
    } else {
        free(app.Elist);
    }
    time_t stop0 = time(NULL);
    double duration0 = stop0 - start0;
    printf("Time of deleating: %10.f seconds\n", duration0);

    /* Время работы приложения */
    app.stop0 = time(NULL);
    double app_duration = app.stop0 - app.start0;
    printf("Time of application: %10.f seconds\n", app_duration);

}

int main(int argc, char *argv[])
{

    /* Initialize */
    init();

    /* Main event loop */
    while (app.running) {
        int comm = prompt();
        switch (comm) {
        case 0:
            func0();
            break;
        case 1:
            func1();
            break;
        case 2:
            func2();
            break;
        case 3:
            func3();
            break;
        case 4:
            func4();
            break;
        case 5:
            func5();
            break;
        case 6:
            func6();
            break;
        case 7:
            func7();
            break;
        }
    }

    /* Fini */
    fini();

    return 0;
}

