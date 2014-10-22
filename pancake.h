/*
 * pancake.h
 *
 *  Created on: Oct 16, 2014
 *      Author: ryan
 */

#ifndef PANCAKE_H_
#define PANCAKE_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

typedef struct Game Game;

struct Game /* the state of the game at each possible move */
{
	char moves[22];
	unsigned int size;
	unsigned int flips;
	int * pancakes;
};

Game *initialize_search(Game *game, int *movesOverall);
void flip(Game *game, int flip);
double calcEntropy(Game *g);
int calcEntropy2(Game *g);
bool check(Game *temp);
void printGame(Game *g);

#endif /* PANCAKE_H_ */
