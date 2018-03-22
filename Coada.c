#include "Coada.h"
#include "Lista.h"
#include "Stiva.h"

//Functia de initializare a unei cozi din vectorul de cozi
TCoada *InitC(size_t d)
{
	TCoada* coada=malloc(sizeof(TCoada));
	if(!coada)
	{
		free(coada);
		fprintf(stderr, "%d\n", MemoryError );
		exit(EXIT_FAILURE);
	}
	coada->dime=d;
	coada->sf=NULL;
	coada->vf=NULL;

	return coada;
}

//Functia introduce in capul cozii un nou element, de tipul TLG , nestiind tipul campului
//destinat informatiei. Astfel functia capata caracter generic.
void Intrq(TCoada *cd, void *bracket)
{
	TLG aux=AlocaCelula(bracket, cd->dime);
	
	if((cd)->vf==NULL && (cd)->sf==NULL)
	{
		(cd)->vf=aux;
		(cd)->sf=aux;
	}						//Retin adresa primului element din coada(capul cozii) pentru ca de aici vom extrage elementele
	else					//Retin adresa ultimului element din coada(sfarsitul cozii), pentru introducerea de noi elemente in coada
	{	
		(cd)->sf->urm=aux;
		(cd)->sf=aux;
	}	
}

//Extrage elementul din capul cozii
void Extrq(TCoada *cd)
{
	if((cd)->vf==NULL)
		return;
	void *aux_info=malloc(cd->dime);
	TLG aux=(cd)->vf;
	(cd)->vf=aux->urm;
	if(cd->vf==NULL)
		cd->sf=NULL;
	memcpy(aux_info, aux->info, cd->dime);
	free(aux_info);
	free(aux);
}

//Functie de afisare a tuturor cozilor. Functia primeste drept argument vectorul de cozi
//si parcurgand vectorul, se va afisa fiecare coada in parte. Afisarea se face de la capul cozii catre final.
void PrintCoada(TCoada ***Queues, int C, FILE *out)
{
	int i;

	//Initializez un vector de cozi auxiliare ce ma va ajuta in afisarea tuturor cozilor
	TCoada **Coada_aux;
	Coada_aux=malloc(C*sizeof(TCoada*));
	if(!Coada_aux)
	{
		free(Coada_aux);
		fprintf(stderr, "%d\n", MemoryError );
		exit(EXIT_FAILURE);
	}

	for(i=0;i<C;i++)
		Coada_aux[i]=InitC((*Queues)[i]->dime);

	//Parcurg vectorul de cozi
	for(i=0; i<C; i++)
	{	
		//Afisez fiecare element din fiecare coada din vector
		fprintf(out, "\"");
		//Ca sa pot "parcurge" coada trebuie sa extrag din aceasta fiecare element pana cand
		//coada va fi nula. Pentru a nu pierde informatia prin extragere, pun fiecare element din capul cozii 
		//initiale in coada de pe aceeasi pozitie din coada auxiliara.

		while((*Queues)[i]->vf!=NULL)
		{	
			AfiEl((*Queues)[i]->vf->info, out);
			Intrq(Coada_aux[i], (*Queues)[i]->vf->info);
			Extrq((*Queues)[i]);
		}
		fprintf(out, "\"\n");
	}

	//La final, reatribui cozii initiale valoarea sa.
	*Queues=Coada_aux;
}

//Pentru sortarea cozii am folosit functia de sortare de la stiva, dupa ce am creat o stiva auxiliara
//cu elementele din coada, prin extragere rand pe rand a elementelor din capul cozii si introducerea lor in varful stivei.
void SortCoada(TCoada **coada, FILE *out)
{
	TStiva *aux_stiva=InitS((*coada)->dime);
	TStiva *rev_stiva=InitS((*coada)->dime);

	while((*coada)->vf!=NULL)
	{
		Push(aux_stiva, (*coada)->vf->info);
		Extrq(*coada);
	}

	SortStiva(&aux_stiva);

	//Dupa sortare am rasturnat stiva rezultat, iar elementele le-am introdus in coada initiala;
	while(aux_stiva->vf!=NULL)
	{
		Push(rev_stiva, aux_stiva->vf->info);
		Pop(aux_stiva);
	}

	while(rev_stiva->vf!=NULL)
	{
		Intrq(*coada, rev_stiva->vf->info);
		Pop(rev_stiva);
	}

	free(aux_stiva);
	free(rev_stiva);
}

//Functia specifica instructiunii correctq. Algoritmul este acelasi ca la StivaCorecta, insa de aceasta data extrag elementele din coada
//oferita drept parametru functiei.
void CoadaCorecta(TCoada *coada, FILE *out)
{
	TCoada *copy_coada=InitC(coada->dime);
	TCoada *work_coada=InitC(coada->dime);
	TStiva *aux_stiva=InitS(coada->dime);
	TStiva *index=InitS(sizeof(int));
	
	int i=0, first_index, current=0, max=0;
	first_index=-1;
	Push(index, &first_index);

	while(coada->vf!=NULL)
	{
		Intrq(work_coada, coada->vf->info);
		Intrq(copy_coada, coada->vf->info);
		Extrq(coada);
	}
	
	while(copy_coada->vf!=NULL)
	{
		Intrq(coada, copy_coada->vf->info);
		Extrq(copy_coada);
	}

	free(copy_coada);

	while(work_coada->vf!=NULL)
	{	
		if( CheckBracket(work_coada->vf->info, 1)==1)
		{
			Push(index, &i);
			Push(aux_stiva, work_coada->vf->info);
		}
		if(CheckBracket(work_coada->vf->info, 2)==1)
		{
			if(aux_stiva->vf==NULL)
				Push(index, &i);

			else if(MatchBracket(aux_stiva->vf->info, work_coada->vf->info)==1)
			{
				Pop(index);
				Pop(aux_stiva);
				current=i-(*(int*)index->vf->info);
				if(current>max)
					max=current;
			}
			else
			{	
				while(aux_stiva->vf!=NULL)
					Pop(aux_stiva);
				Push(index, &i);
			}
		}
		i++;
		Extrq(work_coada);
	}

	fprintf(out, "%d\n", max);
}

