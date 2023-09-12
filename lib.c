
#include "lib.h"

int Show_possibilities() {
    printf("\n");
    printf("1) Input knot\n");
    printf("2) Deleat knot\n");
    printf("3) Chack graph\n");
    printf("4) Input graph from file\n");
    printf("5) Output graph\n");
    printf("6) Find by key\n");
    printf("7) Find by nearblest key\n");
    printf("0) End of work\n");
    printf("\n");
    return 0;
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

void add_node_info(struct knot *now, char *info1) {
    if (info1 == NULL) {
        printf("warn: add_node_info empty argument\n");
        return;
    }
    now->n_inf = now->n_inf + 1;
    now->info = (char **)realloc(now->info, (now->n_inf)*sizeof(char *));
    now->info[now->n_inf - 1] = strdup(info1);
}

struct knot *knot_new(int key, char *value, struct knot *Elist, struct knot *prev) {
    printf("debug: knot_new: key = %u value = %s\n", key, value);
    struct knot *now = (struct knot *)calloc(1, sizeof(struct knot));
    now->key = key;
    now->info = NULL;
    now->n_inf = 0;
    now->color = COLOR_RED;
    now->root = NULL;
    now->left = Elist;
    now->right = Elist;
    now->prev = prev;
    /* Добавить значение в info */
    add_node_info(now, value);
    return now;
}

struct knot *elist_new() {
    struct knot *Elist = (struct knot *)calloc(1, sizeof(struct knot));
    Elist->key = -1;
    Elist->info = NULL;
    Elist->n_inf = 0;
    Elist->root = Elist;
    Elist->left = Elist;
    Elist->right = Elist;
    Elist->color = COLOR_BLACK;
    Elist->prev = Elist;
    return Elist;
}

void elist_release(struct knot *now) {
    free(now);
}


int adding_knot(struct knot *Elist, int key1, char *info1) {

    struct knot *now = Elist->root;

    while (now != Elist) {

        if ((key1 > now->key) && (now->right != Elist)) {
            printf("debug: move right node\n");
            now = now->right;
        }
        else if ((key1 < now->key) && (now->left != Elist)) {
            printf("debug: move left node\n");
            now = now->left;
        }
        else if ((key1 > now->key) && (now->right == Elist)) {
            printf("debug: add right node\n");
            now->right = knot_new(key1, info1, Elist, now);
            Fixing_in(Elist, now->right);
            break;
        }
        else if ((key1 < now->key) && (now->left == Elist)) {
            printf("debug: add left node\n");
            now->left = knot_new(key1, info1, Elist, now);
            Fixing_in(Elist, now->left);
            break;
        }
        else if (key1 == now->key) {
            printf("debug: update node\n");
            add_node_info(now, info1);
            break;
        }
    }

    if (Elist->root == Elist) {
        Elist->root = knot_new(key1, info1, Elist, Elist);
        Elist->root->color = COLOR_BLACK;
    }

    return 0;
}

enum result {
    E_OK  = 25,
};

enum way {
    L_RIGHT = 0,
    L_LEFT  = 1,
};

/**
 * Содает копию
 *
 * Note: Получатель должен заботиться о памяти
**/
static char **info_deep_copy(struct knot *cur) {
    char **info_save = (char **)malloc(cur->n_inf * sizeof(char *));
    for (int j=0; j < cur->n_inf; j++) {
        info_save[j] = strdup(cur->info[j]);
    }
    return info_save;
}

/**
 * Осовобожддаем память векора значений узла
 *
**/
static void info_deep_release(struct knot *cur) {
    for (int j=0; j < cur->n_inf; j++) {
        //char *info = cur->info[j];
        free(cur->info[j]);
        cur->info[j] = NULL; // <---
    }
    free(cur->info);
    cur->info = NULL; // <---
}

/**
 * Удаляет узел по его ключу
 *
**/
int del_knot(struct knot *Elist, int key2) {
    char ** info_save;

    printf("debug: del_knot: key = %d\n", key2);

    /* Нет корня */
    struct knot *root = Elist->root;
    if (Elist->root == Elist) {
        return E_OK;
    }

    /* Когда у корня все листья NIL */
    if ((root->key == key2) && (root->left == Elist) && (root->right == Elist)) {
        printf("debug: del_knot: knot = %p key = %u case = root\n", root, key2);

        info_deep_release(root);
        free(Elist->root);
        Elist->root = NULL;

        return E_OK;
    }

    struct knot *prev;
    int l_or_r = -1;
    struct knot *now = root;
    struct knot *save_now = NULL;

    while(1) {
        if ((key2 > now->key) && (now->right != Elist)) {
            prev = now;
            l_or_r = L_RIGHT;
            now = now->right;
        }
        else if((key2 < now->key) && (now->left != Elist)) {
            prev = now;
            l_or_r = L_LEFT;
            now = now->left;
        }
        else if((key2 > now->key) && (now->right == Elist)) {
            printf("warn: no knot available: key = %u\n", key2);
            return 1;
        }
        else if((key2 < now->key) && (now->left == Elist)) {
            printf("warn: no knot available: key = %u\n", key2);
            return 1;
        }
        else if(key2 == now->key) {
            if((now->left == Elist) && (now->right == Elist)) {
                if (l_or_r == L_RIGHT) {
                    prev->right = Elist;
                } else {
                    prev->left = Elist;
                }

                printf("knot = %p key = %u case = both\n", now, now->key);

                info_deep_release(now);
                free(now);

                break;
            }
            else if((now->left == Elist) && (now->right != Elist)) {
                if (l_or_r == -1) {
                    save_now = now->right;

                    info_deep_release(root);

                    root->key = save_now->key;
                    root->info = save_now->info;
                    root->n_inf = save_now->n_inf;
                    root->left = save_now->left;
                    root->right = save_now->right;

                    printf("knot = %p key = %u case = right\n", save_now, save_now->key);

                    save_now->left = NULL;
                    save_now->right = NULL;
		    save_now->info=NULL;
                    free(save_now);
                }
                else if(l_or_r == L_RIGHT) {
                    prev->right = now->right;
                    now->right = Elist;
                    printf("knot = %p key = %u\n", now, now->key);
                    // TODO - free info
                    info_deep_release(now);
                    free(now);
                }
                else if (l_or_r == L_LEFT) {
                    prev->left = now->right;
                    now->right = Elist;
                    printf("knot = %p key = %u\n", now, now->key);
                    // TODO - free info
                    info_deep_release(now);
                    free(now);
                }
                break;
            }
            else if((now->right == Elist) && (now->left != Elist)) {
                if (l_or_r == -1) {
                    save_now = root->left;

                    info_deep_release(root);

                    root->key = save_now->key;
                    root->n_inf = save_now->n_inf;
                    root->info = save_now->info;
                    root->n_inf = save_now->n_inf;
                    root->left = save_now->left;
                    root->right = save_now->right;

                    printf("knot = %p key = %u case = left1\n", save_now, save_now->key);

                    save_now->left = NULL;
                    save_now->right = NULL;
		    save_now->info=NULL;
                    free(save_now);
                }
                else if(l_or_r == L_RIGHT) {
                    prev->right = now->left;
                    now->left = Elist;
                    printf("knot = %p key = %u case = left2\n", now, now->key);
                    // TODO - free info
                    info_deep_release(now);
                    free(now);
                }
                else if(l_or_r == L_LEFT) {
                    prev->left = now->left;
                    now->left = Elist;
                    printf("knot = %p key = %u case = left3\n", now, now->key);
                    // TODO - free info
                    info_deep_release(now);
                    free(now);
                }
                break;
            }
            else if((now->right != Elist) && (now->left != Elist)) {
                int key_save = 0;

                /* Поиск минимального значения правого поддерева*/
                struct knot *post = now->right;
                struct knot *post_prev = now;
                while(post->left != Elist) {
                    post_prev = post;
                    post = post->left;
                }

                key_save = post->key;
                char **info_save = info_deep_copy(post);
                unsigned n_inf_save = post->n_inf;

                /* ... */
                if(post_prev != now) {
                    post_prev->left = post->right;
                } else {
                    post_prev->right = post->right;
                }
                post->right = Elist;

                /* Отладка */
                printf("knot = %p key = %u case = both\n", post, post->key);

                /* Перевешиваем ключи */
                info_deep_release(now);

                now->key = key_save;
                now->info = info_save;
                now->n_inf = n_inf_save;

                /* Освобождаем post */

                info_deep_release(post);
                free(post);

                break;
            }
        }
    }
    return 0;
}

void OBXOD(struct knot *now3, struct knot *Elist)
{
    if (now3==Elist)
    {
        return ;
    }
    else
    {
        if (now3->right !=Elist)
        {
            OBXOD(now3->right, Elist);
        }
        printf("Found Key: %d and Info: ", now3->key);
	for(int i=0; i<now3->n_inf; i++)
	{
		for(int j=0; j<strlen(now3->info[i]); j++)
			printf("%c", now3->info[i][j]);
		printf(", ");
	}
	printf("and their link: %p\n", now3);
        if (now3->left !=Elist)
        {
            OBXOD(now3->left, Elist);
        }
        return ;
    }
}

void show_node(struct knot *now) {
    if (now != NULL) {
        printf("%d ", now->key);
        for (int p=0; p < now->n_inf; p++) {
            printf("%s,", now->info[p]);
        }
        printf("|");
    } else {
        printf("___|");
    }
}

/**
 * Вывод в порядке убывания
 *
**/
int Show_graph(struct knot *Elist) {
    int err=0;
    printf("\n");
    int se = 0;
    struct knot *now = Elist->root;

    show_node(now);
    printf("\n");

    int jn=0;
    int i1=0;
    for(int k=2;; k=k*2) {
    printf("| ");
    se = 0;
    jn++;
    for(int i=k-1; i>=0; i--)
    {
        now = Elist->root;
        int i1 = ToBin(i);
        for(int j=1; j<=jn; j++)
        {
            if(i1/Pow(10, jn-j)==1)
            {
                if(now == Elist)
                {
                    err=1;
                    break;
                }
                now = now->left;
            }
            else
            {
                if(now == Elist)
                {
                    err=1;
                    break;
                }
                now=now->right;
            }
            i1 = i1%Pow(10, jn-j);
        }
        if(now != Elist)
        {
            show_node(now);
        }
        else if((err==1) || (now == Elist))
        {
            se++;
            err=0;
            printf("___");
            printf("___| ");
        }
    }
    printf("\n");
    if(se==k) {
        break;
    }
    }
    printf("\n");
}

int find_by_key(knot* Elist, int key6)
{
    knot * now=Elist->root;
    knot* prev=NULL;
    if(Elist->root==NULL)
    {
        return 1;
    }
    while(1)
    {
        if(key6>now->key && now->right!=Elist)
        {
            prev = now;
            now = now->right;
        }
        else if(key6<now->key && now->left!=Elist)
        {
            prev = now;
            now = now->left;
        }
        else if(key6>now->key && now->right == Elist)
        {
        //    printf("element not found");
            return 1;
        }
        else if(key6<now->key && now->left == Elist)
        {
        //    printf("element not found");
            return 1;
        }    
        else if(key6==now->key)
        {
            for (int i=0; i<now->n_inf; i++){
                printf("found element: ");
                for(int j=0; j<strlen(now->info[i]); j++)
                {
                    printf("%c", now->info[i][j]);
                }
                printf("\n");
                printf("with key: %d & link: %p\n", now->key, now);
            }
            break;
        }
    }
    return 0;
}

int find_by_near_key(knot * Elist, int key7)
{
    knot* now = Elist->root;
    knot *lit = NULL;
    while(key7 != now->key)
    {
        if(key7 > now->key)
            now = now->right;
        else
            now = now->left;
    }
    if (now->prev != Elist)
    	lit = now->prev;
    else if (now->prev == Elist && now->right!=Elist)
	lit = now->right;
    else if (now->prev == Elist && now->right==Elist && now->left!=Elist)
    	lit = now->left;
    else
    {
	printf("Error: only one element\n");
	return 1;
    }
    knot * now_p = now->prev;
    while(now_p != Elist)
    {
	if(abs(now_p->key - key7) <= abs(lit->key - key7))
	{
	    lit = now_p;
	}
	now_p = now_p->prev;
    }
    knot * now_r= now->right;
    while(now_r != Elist)
    {
	if(abs(now_r->key - key7) <= abs(lit->key - key7))
	{
	    lit = now_r;
	}
	now_r = now_r->left;
    }
    knot * now_l= now->left;
    while(now_l != Elist)
    {
	if(abs(now_l->key - key7) <= abs(lit->key - key7))
	{
	    lit = now_l;
	}
	now_l = now_l->right;
    }
    find_by_key(Elist, lit->key);
/*
    if(abs(now->right->key - now->key)<=abs(now->key - now->left->key) && abs(now->right->key - now->key)<=abs(now->key - now->prev->key))
        find_by_key(Elist, now->right->key);
    if(abs(now->prev->key - now->key)<=abs(now->key - now->right->key) && abs(now->prev->key - now->key)<=abs(now->key - now->left->key))
        find_by_key(Elist, now->prev->key);
    if(abs(now->left->key - now->key)<=abs(now->key - now->right->key) && abs(now->left->key - now->key)<=abs(now->key - now->prev->key))
        find_by_key(Elist, now->left->key);
*/
    return 0;
}

knot * left_turn(knot * top)
{
    knot * new_top_l = top;
    knot * new_l_r = top->right->left;
    top = top->right;
    top->left= new_top_l;
    top->left->right = new_l_r;
    return top;
}

knot * right_turn(knot * top)
{
    knot * new_top_r = top;
    knot * new_r_l = top->left->right;
    top = top->left;
    top->right = new_top_r;
    top->right->left = new_r_l;
    return top;
}

int ToBin(int k) {
    if(k==0)
    return 0;
    int n = 0;
    int res = 0;
    while(k>1) {
    res = res + (k%2)*Pow(10, n);
    k=k/2;
    n++;
    }
    res = res + Pow(10, n);
    return res;
}

int Pow(int a, int n)
{
    if(n==0)
        return 1;
    else
    {
        int res=a;
        for(int i=1; i<n; i++)
        {
            res=res*a;
        }
        return res;
    }
}

int Fixing_in(knot * Elist, knot * new) {
    if(new->color == COLOR_RED)
    {
        if((new->prev->prev->right->color == COLOR_RED) && (new->prev->prev->color == COLOR_BLACK) && (new->prev->prev->left->color == COLOR_RED))
        {
            new->prev->prev->color = 1;
            new->prev->prev->right->color = 0;
            new->prev->prev->left->color = 0;
            Fixing_in(Elist, new->prev->prev);
            return 0;
        }
        else if((new->prev->right == new) && (new->prev->prev->left == new->prev) && (new->prev->prev->color == 0) && 
new->prev->prev->left->color == 1 && new->prev->prev->right->color == 0)
        {
            new->prev->prev->left = left_turn(new->prev);
            Fixing_in(Elist, new->left);
            return 0;
        }
        else if((new->prev->left == new) && (new->prev->prev->right == new->prev) && (new->prev->prev->color == 0) &&
new->prev->prev->left->color == 0 && new->prev->prev->right->color == 1)
        {
            new->prev->prev->right = right_turn(new->prev);
            Fixing_in(Elist, new->right);
            return 0;
        }        
        else if((new->prev->left == new) && (new->prev->prev->left == new->prev) && (new->prev->prev->color == 0) && 
new->prev->prev->left->color == 1 && new->prev->prev->right->color == 0)
        {
            new->prev->prev->color = 1;
            new->prev->color = 0;
            if(new->prev->prev->prev == Elist)
                Elist->root = right_turn(new->prev->prev);
            else if(new->prev->prev->prev->right == new->prev->prev)
                new->prev->prev->prev->right = right_turn(new->prev->prev);
            else if(new->prev->prev->prev->left == new->prev->prev)
                new->prev->prev->prev->left = right_turn(new->prev->prev);
            return 0;
        }
        else if((new->prev->right == new) && (new->prev->prev->right == new->prev) && (new->prev->prev->color == 0) && 
new->prev->prev->left->color == 0 && new->prev->prev->right->color == 1)
        {
            new->prev->prev->color = 1;
            new->prev->color = 0;
            if(new->prev->prev->prev == Elist)
                Elist->root = left_turn(new->prev->prev);
            else if(new->prev->prev->prev->right == new->prev->prev)
                new->prev->prev->prev->right = left_turn(new->prev->prev);
            else if(new->prev->prev->prev->left == new->prev->prev)
                new->prev->prev->prev->left = left_turn(new->prev->prev);
            return 0;
        }
    }
    return 0;
}

int Fixing_del(knot * Elist, knot * new)
{
    if(new->prev == 0 && new->prev->right != new && new->prev->right->color == 1 && new->prev->right->right->color == 0 && 
new->prev->right->left->color == 0)
    {
        new->prev->color = 1;
        new->prev->right->color = 0;
        if(new->prev->prev == Elist)
            Elist->root = left_turn(new->prev);
        else if(new->prev->prev->right == new->prev)
            new->prev->prev->right = left_turn(new->prev);
        else if(new->prev->prev->left == new->prev)
            new->prev->prev->left = left_turn(new->prev);
        Fixing_del(Elist, new);    
        return 0;    
    }
    else if(new->prev == 0 && new->prev->left != new && new->prev->left->color == 1 && new->prev->left->right->color == 0 && 
new->prev->left->left->color == 0)
    {
        new->prev->color = 1;
        new->prev->left->color = 0;
        if(new->prev->prev == Elist)
            Elist->root = right_turn(new->prev);
        else if(new->prev->prev->right == new->prev)
            new->prev->prev->right = right_turn(new->prev);
        else if(new->prev->prev->left == new->prev)
            new->prev->prev->left = right_turn(new->prev);
        Fixing_del(Elist, new);    
        return 0;    
    }
    else if(new->prev->right != new && new->prev->right->color == 0 && new->prev->right->right->color == 0 && 
new->prev->right->left->color == 0)
    {
        new->color = 0;
        new->prev->right->color = 1;
        if(new->prev->color == 0 && new->prev != Elist->root)
            Fixing_del(Elist, new);    
        else if(new->prev == Elist->root && new->prev->color == 0)
        {
            
        }
        else if(new->prev->color = 1)
            new->prev->color = 0;
        return 0;    
    }
    else if(new->prev->left != new && new->prev->left->color == 0 && new->prev->left->right->color == 0 && 
new->prev->left->left->color == 0)
    {
        new->color = 0;
        new->prev->left->color = 1;
        if(new->prev->color == 0 && new->prev != Elist->root)
            Fixing_del(Elist, new);    
        else if(new->prev == Elist->root && new->prev->color == 0)
        {
            
        }
        else if(new->prev->color = 1)
            new->prev->color = 0;
        return 0;    
    }
    else if(new->prev->right != new && new->prev->right->color == 0 && new->prev->right->right->color == 0 && 
new->prev->right->left->color == 1)
    {
        new->prev->right->color = 0;
        new->prev->right->left->color = 1;
        new->prev->right = right_turn(new->prev->right);
        Fixing_del(Elist, new);
        return 0;    
    }
    else if(new->prev->left != new && new->prev->left->color == 0 && new->prev->left->right->color == 1 && 
new->prev->left->left->color == 0)
    {
        new->prev->left->color = 0;
        new->prev->left->right->color = 1;
        new->prev->left    = left_turn(new->prev->left);
        Fixing_del(Elist, new);
        return 0;
    }
    
    else if(new->prev->right != new && new->prev->right->color == 0 && new->prev->right->right->color == 1)
    {
        int save = new->prev->right->color;
        new->prev->right->color = new->prev->color;
        new->prev->color = save;
        new->prev->right->right->color = save;
        if(new->prev->prev == Elist)
            Elist->root = left_turn(new->prev);
        else if(new->prev->prev->right == new->prev)
            new->prev->prev->right = left_turn(new->prev);
        else if(new->prev->prev->left == new->prev)
            new->prev->prev->left = left_turn(new->prev);
        return 0;    
    }
    else if(new->prev->left != new && new->prev->left->color == 0 && new->prev->left->left->color == 1)
    {
        int save = new->prev->right->color;
        new->prev->left->color = new->prev->color;
        new->prev->color = save;
        new->prev->left->left->color = save;
        if(new->prev->prev == Elist)
            Elist->root = right_turn(new->prev);
        else if(new->prev->prev->right == new->prev)
            new->prev->prev->right = right_turn(new->prev);
        else if(new->prev->prev->left == new->prev)
            new->prev->prev->left = right_turn(new->prev);        
        return 0;    
    }
    return 0;
}
