#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lista.h"

#ifndef Coada
#define Coada

#define MemoryError -1

//Strctura pentru coada
typedef struct 
{
	size_t dime;
	TLG vf, sf;
}TCoada;

//Pointer la functia de afisare a elementelor din stiva
void (*AfiEl)(void *, FILE *);

//Pointer la functia de comparare a id-urilor parantezelor din stiva
int (*CompEl)(void *, void *);

int MatchBracket(void *first, void *second);
int CheckBracket(void *bracket, int type);

TCoada *InitC(size_t d);
void Intrq(TCoada *cd, void *bracket);
void Extrq(TCoada *cd);
void PrintCoada(TCoada ***Queues, int C, FILE *out);
void SortCoada(TCoada **coada, FILE *out);
void CoadaCorecta(TCoada *coada, FILE *out);

#endif