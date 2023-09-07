#include "lib.h"

//static knot* root = NULL;
static knot* Elist = NULL;

int func4(const char *input_file) {
	FILE * f_input;
	int k_or_i=0;
	int key4=0;
	char * info4 = calloc(2, sizeof(char));
	int s_of_i = 1;
	int ready=0;
	char symb;
//	char * T_Null = {'\0'};
//	char * symb_str = calloc(1, sizeof(char));
	if ((f_input = fopen(input_file, "r")) == NULL) {
		fprintf(stderr, "Unable to open file %s to read\n", input_file);
		return 2;
	}

	while(fscanf(f_input, "%c", &symb) != EOF) {
//		printf("value = %u\n", symb);

		if(symb == '\n')
		{
			k_or_i++;
			if(k_or_i % 2 == 0)
			{
				if (ready==0)
				{
					ready=1;
					Elist->root = calloc(1, sizeof(knot));
					Elist->root->info = calloc(1, sizeof(char*));
					Elist->root->info[0] = calloc(s_of_i, sizeof(char));
					Elist->root->n_inf = 1;
					strcpy(Elist->root->info[0], info4);
					Elist->root->info[0][s_of_i-1]='\0';
					//printf("|Inputed k:%d i:%d|\n", key4, info4);
					Elist->root->key=key4;
					Elist->root->color = 0;
					Elist->root->right = Elist;
					Elist->root->left = Elist;
					Elist->root->root = NULL;
				}
				else
				{
					adding_knot(Elist, key4, info4);
				}
				s_of_i = 1;
				info4 = realloc(info4, s_of_i*sizeof(char));
			}
		}
		else {
			if (k_or_i%2==0) {
				int znach = (int)(symb);
				key4 = key4*10 + znach - 48;
//				printf("value = %u\n", key4);
			}
			if (k_or_i%2==1) {
				s_of_i++;
//				symb_str[0] = symb;
				info4 = realloc(info4, s_of_i*sizeof(char));
			 	info4[s_of_i-2] = symb;
			}
		}
	}
	adding_knot(Elist, key4, info4);
//	printf("Last: key = %u value = %u\n", key4, info4);
//	free(symb_str);
	fclose(f_input);
	return 0;
}

int main(int argc, char *argv[])
{
	time_t start0, stop0;
	char *input_file;
	int key6;

	start0 = time(NULL);

	//Show_possibilities();

	//root = calloc(1, sizeof(knot));
	Elist = calloc(1, sizeof(knot));

	int comm=0;
/*
	root->key = 0;
	root->info = calloc(1, sizeof(knot));
	root->color = 0;
	root->right = Elist;
	root->left = Elist;
	root->root = NULL;
	root->prev = Elist;
*/
	Elist->info = NULL;
	Elist->key = 0;
	Elist->n_inf = 0;
	Elist->root = NULL;
	Elist->prev = NULL;
	Elist->color = 0;
	Elist->right = NULL;
	Elist->left = NULL;

	char ** info_save = calloc(1, sizeof(char*));

	char * charr = calloc(1, sizeof(char));

	int key1, key2;
	int key4=0;
	int k_or_i=0;
	int ready=0;
	char * info1 = calloc(1, sizeof(char));

	int stopper = 0;
	while (stopper==0) {
		Show_possibilities();
		scanf("%d", &comm);
		switch (comm) {
		case 0:
			stopper = 1;
			time_t start0 = time(NULL);
			if(Elist->root)
			{
				while (del_knot(Elist, Elist->root->key, info_save) != 25);
			}
			else
			{
				free(Elist);
			}
			time_t stop0 = time(NULL);
			double duration0 = stop0 - start0;
			printf("Time of deleating: %10.f seconds\n", duration0);
			free(charr);
			free(info_save);
			free(info1);
			break;
		case 1:
			printf("Input key of your knot\n");
			scanf("%d", &key1);
			int lenn = 1;
			char charr0;
			printf("Input info in your knot\n");
			while(scanf("%c", &charr0)!='\n')
			{
				charr[0] = charr0;
				info1 = realloc(info1, lenn*sizeof(char));
				lenn++;
				if(lenn == 2)
					strcpy(info1, charr);
				else
					strcat(info1, charr);
			}
			adding_knot(Elist, key1, info1);
			break;
		case 2:
			printf("Input key of deleatable element\n");
			scanf("%d", &key2);
			time_t start2 = time(NULL);
			del_knot(Elist, key2, info_save);
			time_t stop2 = time(NULL);
			double duration2 = stop2 - start2;
			printf("Time for deleat element: %10.f seconds\n", duration2);
			break;
		case 3:
			time_t start3 = time(NULL);
			OBXOD(Elist->root, Elist);
			time_t stop3 = time(NULL);
			double duration3 = stop3 - start3;
			printf("Time of checking: %10.f seconds\n", duration3);
			break;
		case 4:
			input_file = argv[optind];
			time_t start4 = time(NULL);
			if (input_file == NULL)
			{
				perror("Missed file");
				return 1;
			}
			func4(input_file);
			time_t stop4 = time(NULL);
			double duration4 = stop4 - start4;
			printf("Time of downloading: %10.f seconds\n", duration4);
			break;
		case 5:
			Show_graph(Elist);
			break;
		case 6:
			int key6 = 0;
			printf("Input key of findouble element\n");
			scanf("%d", &key6);
			find_by_key(Elist, key6);
			break;
		case 7:
			int key7 = 0;
			printf("Input key\n");
			scanf("%d", &key7);
			find_by_near_key(Elist, key7);
			break;
		}
	}
	return 0;
}
