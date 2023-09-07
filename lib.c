#include "lib.h"
int Show_possibilities()
{
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

struct knot *knot_new(int key, char * info, int l_of_i, knot * Elist, knot * prev) {
    struct knot *now = calloc(1, sizeof(struct knot));

    printf("knot = %p key = %d value = %s\n", now, key, info);

    now->key = key;
    now->n_inf = 1;
    now->info = calloc(1, sizeof(char*));
    now->info[0] = strdup(info);

    now->left = Elist;
    now->right = Elist;
    now->color = 1;
    now->prev = prev;
    now->root = NULL;

    return now;
}

int adding_knot(knot* Elist, int key1, char *info1) {

	struct knot *now = Elist->root;

	while (now != NULL) {
		if ((key1 > now->key) && now->right) {
			now = now->right;
		}
		else if ((key1 < now->key) && now->left) {
			now = now->left;
		}
		else if ((key1 > now->key) && (now->right == NULL)) {
			now->right = knot_new(key1, info1, strlen(info1), Elist, now);
			Fixing_in(Elist, now->right);
			break;
		}
		else if ((key1 < now->key) && (now->left == NULL)) {
			now->left = knot_new(key1, info1, strlen(info1), Elist, now);
			Fixing_in(Elist, now->left);
			break;
		}
		else if (key1 == now->key)
		{
			now->n_inf = now->n_inf + 1;
			now->info = realloc(now->info, (now->n_inf)*sizeof(char *));
			now->info[now->n_inf-1] = strdup(info1);
			break;
		}
	}

    if (Elist->root == NULL) {
        Elist->root = knot_new(key1, info1, strlen(info1), Elist, NULL);
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
 * Удаляет узел по его ключу
 *
**/
int del_knot(struct knot *Elist, int key2) {
        char ** info_save;
	knot * root = Elist->root;
	if (Elist->root==NULL) {
		return E_OK;
	}

	if ((root->key == key2) && (root->left==Elist) && (root->right==Elist)) {
		printf("knot = %p key = %u case = root\n", root, key2);
		free(Elist->root);
		return E_OK;
	}

	knot * prev;
	int l_or_r = -1;
	knot * now = root;
	knot* save_now = NULL;

	while(1) {
		if ((key2 > now->key) && now->right!=Elist) {
			prev = now;
			l_or_r = L_RIGHT;
			now = now->right;
		}
		else if((key2 < now->key) && now->left!=Elist) {
			prev = now;
			l_or_r = L_LEFT;
			now = now->left;
		}
		else if((key2 > now->key) && (now->right == Elist)) {
			printf("warn: no knot available: key = %u\n", key2);
			return 1;
		}
		else if(key2<now->key && now->left == Elist) {
			printf("warn: no knot available: key = %u\n", key2);
			return 1;
		}
		else if(key2 == now->key) {
			if((now->left==Elist) && (now->right==Elist)) {
				if(l_or_r == L_RIGHT)
					prev->right = Elist;
				else
					prev->left = Elist;
				printf("knot = %p key = %u case = both\n", now, now->key);
				for(int i=0; i<now->n_inf; i++)
					free(now->info[i]);
				free(now->info);
				free(now);
				break;
			}
			else if((now->left == Elist) && now->right!=Elist)
			{
				if (l_or_r==-1)
				{
					root->key = root->right->key;
					root->info = root->right->info;
					save_now = root->right;
 					root->left = save_now->left;
					root->right = save_now->right;
					save_now->right = Elist;
					printf("knot = %p key = %u case = right\n", save_now, save_now->key);
					free(save_now);
				}
				else if(l_or_r == L_LEFT)
				{
					prev->right = now->right;
					now->right=Elist;
					printf("knot = %p key = %u\n", now, now->key);
					free(now);
				}
				else
				{
					prev->left = now->right;
					now->right=Elist;
					printf("knot = %p key = %u\n", now, now->key);
					free(now);
				}
				break;
			}
			else if((now->right == Elist) && now->left!=Elist) {
				if (l_or_r == -1) {
					root->key = root->left->key;
					root->info = root->left->info;
					save_now = root->left;
					root->left = save_now->left;
					root->right = save_now->right;
					save_now->left=Elist;
					printf("knot = %p key = %u case = left1\n", save_now, save_now->key);
					free(save_now);
				}
				else if(l_or_r == 0) {
					prev->right = now->left;
					now->left = Elist;
					printf("knot = %p key = %u case = left2\n", now, now->key);
					free(now);
				}
				else if(l_or_r == 1) {
					prev->left = now->left;
					now->left = Elist;
					printf("knot = %p key = %u case = left3\n", now, now->key);
					free(now);
				}
				break;
			}
			else if((now->right != Elist) && (now->left != Elist)) {
				int key_save = 0;
				knot *post = now->right;
				knot *post_prev = now;
				while(post->left) {
					post_prev = post;
					post = post->left;
				}
				key_save = post->key;
				char ** info_save = realloc(info_save, post->n_inf*sizeof(char*));
				for (int j=0; j<now->n_inf; j++)
				{
					info_save[j] = calloc(strlen(post->info[j]), sizeof(char));
					for (int q=0; q<strlen(post->info[j]); q++)
					{
						info_save[j][q] = post->info[j][q];
					}
				}
				if(post_prev != now)
				{
					post_prev->left = post->right;
				}
				else
				{
					post_prev->right=post->right;
				}
				post->right = Elist;
				printf("knot = %p key = %u case = both\n", post, post->key);
				free(post);
				now->key = key_save;
				for (int j=0; j<post->n_inf; j++)
				{
					for (int q=0; q<strlen(post->info[j]); q++)
					{
						post->info[j][q] = info_save[j][q];
						free(post->info[j]);
					}
				}			
				break;
			}
		}
	}
	return 0;
}

void OBXOD(knot * now3, knot * Elist)
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
		if (now3->left !=Elist)
		{
			OBXOD(now3->left, Elist);
		}
		printf("Found Key: %d and Info: %d and their link %p: %d\n", now3->key, now3->info, now3);
		return ;
	}
}

void show_node(struct knot *cur) {
    if (cur != NULL) {
        printf("root: \n");
        printf("-----\n");
        printf("| %d |\n", cur->key);
        for(int n=0; n < cur->n_inf; n++) {
            printf("| %s |\n", cur->info[n]);
        }
        printf("-----\n");
    } else {
        printf("root: empty\n");
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
				if(now == NULL)
				{
					err=1;
					break;
				}
				now = now->left;
			}
			else
			{
				if(now==NULL)
				{
					err=1;
					break;
				}
				now=now->right;
			}
			i1 = i1%Pow(10, jn-j);
		}
		if(now)
		{
			printf("%d ", now->key);
			printf("%d | ", now->info);
		}
		else if(err==1 || now==NULL)
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
	knot * now=Elist;
	knot* prev=NULL;
	knot* root = Elist->root;
	if(root==NULL)
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
		//	printf("element not found");
			return 1;
		}
		else if(key6<now->key && now->left == Elist)
		{
		//	printf("element not found");
			return 1;
		}	
		else if(key6==now->key)
		{
			for (int i=0; i<now->n_inf; i++){
				printf("found element: ");
				for(int j=0; j<strlen(now->info[i]); j++)
				{
					printf("%d", now->info[i][j]);
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
	while(key7 != now->key)
	{
		if(key7 > now->key)
			now = now->right;
		else
			now = now->left;
	}
	if(abs(now->right->key - now->key)<abs(now->key - now->left->key) && abs(now->right->key - now->key)<abs(now->key - now->prev->key))
		find_by_key(Elist, now->right->key);
	if(abs(now->prev->key - now->key)<abs(now->key - now->right->key) && abs(now->prev->key - now->key)<abs(now->key - now->left->key))
		find_by_key(Elist, now->prev->key);
	if(abs(now->left->key - now->key)<abs(now->key - now->right->key) && abs(now->left->key - now->key)<abs(now->key - now->prev->key))
		find_by_key(Elist, now->left->key);
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

int Fixing_in(knot * Elist, knot * new)
{
	if(new->color == 1)
	{
		if(new->prev->prev->right->color == 1 && new->prev->prev->color == 0 && new->prev->prev->left->color==1)
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
		new->prev->left	= left_turn(new->prev->left);
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
