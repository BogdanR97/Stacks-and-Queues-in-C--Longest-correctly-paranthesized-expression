/*ROMAN Bogdan-Gabriel - 312CB*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Stiva.h"
#include "Coada.h"

//Codul -1 va fi afisat la stderr in cazul alocarilor de memorie nereusite
#define MemoryError -1

//Functia folosita pentru comparea id-urilor parantezelor din stiva
int CompId(void *x, void *y)
{
	int z= ((Paranteza*)x)->id_paranteza - ((Paranteza*)y)->id_paranteza;
	if(z<0)
		return -1;
	else if(z==0)
		return 0;
	else
		return 1;
}

//Functia folosita pentru afisarea parantezelor
void PrintParanteza( void *data, FILE *file)
{
	fprintf(file, "%c", ((Paranteza*)data)->tip);
}

//Functie folosita in implementarea functiei corespunzatoare comenzilor corrects/correctq.
//Functia primeste ca parametrii 2 elemente: primul element va fi o paranteza deschia de orice tip,
//iar al doilea element va fi o paranteza inchisa de orice tip. Daca tipul elementele sunt de acelasi tip
//functia intoarce valoarea 1(adevarat), altfel intoarce valoarea 0(fals).
int MatchBracket(void *first, void *second)
{
	if(((Paranteza*)first)->tip == '(' && ((Paranteza*)second)->tip == ')')
		return 1;
	else if(((Paranteza*)first)->tip == '[' && ((Paranteza*)second)->tip == ']')
		return 1;
	else if(((Paranteza*)first)->tip == '{' && ((Paranteza*)second)->tip == '}')
		return 1;
	else
		return 0;
}

//Functie folosita in implementarea functiei corespunzatoare comenzilor corrects/correctq.
//Functia primeste ca prim parametru un tip de paranteza, iar al doilea parametru transmis functiei
//ii spune daca se cere verificarea tipului unei paranteze inchise/deschise;
int CheckBracket(void *bracket, int type)
{
	if(type==1 &&( ((Paranteza*)bracket)->tip=='(' || ((Paranteza*)bracket)->tip=='['
		|| ((Paranteza*)bracket)->tip=='{'))
		return 1;

	else if(type==2 &&( ((Paranteza*)bracket)->tip==')' || ((Paranteza*)bracket)->tip==']'
	 || ((Paranteza*)bracket)->tip=='}'))
		return 1;

	else return 0;
}

//Functia de citire a operatiilor din fisierul input
void ReadOperation(FILE * file, char *data)
{
	char ch;
	int len=0;

	ch=fgetc(file);
	while(ch!=' ' && ch!='\n') //nestiind lungimea string-ului pun conditii de oprire;
	{
		if(feof(file))break;
		data[len]=ch;
		len++;
		ch=fgetc(file);
	}
	data[len]='\0';
}

int main(int argc, char *argv[])
{
	int N, S, C, i, id_stiva, id_coada;
	char *op;
	FILE *fish, *out;
	fish=fopen(argv[1], "rt");
	out=fopen(argv[2], "wt");

	fscanf(fish, "%d", &N);
	fscanf(fish, "%d", &S);
	fscanf(fish, "%d", &C);
	fseek(fish,1,SEEK_CUR);

	AfiEl=PrintParanteza;
	CompEl=CompId;

	//Initializarea vectorului de stive
	TStiva **Stacks=malloc(S*sizeof(TStiva*));
	if(!Stacks)
		return 0;
	for(i=0; i<S; i++)
		Stacks[i]=InitS(sizeof(Paranteza));

	//Initializarea vectorului de cozi
	TCoada **Queues=(TCoada **)malloc(C*sizeof(TCoada*));
	if(!Queues)
		return 0;
	for(i=0; i<C; i++)
		Queues[i]=InitC(sizeof(Paranteza));

	//Parcurg fiecare linie din fisier si citesc instructiunea corespunzatoare
	for(i=0; i<N; i++)
	{
		op=malloc(20*sizeof(char));
		ReadOperation(fish, op);

		if(strcmp(op, "push")==0)
		{
			fscanf(fish, "%d", &id_stiva);
			Paranteza *bracket=malloc(sizeof(Paranteza));
			fscanf(fish, "%d %c", &bracket->id_paranteza, &bracket->tip);
			fseek(fish, 1, SEEK_CUR);
			Push(Stacks[id_stiva], bracket);	
		}

		if(strcmp(op, "intrq")==0)
		{
			fscanf(fish, "%d", &id_coada);
			Paranteza *bracket=malloc(sizeof(Paranteza));
			fscanf(fish, "%d %c", &bracket->id_paranteza, &bracket->tip);
			fseek(fish, 1, SEEK_CUR);
			Intrq(Queues[id_coada], bracket);
		}

		if(strcmp(op, "pop")==0)
		{
			fscanf(fish, "%d", &id_stiva);
			fseek(fish, 1, SEEK_CUR);
			Pop(Stacks[id_stiva]);
		}

		if(strcmp(op, "extrq")==0)
		{
			fscanf(fish, "%d", &id_coada);
			fseek(fish, 1, SEEK_CUR);
			Extrq(Queues[id_coada]);
		}

		if (strcmp(op, "printq")==0)
			PrintCoada(&Queues, C, out);

		if(strcmp(op, "prints")==0)
			PrintStiva(&Stacks, S, out);

		if(strcmp(op, "sorts")==0)
		{
			fscanf(fish, "%d", &id_stiva);
			fseek(fish, 1, SEEK_CUR);
			SortStiva(&Stacks[id_stiva]);
		}

		if(strcmp(op, "sortq")==0)
		{
			fscanf(fish, "%d", &id_coada);
			fseek(fish, 1, SEEK_CUR);
			SortCoada(&Queues[id_coada], out);
		}

		if(strcmp(op, "corrects")==0)
		{
			fscanf(fish, "%d", &id_stiva);
			fseek(fish, 1, SEEK_CUR);
			StivaCorecta(Stacks[id_stiva], out);
		}

		if(strcmp(op, "correctq")==0)
		{
			fscanf(fish, "%d", &id_coada);
			fseek(fish, 1 ,SEEK_CUR);
			CoadaCorecta(Queues[id_coada], out);
		}
	}

	for(i=0; i<S; i++)
	{
		while(Stacks[i]->vf!=NULL)
			Pop(Stacks[i]);
		free(Stacks[i]);
	}
	free(Stacks);

	for(i=0; i<C; i++)
	{
		while(Queues[i]->vf!=NULL)
			Extrq(Queues[i]);
		free(Queues[i]);
	}
	free(Queues);

	fclose(fish); fclose(out);

	return 0;

}