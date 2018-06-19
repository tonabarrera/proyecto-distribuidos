#include "Archivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Voto {
	char numTel[11];
    char RFC[14];
    short int partido;
};
char partidos[10][10]={"PAN","PRI","PRD","VERDE","PT","MC","NA","MORENA","ES","BRONCO"};
int main()
{
	char numTel[10];
	int nbytes;
	int encontrado=0;
	Archivo lectura("database.txt");
	struct Voto *aux;
	struct Voto *otro = (struct Voto *) malloc(sizeof(struct Voto));
	printf("Ingrese el número a buscar: ");
	scanf("%s",numTel);
	while((nbytes = lectura.lee(sizeof(struct Voto))) > 0 && encontrado==0)
	{
		memcpy(otro,lectura.get_contenido(),sizeof(struct Voto));
		if(strcmp(numTel,otro->numTel)==0)
		{
			encontrado=1;
			aux=otro;
		}
	}
	if(encontrado==1)
	{
		printf("Registro encontrado\n");
		printf("Numero telefonico: %s\n",aux->numTel);
		printf("RFC: %s\n",aux->RFC);
		printf("Partido: %s\n",partidos[aux->partido]);
	} 
	else
	{
		printf("Registro no encontrado.\n");
	}
	lectura.cerrar();
}