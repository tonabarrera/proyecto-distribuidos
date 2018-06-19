#include "SocketDatagrama.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
int main(int argc, char *argv[])
{
	if(argc!=2)
	{
		printf("Modo de uso: servidor puerto");
		exit(1);
	}
	int port=atoi(argv[1]);
	typedef struct 
	{
		char numTel[10];
		char rfc[13];
		short int partido;
	}mensaje;
	mensaje *votante;
    SocketDatagrama socket(port);
    int recibido=0,res;
    while (1) 
	{
        PaqueteDatagrama Cliente(sizeof(mensaje));
        recibido=socket.recibe(Cliente);
        if(recibido<0)
          continue;
        votante = (mensaje *)Cliente.obtieneDatos();
	printf("Numero Telefonico: "); 
	for(int i=0;i<10;i++)
	{
		printf("%c",votante->numTel[i]);
	}
	printf("\nRFC: ");
	for(int j=0;j<13;j++)
	{
		printf("%c",votante->rfc[j]);
	}
	printf("\n");
	printf("Partido: %d\n",votante->partido);
	res=8;
        PaqueteDatagrama Servidor((char *) &res,sizeof(int),Cliente.obtieneDireccion(),Cliente.obtienePuerto());
        socket.envia(Servidor);
    }
    return 0;
}
