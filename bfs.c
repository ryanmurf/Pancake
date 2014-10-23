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
void _freeArray(Game **temp, int size);
Game *minEntropy(Game **array, int size);
Game *minEntropy2(Game **array, int size);
Game **reduce(Game **array, int *size, Game **winner);
void _freeGame(Game *temp);

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
	if((int)(game->flips))
		size = g->size - 2;
	else
		size = g->size - 1;

	//printf("Size of array : %u - Last flip %u\n", size, game->flips);

	temp = malloc(sizeof(Game *) * size);
	for (i = 0; i < size; i++) {
        temp[i] = makeGame(g);	
		if((int)game->flips) {
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
	while(i < (2*g->size - 1)) {
		next = gen(temp, size);
		_freeArray(temp, size);
		size = (g->size-2) * size;
		next = reduce(next, &size, &winner);
		if(next == NULL) {
			if(!check(winner)) {
				flip(winner, 1);
				sprintf(ctemp, "%u ", 1);
				appendMove(winner, ctemp);
				moves++;
			}
			temp = next;
			moves++;
			break;
		}
		temp = next;
		i++;
		moves++;
	}
	if(temp != NULL)
		_freeArray(temp, size);
	*movesOverall = *movesOverall + moves;
	return winner;
}

Game **reduce(Game **array, int *size, Game **winner) {
	Game **reduced;
	int i,j;
	int idxReduced=0;
	int newSize;
	int keep;
	//First check the array for a lower entropy value
	//If we find one reduce the array to just that one.
	Game *tempWinner;
	tempWinner = minEntropy2(array, *size);

	if(calcEntropy2(tempWinner) < calcEntropy2(*winner)) {
		if(*winner != g) {
			_freeGame(*winner);
		}
		*winner = tempWinner;
		_freeArray(array,(*size));

		if (check(*winner) || calcEntropy2(*winner)==0) {
			return NULL;
		}
		reduced = (Game **) malloc(sizeof(Game *));
		reduced[0] = makeGame(*winner);
		free(reduced[0]->moves);
		reduced[0]->curSize = (*winner)->curSize;
		reduced[0]->moves = malloc(sizeof(char) * ((reduced[0]->curSize+1)*3));
		reduced[0]->moves[0] = '\0';
		reduced[0]->flips = (*winner)->flips;
		appendMove(reduced[0], (*winner)->moves);
		*size = 1;
		return reduced;
	}

	//The next array will have (nPancakes-2)*size
	//Lets set a hard limit for next array by reducing this one
	//(4Bytes+nPancakes+2*nPancakes*3)*1.5 million
	//Max case nPancakes=100
	//4+100+200*3=704Bytes * 1.5 million = 1GB of Memory
	//(nPancakes-2)*size=1,500,000 : size=(1500000)/(nP-2)
	newSize = (1500000)/((*winner)->size-2);
	if(newSize >= (*size))
		return array;

	reduced = (Game **) malloc(sizeof(Game *)*1500000);

	for(i=0; i<(*size); i++) {
		//Does it go into reduced or not
		keep = rand() % 100;
		if(keep <= ((newSize/(*size))*100 + 2)) {
			reduced[idxReduced++] = array[i];
			if(idxReduced == newSize) {
				//finish cleaning old array out
				for (j = i; j < (*size); j++) {
					_freeGame(array[i]);
				}
				free(array);
				*size = newSize;
				return reduced;
			}
		} else {
			_freeGame(array[i]);
		}
	}


	_freeArray(array,(*size));
	*size = newSize;
	return reduced;
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
	copy->pancakes = malloc(sizeof(char) * max->size);
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
	Game *min, *copy;

	copy = malloc(sizeof(Game));
	i = rand() % size;
	minEntropy = calcEntropy2(array[i]);
	min = array[i];
	for(i=0; i<size; i++) {
		tempEntropy = calcEntropy2(array[i]);
		if(minEntropy > tempEntropy) {
			minEntropy = tempEntropy;
			min = array[i];
		}
	}
	copy->curSize = min->curSize;
	copy->moves = (char*) malloc(sizeof(char) * ((min->curSize+1)*3));
	copy->moves[0] = '\0';
	strcpy(copy->moves, min->moves);
	copy->pancakes = malloc(sizeof(char) * min->size);
	for(i=0; i<min->size; i++)
		copy->pancakes[i] = min->pancakes[i];
	copy->size = min->size;
	copy->flips = min->flips;

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
		temp[i]->moves = malloc(sizeof(char) * ((array[i/(pancakes-2)]->curSize+1)*3));
        temp[i]->curSize = array[i/(pancakes-2)]->curSize;
        temp[i]->moves[0] = '\0';
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

void _freeArray(Game **temp, int size) {
	int i;
	for(i=0; i<size; i++) {
		_freeGame(temp[i]);
	}
	free(temp);
}

void _freeGame(Game *temp) {
	free(temp->pancakes);
	free(temp->moves);
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
    char* moves = (char*) malloc(sizeof(char) * (g->curSize+1) * 3);
    strcpy(moves, g->moves);
    free(g->moves);
    g->moves = moves;

    strcat(g->moves, move);
    g->curSize++;
}

Game* makeGame(Game* game) {
    Game* temp = (Game*) malloc(sizeof(Game));
	temp->size = game->size;
	temp->pancakes = (char*) malloc(sizeof(char) * game->size);
	memcpy(temp->pancakes, game->pancakes, sizeof(char) * game->size);

    temp->curSize = 0;
    temp->moves = (char*) malloc(sizeof(char) * 1);
	temp->moves[0] = '\0';
    return temp;
}

