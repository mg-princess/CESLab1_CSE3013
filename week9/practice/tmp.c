#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Node {
	char name[16];
	int score;
	struct Node *next;
} Node;

int userNum;

int main() {
    Node *node = malloc(sizeof(*node));
    FILE *file;
	int i, userScore;
	char name[16];

	file = fopen("rank.txt", "r+");
	fscanf(file, "%d", &userNum);
	fgetc(file);
    printf("%d\n", userNum);
	Node *curNode = node, *newNode;

	for(i = 0; i<userNum; i++) {
		newNode = malloc(sizeof(*node));
		fscanf(file, "%s %d", name, &userScore);
		fgetc(file);
        
		strncpy(newNode->name, name, 16);
		newNode->name[16-1] = '\0';
		newNode->score = userScore;
		newNode->next = NULL;

		curNode->next = newNode;
		curNode = curNode->next;
	}
	fclose(file);

    printf("      name     |   score\n");
			printf("----------------------------\n");
			int X = 0, Y = 0;
			printf("X: "); X = getchar(); getc(); X-='0'+1;
			printf("Y: "); Y = getchar(); getc(); Y-='0'+1;
			if(X == '\n' - '0' - 1) X = 0;
			if(Y == '\n' - '0' - 1) Y = userNum;
            printf("%d %d\n", X, Y);
			if(X > Y || X*Y < 0){
				printf("search failure: no rank in the list\n");
				return;
			}
			int count = 0;
			curNode = node->next;
			while(curNode != NULL) {
				if(X <= count && count <= Y)
					printf("%-16s| %d\n", curNode->name, curNode->score);
				curNode = curNode->next;
                count++;
			}

    return 0;
}