/*
 * bfs.c
 *
 *  Created on: Oct 19, 2014
 *      Author: ryan
 */


#include "pancake.h"

Game *g;
int moves;

Game ** gen(Game **array, int size);
void _free(Game **temp, int size);
Game *minEntropy(Game **array, int size);
Game *minEntropy2(Game **array, int size);

Game *initialize_search(Game *game, int *movesOverall) {
	Game *tempWinner;
	Game *winner;
	Game ** temp;
	Game ** next;
	char ctemp[10];
	int size;
	int i;
	int idxFlip=0;
	moves=0;
	g = game;

	winner = game;
	//If this state is because of a flip do not repeat flip
	if(game->flips)
		size = g->size - 2;
	else
		size = g->size - 1;

	//printf("Size of array : %u - Last flip %u\n", size, game->flips);

	temp = malloc(sizeof(Game *) * size);
	for (i = 0; i < size; i++) {
        temp[i] = makeGame(g);	
		if(game->flips) {
			idxFlip = (i % (g->size-2))+1;
			if(idxFlip >= game->flips)
				idxFlip++;
			flip(temp[i], idxFlip);
			temp[i]->flips = idxFlip;
		} else {
			idxFlip = (i % (g->size-1))+1;
			flip(temp[i], idxFlip);
			temp[i]->flips = idxFlip;
		}
		sprintf(ctemp, "%u ", idxFlip);
		appendMove(temp[i], ctemp);
	}
	tempWinner = minEntropy2(temp, size);
	if(calcEntropy2(tempWinner) < calcEntropy2(winner)) {
		winner = tempWinner;
	}
	moves++;

	i=0;
	if (!check(winner))
	while(i < 6) {
		next = gen(temp, size);
		_free(temp, size);
		tempWinner = minEntropy2(next, size);
		if(calcEntropy2(tempWinner) < calcEntropy2(winner)) {
			if(winner != game) {
				free(winner->pancakes);
				free(winner);
			}
			winner = tempWinner;
			if (check(winner)) {
				temp = next;
				moves++;
				break;
			}

		}
		size = (g->size-2) * size;
		temp = next;
		i++;
		moves++;
	}
	_free(temp, size);
	*movesOverall = *movesOverall + moves;
	return winner;
}

Game **reduce(Game **array, int *size, Game *winner) {

}

Game *minEntropy(Game **array, int size) {
	int i;
	double minEntropy;
	double tempEntropy;
	Game *max, *copy;

	copy = malloc(sizeof(Game));
	i = rand() % size;
	minEntropy = calcEntropy(array[i]);
	max = array[i];
	for(i=0; i<size; i++) {
		tempEntropy = calcEntropy(array[i]);
		if(minEntropy > tempEntropy) {
			minEntropy = tempEntropy;
			max = array[i];
		}
	}
	printf("Depth %u MinEntropy %f\n", moves, minEntropy);
	copy->pancakes = malloc(sizeof(int) * max->size);
	for(i=0; i<max->size; i++)
		copy->pancakes[i] = max->pancakes[i];
	copy->size = max->size;
	copy->flips = max->flips;

	return copy;
}

Game *minEntropy2(Game **array, int size) {
	int i;
	int minEntropy;
	int tempEntropy;
	Game *max, *copy;

	copy = malloc(sizeof(Game));
	i = rand() % size;
	minEntropy = calcEntropy2(array[i]);
	max = array[i];
	for(i=0; i<size; i++) {
		tempEntropy = calcEntropy2(array[i]);
		if(minEntropy > tempEntropy) {
			minEntropy = tempEntropy;
			max = array[i];
		}
	}

    copy->moves[0] = '\0';
	appendMove(copy, max->moves);
	copy->pancakes = malloc(sizeof(int) * max->size);
	for(i=0; i<max->size; i++)
		copy->pancakes[i] = max->pancakes[i];
	copy->size = max->size;
	copy->flips = max->flips;

	return copy;
}

Game ** gen(Game **array, int size) {
	Game ** temp;
	char ctemp[10];
	int pancakes = array[0]->size;
	int csize;
	int idxFlip;
	int i;

	//don't repeat the last move
	csize = (pancakes-2)*size;

	temp = malloc(sizeof(Game *) * csize);
	for(i=0; i<csize; i++) {
		temp[i] = malloc(sizeof(Game));
		temp[i]->size = pancakes;
		temp[i]->pancakes = malloc(sizeof(char) * pancakes);
        temp[i]->curSize = array[i/(pancakes-2)]->curSize;
        temp[i]->maxSize = array[i/(pancakes-2)]->maxSize;
		memcpy(temp[i]->pancakes, array[i/(pancakes-2)]->pancakes, sizeof(char) * pancakes);
		strcpy(temp[i]->moves, array[i/(pancakes-2)]->moves);
		idxFlip = (i % (pancakes-2))+1;
		if(idxFlip >= array[i/(pancakes-2)]->flips)
			idxFlip++;
		temp[i]->flips = idxFlip;
		flip(temp[i], idxFlip);
		sprintf(ctemp, "%u ", idxFlip);
		appendMove(temp[i], ctemp);
	}
	//printf("\n");
	return temp;
}

void _free(Game **temp, int size) {
	int i;
	for(i=0; i<size; i++) {
		free(temp[i]->pancakes);
		free(temp[i]);
	}
	free(temp);
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

void appendMove(Game* g, char* move) {
    if (g->curSize >= g->maxSize) {
        g->maxSize = g->maxSize * 2;
        char* moves = (char*) malloc(g->maxSize);
        strcpy(moves, g->moves);
        free(g->moves);
        g->moves = moves;
    }
    strcat(g->moves, move);
    g->curSize++;
}

Game* makeGame(Game* g) {
    Game* temp = (Game*) malloc(sizeof(Game));
	temp->size = g->size;
	temp->pancakes = (char*) malloc(sizeof(char) * g->size);
	memcpy(temp->pancakes, g->pancakes, sizeof(char) * g->size);
    temp->maxSize = 1;
    temp->curSize = 0;
    temp->moves = (char*) malloc(sizeof(char) * 3);
	temp->moves[0] = '\0';
    return temp;
}

