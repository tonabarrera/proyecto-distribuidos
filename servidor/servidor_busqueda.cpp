#include "Archivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SocketDatagrama.h"
struct Voto {
	char numTel[11];
    char RFC[14];
    short int partido;
};
char partidos[10][10]={"PAN","PRI","PRD","VERDE","PT","MC","NA","MORENA","ES","BRONCO"};
int main(int argc, char *argv[])
{
	if(argc!=2) {
        printf("Modo de uso: servidor puerto");
        exit(1);
    }
	int port=atoi(argv[1]);
	char numTel[11];
	int nbytes;
	int encontrado=0;
	SocketDatagrama socket(port);
    int recibido=0;
	struct Voto *res;
	while (true) {
        printf("%s\n", "Esperando...");
        PaqueteDatagrama cliente(11);
        recibido=socket.recibe(cliente);
		printf("Recibido: %d\n",recibido);
        if(recibido<0)
          continue;
		memcpy(numTel,cliente.obtieneDatos(),11);
		printf("N: %s\n",numTel);
		Archivo lectura("database.txt");
		struct Voto *otro = (struct Voto *) malloc(sizeof(struct Voto));
		while((nbytes = lectura.lee(sizeof(struct Voto))) > 0 && encontrado==0)
		{
			memcpy(otro,lectura.get_contenido(),sizeof(struct Voto));
			printf("NM: %s\n",otro->numTel);
			if(strcmp(numTel,otro->numTel)==0)
			{
				encontrado=1;
				res=otro;
			}
		}
		printf("OK\n");
		if(encontrado==0)
		{
			memcpy(res->numTel,numTel,11);
			memcpy(res->RFC,"",14);
			res->partido=-1;
		}
		printf("OK2\n");
		encontrado=0;
		PaqueteDatagrama servidor((char *)res,sizeof(struct Voto),cliente.obtieneDireccion(),cliente.obtienePuerto());
        socket.envia(servidor);
		free(res);
		lectura.cerrar();
	}
}