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
	char *numTel;
	int nbytes;
	int encontrado=0;
	SocketDatagrama socket(port);
    int recibido=0;
	struct Voto *res;
	while (true) {
        printf("%s\n", "Esperando...");
        PaqueteDatagrama cliente(sizeof(struct Voto));
        recibido=socket.recibe(cliente);
        if(recibido<0)
          continue;
		numTel = (char *)cliente.obtieneDatos();
		Archivo lectura("database.txt");
		struct Voto *otro = (struct Voto *) malloc(sizeof(struct Voto));
		printf("NB: %d\n",nbytes);
		while((nbytes = lectura.lee(sizeof(struct Voto))) > 0 && encontrado==0)
		{
			memcpy(otro,lectura.get_contenido(),sizeof(struct Voto));
			if(strcmp(numTel,otro->numTel)==0)
			{
				encontrado=1;
				res=otro;
			}
		}
		if(encontrado==0)
		{
			res->partido=-1;
		}
		encontrado=0;
		PaqueteDatagrama servidor((char *)res,sizeof(struct Voto),cliente.obtieneDireccion(),cliente.obtienePuerto());
        socket.envia(servidor);
		lectura.cerrar();
	}
}