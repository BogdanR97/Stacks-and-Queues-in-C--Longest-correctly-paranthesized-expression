#include "Lista.h"

#define MemoryError -1

//Functie de alocare a unei celule din Lista Generica
TLG AlocaCelula(void *el, size_t d)
{
	TLG aux;
	aux=(TLG)malloc(sizeof(TCelulaG));
	if(!aux)
	{
		free(aux);
		fprintf(stderr, "%d\n", MemoryError );
		exit(EXIT_FAILURE);
	}
	aux->urm=NULL;
	aux->info=malloc(d);
	if(!aux->info)
	{
		free(aux);
		fprintf(stderr, "%d\n", MemoryError );
		exit(EXIT_FAILURE);
	}
	memcpy(aux->info,el,d);
	return aux;
}