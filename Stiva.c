#include "Stiva.h"
#include "Lista.h"

//Functia de initializare a unei stive din vectorul de stive
TStiva *InitS(size_t d)
{
	TStiva *stiva=malloc(sizeof(TStiva));
	if(!stiva)
	{
		free(stiva);
		fprintf(stderr, "%d\n", MemoryError);
		exit(EXIT_FAILURE);
	}
	stiva->dime=d;
	stiva->vf=NULL;

	return stiva;
}

//Functia introduce in varful stivei un nou element, de tipul TLG , nestiind tipul campului
//destinat informatiei. Astfel functia capata caracter generic.
void Push(TStiva *stiva, void *bracket)
{
	TLG aux;
	aux=AlocaCelula(bracket, stiva->dime);
	if((stiva)->vf==NULL)
		(stiva)->vf=aux;
	else							//Retin adresa primului element din stiva. Elementele sunt inserate mereu in varful stivei
	{
		aux->urm=(stiva)->vf;
		stiva->vf=aux;
	}
}

//Extrage primul element din varful stivei
void Pop(TStiva *stiva)
{
	if(stiva->vf==NULL)
		return;
	void *aux_info=malloc(stiva->dime);
	TLG aux=stiva->vf;
	stiva->vf=aux->urm;
	memcpy(aux_info, aux->info, stiva->dime);
	free(aux_info);
	free(aux);
}

//Functie de afisare a tuturor stivelor. Functia primeste drept argument vectorul de stive
//si parcurgand vectorul, sa va afisa fiecare stiva in parte. Afisarea se face de la baza stivei catre varf.
void PrintStiva(TStiva ***Stacks, int S, FILE *out)
{
	int i;
	//Initializez un vector de stive auxiliar, ce ma va ajuta in afisarea tuturor stivelor
	TStiva **aux_stiva=malloc(S*sizeof(TStiva*));
	if(!aux_stiva)
	{
		free(aux_stiva);
		fprintf(stderr, "%d\n", MemoryError );
		exit(EXIT_FAILURE);
	}

	for(i=0;i<S;i++)
		aux_stiva[i]=InitS((*Stacks)[i]->dime);

	//Parcurg vectorul de stive
	for(i=0;i<S;i++)
	{	
		//Pentru afisare, extrag fiecare element si il introduc in stiva auxiliara; voi ajunge astfel sa am "rasturnatul" stivei initiale
		//Cand stiva mea devine vida, rastorn elementele extrase(ce se afla in stiva auxiliara) inapoi in stiva initiala.
		fprintf(out, "\"");
		while((*Stacks)[i]->vf!=NULL)
		{
			Push(aux_stiva[i], (*Stacks)[i]->vf->info);
			Pop((*Stacks)[i]);
		}

		while(aux_stiva[i]->vf!=NULL)
		{
			AfiEl(aux_stiva[i]->vf->info, out);
			Push((*Stacks)[i], aux_stiva[i]->vf->info);
			Pop(aux_stiva[i]);
		}
		fprintf(out, "\"\n");
	}
}

//Functie de sortare a stivei. Compar fiecare element extras din varful stivei, cu restul elementelor
//ce vor fi extrase pe rand si introduse intr-o stiva auxiliara.
void SortStiva(TStiva **stiva)
{
	void *elem = malloc((*stiva)->dime);
	if(!elem)
	{
		free(elem);
		fprintf(stderr, "%d\n", MemoryError );
		exit(EXIT_FAILURE);
	}
	TStiva *aux_stiva=InitS((*stiva)->dime);

	while((*stiva)->vf!=NULL)
	{
		((Paranteza*)elem)->id_paranteza=((Paranteza*)(*stiva)->vf->info)->id_paranteza;
		((Paranteza*)elem)->tip=((Paranteza*)(*stiva)->vf->info)->tip;

		Pop(*stiva);
		while(aux_stiva->vf!=NULL && CompEl(aux_stiva->vf->info, elem)==-1)
		{
			Push(*stiva, aux_stiva->vf->info);
			Pop(aux_stiva);
		}
		Push(aux_stiva, elem);
	}

	while(aux_stiva->vf!=NULL)
	{
		Push(*stiva, aux_stiva->vf->info);
		Pop(aux_stiva);
	}

	free(elem);
	free(aux_stiva);
}

