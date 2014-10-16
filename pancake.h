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
	unsigned int size;
	int * pancakes;
};

#endif /* PANCAKE_H_ */
