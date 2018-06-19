#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <stdlib.h>
#include <string.h>
#include <mutex>
#include "PaqueteDatagrama.h"
#include "SocketDatagrama.h"
using namespace std;
#define IP1 "10.100.77.38"
#define IP2	"10.100.79.185"
#define IP3	"10.100.67.49"
typedef struct Mensaje{
	char numTel[11];
	char rfc[14];
	short int partido;
}Mensaje;

void generaNumTel(char *numTel);
void generaRFC(char *rfc);
short int generaPartido(void);
void llenaMensaje(Mensaje *mensaje);
void enviaMensaje(char *IP,int puerto, Mensaje *mensaje);
mutex m;
int main(int argc, char const *argv[])
{
	FILE *numeros;
	int i,j;
	char ip1[] = IP1;
	char ip2[] = IP2;
	char ip3[] = IP3;

	//Crea un nuevo archivo para guardas los números.
	numeros = fopen("numeros.txt","w");
	fclose(numeros);
	//--------------
	// for(j = 0; j < 200; j++){
	// 	Mensaje mensaje;
	// 	llenaMensaje(&mensaje);
	// 	for (i = 0; i < 10; ++i)
	// 		printf("%c",mensaje.numTel[i] );
	// 	printf("\n");
	// 	for (i = 0; i < 13; ++i)
	// 		printf("%c",mensaje.rfc[i] );
	// 	printf("\n%d\n", mensaje.partido);
	// }
	Mensaje mensaje;
	for(i = 0;true;i++){
		llenaMensaje(&mensaje);
		thread th1(enviaMensaje,ip1,7000,&mensaje),th2(enviaMensaje,ip2,7000,&mensaje),th3(enviaMensaje,ip3,7000,&mensaje);
		th1.join();
		th2.join();
		th3.join();
	}
	return 0;
};
void generaNumTel(char *numTel){
	int i;
	for(i = 0;i < 10;i++)
		numTel[i] = (rand() % 10) + 48;
	numTel[i] = '\0';
	return;
};
void generaRFC(char *rfc){
	int i;
	//Llena los primeros cuatro slots con letras mayúsculas.
	for(i = 0;i < 4;i++)
		rfc[i] = (rand() % 25) + 65;
	//Llena los slot (4 al 9) con números aleatorios.
	for(;i < 10;i++)
		rfc[i] = (rand() % 10) + 48;
	//Llena los últimos tres slots.
	rfc[i++] = '1';
	rfc[i++] = '2';
	rfc[i++] = '3';
	rfc[i] = '\0';

	return;
};
short int generaPartido(void){
	int aux;
	aux = rand() % 100;
	//Votos AMLO.
	if(aux < 35){
		aux = rand() % 3;
		switch(aux){
			case 0:
				return 4;
				break;
			case 1:
				return 7;
				break;
			case 2:
				return 8;
				break;
		}
	}//Votos Anaya.
	else if(aux >= 35 && aux < 70){
		aux = rand() % 3;
		switch(aux){
			case 0:
				return 0;
				break;
			case 1:
				return 2;
				break;
			case 2:
				return 5;
				break;
		}
	}//Votos Meade.
	else if(aux >= 70 && aux < 90){
		aux = rand() % 3;
		switch(aux){
			case 0:
				return 1;
				break;
			case 1:
				return 3;
				break;
			case 2:
				return 6;
				break;
		}
	}//Votos Bronco.
	else if(aux >= 90){
		return 9;
	}
};
void llenaMensaje(Mensaje *mensaje){
	FILE *numeros;
	int i;
	char c;
	char aux[10],aux2[10];
	mensaje->partido = generaPartido();
	generaRFC(mensaje->rfc);
	numeros = fopen("numeros.txt","r+");
	num: generaNumTel(aux);
	do{
		i = 0;
		c = fgetc(numeros);
		if(c != EOF){
			aux2[i] = c;
			for (i = 1; i < 10; ++i)
				aux2[i] = fgetc(numeros);
			if(strncmp(aux,aux2,10) == 0)
				goto num;
			c = fgetc(numeros);
		}
	}while(c != EOF);
	strncpy(mensaje->numTel,aux,10);
	for (i = 0; i < 10; ++i)
		fprintf(numeros, "%c", aux[i]);
	fprintf(numeros, " ");
	fclose(numeros);

	return;
};
void enviaMensaje(char *IP,int puerto, Mensaje *mensaje){
	m.lock();
	int r,i=0;
	PaqueteDatagrama paquete = PaqueteDatagrama((char *)mensaje,sizeof(Mensaje),IP,puerto);
	SocketDatagrama socket = SocketDatagrama(0);
	socket.envia(paquete);
	do{
		r = socket.recibeTimeout(paquete,2,500);
		printf("%s\n",IP );
	}while(r < 1 && i++ < 6);
	m.unlock();
}