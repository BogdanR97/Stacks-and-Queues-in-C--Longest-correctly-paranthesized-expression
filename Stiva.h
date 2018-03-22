#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lista.h"

#ifndef Stiva
#define Stiva

#define MemoryError -1

//Structura pentru stiva
typedef struct 
{
	size_t dime;
	TLG vf;
}TStiva;

//Pointer la functia de afisare a elementelor din stiva
void (*AfiEl)(void *, FILE *);

//Pointer la functia de comparare a id-urilor parantezelor din stiva
int (*CompEl)(void *, void *);

int MatchBracket(void *first, void *second);
int CheckBracket(void *bracket, int type);

TStiva *InitS(size_t d);
void Push(TStiva *stiva, void *bracket);
void Pop(TStiva *stiva);
void PrintStiva(TStiva ***Stacks, int S, FILE *out);
void SortStiva(TStiva **stiva);
void StivaCorecta(TStiva *stiva, FILE *out);

#endif