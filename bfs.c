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
bool check(Game *temp);

void initialize_search(Game *game) {
	Game ** temp;
	Game ** next;
	int size;
	int i;
	moves=0;
	g = game;

	size = g->size - 1;

	temp = malloc(sizeof(Game *) * size);
	for (i = 0; i < size; i++) {
		temp[i] = malloc(sizeof(Game));
		temp[i]->size = g->size;
		temp[i]->pancakes = malloc(sizeof(int) * g->size);
		memcpy(temp[i]->pancakes, g->pancakes,
				sizeof(int) * g->size);
		flip(temp[i], (i % (g->size-1))+1);
		if(check(temp[i])) {
			printf("winner in %u moves\n", moves);
			exit(0);
		}

	}
	moves++;

	while(true) {
		next = gen(temp, size);
		_free(temp, size);
		size = (g->size-1) * size;
		temp = next;
		moves++;
		printf("%u\n", moves);
	}
}

Game ** gen(Game **array, int size) {
	Game ** temp;
	int pancakes = array[0]->size;
	int csize;
	int i;

	csize = (pancakes-1)*size;

	temp = malloc(sizeof(Game *) * csize);
	for(i=0; i<csize; i++) {
		temp[i] = malloc(sizeof(Game));
		temp[i]->size = pancakes;
		temp[i]->pancakes = malloc(sizeof(int) * pancakes);
		memcpy(temp[i]->pancakes, array[i/(pancakes-1)]->pancakes, sizeof(int) * pancakes);
		flip(temp[i], (i % (pancakes-1))+1);
		if (check(temp[i])) {
			printf("winner in %u moves\n", moves);
			exit(0);
		}
	}

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
