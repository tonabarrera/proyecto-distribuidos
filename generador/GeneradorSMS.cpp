#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <stdlib.h>
#include <string.h>
#include <mutex>
#include <atomic>
#include "PaqueteDatagrama.h"
#include "SocketDatagrama.h"
#include "Semaforo.h"
using namespace std;
#define PAN 0
#define PRI 1
#define PRD 2
#define VERDE 3
#define PT 4
#define MC 5
#define NA 6
#define MORENA 7
#define ES 8
#define BRONCO 9

#define AMLO 0
#define ANAYA 1
#define MEADE 2
#define BRONCOC 3

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
void enviaMensaje(char *IP,int puerto, Mensaje *mensaje,int *n);

int partidos[10];
int candidatos[4];
int banderas[3];
atomic<int> global1(0);
atomic<int> global2(0);
atomic<int> global3(0);
mutex m;
int main(int argc, char const *argv[])
{
	FILE *numeros;
	int i,j;
	char ip1[] = IP1;
	char ip2[] = IP2;
	char ip3[] = IP3;
	int n1=0,n2=0,n3=0;
	//Limpia conteo de partidos.
	for (i = 0; i < 10; ++i)
		partidos[i] = 0;
	//Limpia conteo de candidatos.
	for (i = 0; i < 4; ++i)
		candidatos[i] = 0;
	//Limpiabanderas
	for (i = 0; i < 3; ++i)
		banderas[i] = 0;

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
	thread hilos[3];
	Mensaje mensaje;
	for (i = 0; true; ++i){
		llenaMensaje(&mensaje);
		thread th1(enviaMensaje,ip1,7000,&mensaje,&n1),th2(enviaMensaje,ip2,7000,&mensaje,&n2),th3(enviaMensaje,ip3,7000,&mensaje,&n3);
		// if(banderas[0]==0){
		// 	printf("bandera1\n");
		// 	hilos[0] = thread(enviaMensaje,ip1,7000,&mensaje);
		// }
		// if(banderas[1]==0){
		// 	printf("bandera2\n");
		// 	hilos[0] = thread(enviaMensaje,ip2,7000,&mensaje);
		// }
		// if(banderas[2]==0){
		// 	printf("bandera3\n");
		// 	hilos[0] = thread(enviaMensaje,ip3,7000,&mensaje);
		// }
		if(global1==0){
			printf("bandera1\n");
			th1.join();
			//hilos[0].join();
		}
		if(global2==0){
			printf("bandera2\n");
			th2.join();
			//hilos[2].join();
		}
		if(global3==0){
			printf("bandera3\n");
			th3.join();
			// hilos[3].join();
		}
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
		candidatos[AMLO]++;
		aux = rand() % 3;
		switch(aux){
			case 0:
			partidos[PT]++;
				return PT;
				break;
			case 1:
				partidos[MORENA]++;
				return MORENA;
				break;
			case 2:
				partidos[ES]++;
				return ES;
				break;
		}
	}//Votos Anaya.
	else if(aux >= 35 && aux < 70){
		candidatos[ANAYA]++;
		aux = rand() % 3;
		switch(aux){
			case 0:
				partidos[PAN]++;
				return PAN;
				break;
			case 1:
				partidos[PRD]++;
				return PRD;
				break;
			case 2:
				partidos[MC]++;
				return MC;
				break;
		}
	}//Votos Meade.
	else if(aux >= 70 && aux < 90){
		candidatos[MEADE]++;
		aux = rand() % 3;
		switch(aux){
			case 0:
				partidos[PRI]++;
				return PRI;
				break;
			case 1:
				partidos[VERDE]++;
				return VERDE;
				break;
			case 2:
				partidos[NA]++;
				return NA;
				break;
		}
	}//Votos Bronco.
	else if(aux >= 90){
		candidatos[BRONCOC]++;
		partidos[BRONCO]++;
		return BRONCO;
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
void enviaMensaje(char *IP,int puerto, Mensaje *mensaje,int *n){
	if(*n == 0){
		int r,i=0;
		PaqueteDatagrama paquete = PaqueteDatagrama((char *)mensaje,sizeof(Mensaje),IP,puerto);
		SocketDatagrama socket = SocketDatagrama(0);
		//printf("%d\n",mensaje->partido );
		socket.envia(paquete);
		do{
			r = socket.recibeTimeout(paquete,2,500);
			printf("%s\n",IP );
		}while(r < 1 && i++ < 6);
		//m.lock();
		if(i == 7){
			// printf("entro\n");
			// if (strcmp(IP,IP1) == 0){
			// 	//banderas[0]++;
			// 	global1++;
			// 	printf("1\n");
			// }
			// if (strcmp(IP,IP2) == 0){
			// 	printf("2\n");
			// 	global2++;
			// 	//banderas[1]++;
			// }
			// if (strcmp(IP,IP3) == 0){
			// 	printf("3\n");
			// 	global3++;
			// 	//banderas[2]++;
			// }
			*n++;
		}
		//m.unlock();
		//Imprime votospor partido/
		printf("--------------------------\n");
		printf("PAN: %d\n", partidos[PAN]);
		printf("PRI: %d\n", partidos[PRI]);
		printf("PRD: %d\n", partidos[PRD]);
		printf("VERDE: %d\n", partidos[VERDE]);
		printf("PT: %d\n", partidos[PT]);
		printf("MC: %d\n", partidos[MC]);
		printf("NA: %d\n", partidos[NA]);
		printf("MORENA: %d\n", partidos[MORENA]);
		printf("ES: %d\n", partidos[ES]);
		printf("BRONCO: %d\n", partidos[BRONCO]);
	}
}