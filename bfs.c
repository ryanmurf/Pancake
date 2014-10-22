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
		temp[i] = malloc(sizeof(Game));
		temp[i]->size = g->size;
		temp[i]->pancakes = malloc(sizeof(int) * g->size);
		memcpy(temp[i]->pancakes, g->pancakes, sizeof(int) * g->size);
		temp[i]->moves[0] = '\0';
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
		strcat(temp[i]->moves, ctemp);
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

	for(i=0; i<22; i++)
		copy->moves[i] = '\0';
	strcpy(copy->moves, max->moves);
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
		temp[i]->pancakes = malloc(sizeof(int) * pancakes);
		memcpy(temp[i]->pancakes, array[i/(pancakes-2)]->pancakes, sizeof(int) * pancakes);
		strcpy(temp[i]->moves, array[i/(pancakes-2)]->moves);
		idxFlip = (i % (pancakes-2))+1;
		if(idxFlip >= array[i/(pancakes-2)]->flips)
			idxFlip++;
		temp[i]->flips = idxFlip;
		flip(temp[i], idxFlip);
		sprintf(ctemp, "%u ", idxFlip);
		strcat(temp[i]->moves, ctemp);
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