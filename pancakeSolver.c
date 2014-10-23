/*
 * pancakeSolver.c
 *
 *  Created on: Oct 23, 2014
 *      Author: ryan
 */

#include "pancake.h"

Game *game;
static int getLine (char *prmpt, char *buff, size_t sz);

int main(int argc, char *argv[]) {
	int len=0, i=0;
	char * pch;
	char gameB[1024] = {'\0'};
	char moves[1024] = {'\0'};
	char temp[120];
	printf("\n");
	getLine("Please paste game and press enter\n", gameB, 1024);
	//scanf("%[^\n]",gameB);


	pch = strtok(gameB, " ");
	while (pch != NULL) {
		temp[len] = atoi(pch);
		len++;
		if (len > 100) {
			printf("Game to large.\n");
			exit(0);
		}
		pch = strtok(NULL, " ");
	}

	game = malloc(sizeof(Game));
	game->pancakes = malloc(sizeof(char) * len);
	for(i=0; i<len; i++)
		game->pancakes[i] = temp[i];
	game->flips = 0;
	game->curSize = 0;
	game->size = len;

	printf("Game of size %u:\n", game->size);
	printGame(game);

	//printf("\n");
	getLine("Enter Moves Then Press Enter\n", moves, 1024);
	len=0;
	pch = strtok(moves, " ");
	while (pch != NULL) {
		i = atoi(pch);
		flip(game, atoi(pch));
		printf("Flip %u : ",i);
		printGame(game);
		len++;
		pch = strtok(NULL, " ");
	}

	if(check(game)) {
		printf("Game Solved. Moves : %u\n",len);
	} else {
		printf("Game not solved.");
		printGame(game);
	}
}

void printGame(Game *g) {
	int j=0;
	for (j = 0; j < g->size; j++) {
		printf("%u ", g->pancakes[j]);
	}
	printf("\n");
}

void flip(Game *game, int flip) {
	int i,end, temp;

	if(flip==game->size)
		return;
	if(flip < 0 || flip > game->size) {
		printf("Invalid flip index in flip.");
		exit(1);
	}


	end = game->size - flip;

	for(i=0; i<end; i++, end--) {
		temp = game->pancakes[i];
		game->pancakes[i] = game->pancakes[end];
		game->pancakes[end] = temp;
	}

	//printf("%u ", flip);
	game->flips++;
}

bool check(Game *temp) {
	int i;
	for(i=temp->size; i>0; i--) {
		if(temp->pancakes[i-1] == i)
			continue;
		else
			return false;
	}
	return true;
}

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2
static int getLine (char *prmpt, char *buff, size_t sz) {
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}
