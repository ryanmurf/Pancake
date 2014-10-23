/*
 * pancake.c
 *
 *  Created on: Oct 16, 2014
 *      Author: ryan
 */


#include "pancake.h"
#include <time.h>

//buffer used by input statements
char inbuf[1024];
char _file[1024];

int nGames=0;
Game *games[13];

void init_args(int argc, char **argv);
void readGameFile(const char *s);

int max(Game *g, int n);
void pancakeFlipSort(Game *g);

int main(int argc, char *argv[]) {
	int k=0, gg=0;
	Game *g;
	nGames = 0;

	init_args(argc, argv);

	srand(time(NULL));

	readGameFile(_file);
	for (gg = 0; gg < 13; gg++) {
		printf("Entropy of Game %u Size %u : Entropy %u\n", gg+1, games[gg]->size, calcEntropy2(games[gg]));
		//printGame(games[gg]);
		g = games[gg];

		g = initialize_search(g, &k);
		if (check(g)) {
			printf("%s0 (%u)\n", g->moves, k);
		} else {
			printf("%s", g->moves);
        }
        g->moves[0] = '\0';

		if(g != games[gg]) {
			free(g->pancakes);
			free(g->moves);
			free(g);
		}
		k=0;
	}
	//Trivial Solution
	/*printf("Games : %u\n", nGames);

	for(i=0; i<nGames; i++) {
		printf("Game %u - Size %u\n", i+1, games[i]->size);
		g = games[i];
		for(j=0; j<g->size; j++) {
			printf("%u ", g->pancakes[j]);
		}
		printf("\n");
		pancakeFlipSort(g);
		printf("0 (%u)\n", g->flips);
		//for (j = 0; j < g->size; j++) {
		//	printf("%u ", g->pancakes[j]);
		//}
		printf("\n");
	}*/

	return 0;
}

void printGame(Game *g) {
	int j=0;
	for (j = 0; j < g->size; j++) {
		printf("%u ", g->pancakes[j]);
	}
	printf("\n");
}

bool GetALine(FILE *f, char buf[]) {
	/* Read a line of possibly commented input from the file *f.*/
	char *p;
	bool not_eof = false;
	while ( fgets(buf, 1024, f) != NULL) {
		p=strchr(buf, (int) '\n');
		if ( p != NULL )
			*p = '\0';

		if (*buf != '\0') {
			not_eof = true;
			break;
		}
	}
	return (not_eof);
}

void readGameFile(const char *s) {
	FILE *f;
	int lineno=0, i=0;
	char * pch;
	Game *game;
	char buf[1024];

	int gameSize = 0;

	if(s==NULL)
		exit(1);

	f = fopen(s, "r");
	if(f == NULL) {
		printf("Could not open file");
		exit(1);
	}

	while(GetALine(f, inbuf)) {
		game = malloc(sizeof(Game));

		strcpy(buf,inbuf);

		pch = strtok(inbuf, " ");
		while (pch != NULL)
		{
			gameSize++;
			pch = strtok (NULL, " ");
		}
		game->size = gameSize;
		game->flips = 0;
		game->pancakes = malloc(sizeof(int) * gameSize);
		//game->moves[0] = '\0';
		gameSize = 0;

		i=0;
		pch = strtok(buf, " ");
		while (pch != NULL) {
			game->pancakes[i] = atoi(pch);
			i++;
			pch = strtok(NULL, " ");
		}

		games[nGames] = game;
		nGames++;

		lineno++;
	}
}

void init_args(int argc, char **argv) {

	char str[1024], *opts[] = { "-f" }; /* valid options */
	int valopts[] = { 1 }; /* indicates options with values */
	/* 0=none, 1=required, -1=optional */
	int i, /* looper through all cmdline arguments */
	a, /* current valid argument-value position */
	op, /* position number of found option */
	nopts = sizeof(opts) / sizeof(char *);

	a = 1;
	for (i = 1; i <= nopts; i++) {
		if (a >= argc)
			break;

		/* figure out which option by its position 0-(nopts-1) */
		for (op = 0; op < nopts; op++) {
			if (strncmp(opts[op], argv[a], 2) == 0)
				break; /* found it, move on */
		}
		if (op == nopts) {
			fprintf(stderr, "Invalid option %s\n", argv[a]);
			printf("%s", "Pancake requires an input file. Use -f filename.txt");
			exit(-1);
		}

		*str = '\0';
		/* extract value part of option-value pair */
		if (valopts[op]) {
			if ('\0' != argv[a][2]) { /* no space betw opt-value */
				strcpy(str, (argv[a] + 2));
			} else if ('-' != *argv[a + 1]) { /* space betw opt-value */
				strcpy(str, argv[++a]);
			} else if (0 < valopts[op]) { /* required opt-val not found */
				fprintf(stderr, "Incomplete option %s\n", opts[op]);
				printf("%s", "Pancake requires an input file. Use -f filename.txt");
				exit(-1);
			} /* opt-val not required */
		}

		/* tell us what to do here                   */
		/* set indicators/variables based on results */
		switch (op) {
		case 0: /* -d */
			strcpy(_file, str);
			break; /* -f */
		default:
			fprintf(stderr, "Programmer: bad option in main:init_args:switch");
		}

		a++; /* move to next valid arg-value position */

	} /* end for(i) */
}

/*
 * This will flip the stack of pancakes at flip.
 * pancakes are ordered top.......bottom.
 * flip(,1) will flip the entire stack.
 * 1 to size are possible flip values.
 */
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

/*
 * This will find the max from top, index 0, to (n-1).
 * will return the index
 */
int max(Game *g, int n) {
	int idxn, i;
	for(idxn=0, i=0; i<n; i++)
		if(g->pancakes[i] > g->pancakes[idxn])
			idxn = i;
	return idxn;
}

/*
 * This will flip the largest to the bottom of the pile
 * repeated until done.
 */
void pancakeFlipSort(Game *g) {
	int i;
	int idxMax;

	for(i=g->size; i>1; i--) {
		idxMax = max(g,i);

		if(idxMax != (i-1)) {
			flip(g, g->size-idxMax);
			flip(g, g->size-(i-1));
		}
	}
}
