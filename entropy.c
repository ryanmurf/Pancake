#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "pancake.h"

#define MAXLEN 250 //maximum string length

static int makehist(int *S,int *hist,int len);
static double entropy(int *hist,int histlen,int len);
static int *_diff(Game *g);

static int makehist(int *S,int *hist,int len){
	int wherechar[256];
	int i,histlen;
	histlen=0;
	for(i=0;i<256;i++)wherechar[i]=-1;
	for(i=0;i<len;i++){
		if(wherechar[S[i]]==-1){
			wherechar[S[i]]=histlen;
			histlen++;
		}
		hist[wherechar[S[i]]]++;
	}
	return histlen;
}
 
static double entropy(int *hist,int histlen,int len){
	int i;
	double H;
	H=0;
	for(i=0;i<histlen;i++){
		H-=(double)hist[i]/len*log2((double)hist[i]/len);
	}
	return H;
}

double calcEntropy(Game *g) {
	double entro=0;
	int *hist, *diff, histlen;

	hist=(int*)calloc((g->size-1),sizeof(int));
	diff = _diff(g);
	histlen = makehist(diff,hist,(g->size-1));
	entro = entropy(hist,histlen,g->size);

	free(hist);
	free(diff);
	return entro;
}

int calcEntropy2(Game *g) {
	int entro=(g->size-1),i;
	int *diff;

	diff = _diff(g);
	for(i=0; i<(g->size-1); i++) {
		if(diff[i] == 1)
			entro--;
	}

	free(diff);

	return entro;
}

static int *_diff(Game *g) {
	int *diff;
	int i=0;

	diff=(int*)calloc((g->size-1),sizeof(int));
	for(i=0; i<(g->size-1); i++) {
		diff[i] = abs(g->pancakes[i+1] - g->pancakes[i]);
	}

	return diff;
}
 
/*int main(void){
	char * pch;
	char S[MAXLEN];
	int temp[100];
	int i,len=0,*hist, *data,histlen;
	double H;
	scanf("%[^\n]",S);

	pch = strtok(S, " ");
	while (pch != NULL)
	{
		temp[len] = atoi(pch);
		len++;
		if(len > 100)
			exit(0);
		pch = strtok (NULL, " ");
	}
	data=(int*)malloc(len*sizeof(int));
	hist=(int*)calloc(len,sizeof(int));
	for(i=0; i<len; i++)
		data[i] = temp[i];



	histlen=makehist(data,hist,len);
	printf("length %u. Histogram Length %u\n", len, histlen);

	//hist now has no order (known to the program) but that doesn't matter
	H=entropy(hist,histlen,len);
	printf("%lf\n",H);
	return 0;
}*/
