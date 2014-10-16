/*
 * pancake.c
 *
 *  Created on: Oct 16, 2014
 *      Author: ryan
 */


#include "pancake.h"

//buffer used by input statements
char inbuf[1024];
char _file[1024];

int nGames=0;
Game *games[10];

void init_args(int argc, char **argv);
void readGameFile(const char *s);

int main(int argc, char *argv[]) {
	nGames = 0;
	init_args(argc, argv);

	readGameFile(_file);

	printf("Games : %u\n", nGames);
	printf("%s", _file);

	return 0;
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

		pch = strtok(inbuf, " ");
		while (pch != NULL)
		{
			gameSize++;
			pch = strtok (NULL, " ");
		}
		game->size = gameSize;
		game->pancakes = malloc(sizeof(int) * gameSize);
		gameSize = 0;

		i=0;
		pch = strtok(inbuf, " ");
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