//Functia specifica instructiunii corrects
void StivaCorecta(TStiva *stiva, FILE *out)
{
	int i=0, max=0, current=0;
	TStiva *copy_stiva=InitS(stiva->dime);
	TStiva *rev_stiva=InitS(stiva->dime);
	TStiva *aux_stiva=InitS(stiva->dime);
	TStiva *index=InitS(sizeof(int));	//am initializat o stiva ce va avea elemente de tip int
	int first_index;					//pentru introducerea indecsilor parantezelor din stiva data drep argument. Primul element din aceasta stiva va fi -1.
	first_index=-1;
	Push(index, &first_index);
	
	//In rezolvarea cerintei, stiva de lucru(copy_stiva) va fi rasturnatul stivei initiale. Deci rastorn stiva initiala.
	while(stiva->vf!=NULL)
	{
		Push(rev_stiva, stiva->vf->info);
		Push(copy_stiva, stiva->vf->info);
		Pop(stiva);
	}

	while(rev_stiva->vf!=NULL)
	{	
		Push(stiva, rev_stiva->vf->info);
		Pop(rev_stiva);
	}

	//Extrag fiecare element din stiva, pana cand aceasta ramane fara elemente. Fiecare paranteza va fi prelucrata la nivelul fiecarei iteratii.
	while(copy_stiva->vf!=NULL)
	{	
		//Daca paranteza mea este deschisa(nu ma intereseaza tipul ei), introduc indexul in stiva "index" si paranteza insasi in stiva aux_stiva. 
		if( CheckBracket(copy_stiva->vf->info, 1)==1)
		{
			Push(index, &i);
			Push(aux_stiva, copy_stiva->vf->info);
		}

		//Cazul in care paranteza este inchisa:
		if(CheckBracket(copy_stiva->vf->info, 2)==1)
		{
			//Daca stiva in care introduc toate parantezele deschise este goala, expresia insasi nu va fi corecta deoarece nu s-a deschis nici o paranteza
			//si prin urmare nu e nevoie de nici o paranteza inchisa pentru a valida expresia. Din acest motiv lungimea expresiei mele trebuie resetata. Acest lucru
			//se va realiza prin introducerea in varful stivei a indexului parantezei inchise.
			if(aux_stiva->vf==NULL)
				Push(index, &i);

			//Daca paranteza inchisa din aceasta iteratie este de acelasi tip cu paranteza deschisa din varful aux_stiva, inseamna ca am 
			//o expresie corect parantezata pana in momentul de fata. Scot ultimul ultimul element din stiva "index"(acesta reprezentand index-ul parantezei deschise
			// ce tocmai a fost corect inchisa) si scad din i (indexul actual de iteratie), indexul ce se va afla in varful stivei index, afland astfel
			//lungimea sirului corect parantezat.
			else if(MatchBracket(aux_stiva->vf->info, copy_stiva->vf->info)==1)
			{
				Pop(index);
				Pop(aux_stiva);
				current=i-(*(int*)index->vf->info);
				if(current>max)
					max=current;
			}

			//Daca paranteza este inchisa, dar nu este de acelasi tip cu paranteza deschisa din varful aux_stiva, aux stiva trebuie golita. Altfel am putea
			//risca sa validam cazuri de genul "{[)}" , deoarece in stiva ne-ar putea ramane paranteze deschise ce ar putea fi corect inchise mai tarziu in expresie
			//dar nu ar reprezenta o expresie valida in intregime, deorece in interiorul acestora am avea paranteze incorecte. Mai mult, indexul parantezei de inchidere
			//trebuie pus in varful stivei index; lungimea expresiei se va reseta in acest moment, pornind de la indexul acestei paranteze incorecte.
			else
			{	
				while(aux_stiva->vf!=NULL)
					Pop(aux_stiva);
				Push(index, &i);
			}
		}
		i++;
		Pop(copy_stiva);
	}

	fprintf(out, "%d\n", max);

	free(rev_stiva); free(copy_stiva); free(aux_stiva); free(index);
}