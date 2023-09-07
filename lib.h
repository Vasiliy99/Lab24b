#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <wchar.h>
#include <unistd.h>

typedef struct knot{
int key;
char ** info;
int color;
int n_inf;
struct knot * root;
struct knot * prev;
struct knot * right;
struct knot * left;
}knot;

int Show_possibilities();

int Fixing_in(knot * Elist, knot * new);

int Fixing_del(knot * Elist, knot * new);

knot * right_turn(knot * top);

knot * left_turn(knot * top);

int del_knot(knot * Elist, int key2, char ** info_save);

int adding_knot(knot * Elist, int key1, char * info1);

void OBXOD(knot * now3, knot * Elist);

int Show_graph(knot * Elist);

int find_by_key(knot * Elist, int key6);

int find_by_near_key(knot * Elist, int key7);

int ToBin(int k);

int Pow(int a, int n);
