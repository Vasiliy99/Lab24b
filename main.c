
#include "lib.h"
#include "reader.h"
#include "util.h"

struct app {
    bool         running;     /* Application running */
    time_t       start0;
    time_t       stop0;
    struct knot *Elist;       /* Tree root           */
};

static struct app app;

/**
 * Выход
 *
**/
void func0() {
    app.running = false;
}

/**
 * Добавление элемента
 */
void func1() {
    int key1 = input_number("Input key of your knot");
    char *info1 = input_text("Input info in your knot");
    printf("debug: key = %d value = %s\n", key1, info1);
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

void custom_read_pair(int key, char *value) {
//    printf("debug: read_pair: key = %d value = %s\n", key, value);
    adding_knot(app.Elist, key, value);
}

void func4() {
    char *input_file = input_text("Enter source filename");
    time_t start4 = time(NULL);
    if (input_file == NULL) {
        perror("Missed file");
        return;
    }
    /* Reading */
    read_pair(input_file, custom_read_pair);
    time_t stop4 = time(NULL);
    double duration4 = stop4 - start4;
    printf("Time of downloading: %10.f seconds\n", duration4);
    free(input_file);
}

void func5() {
    printf("--- Show ---\n");
    Show_graph(app.Elist);
}

void func6() {
    int key7 = input_number("Input key");
    find_by_key(app.Elist, key7);
}

void func7() {
    int key7 = input_number("Input key of findouble element");
    find_by_near_key(app.Elist, key7);
}

int prompt() {
    Show_possibilities();
    int comm = input_number("");
    return comm;
}

void init() {

    app.running = true;
    app.start0 = time(NULL);

    app.Elist = elist_new();

}

void fini() {

    /* Удаление RBTree по ключу */
    time_t start0 = time(NULL);
    if(app.Elist->root) {
        while (del_knot(app.Elist, app.Elist->root->key) != 25);
    }
    time_t stop0 = time(NULL);
    double duration0 = stop0 - start0;
    printf("Time of deleating: %10.f seconds\n", duration0);

    /* Удаление NIL элемента */
    elist_release(app.Elist);

    /* Профилирование времени работы всего приложения */
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
