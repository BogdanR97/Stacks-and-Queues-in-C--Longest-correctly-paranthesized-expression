#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef Lista_Generica
#define Lista_Generica

#define MemoryError -1

//Structura pentru campul destinat informatiei
typedef struct
{
	int id_paranteza;
	char tip;
}Paranteza;

//Structura Listei Generice
typedef struct celulag
{ 
  void* info;           
  struct celulag *urm;   
} TCelulaG, *TLG; 

TLG AlocaCelula(void *el, size_t d);
#endif