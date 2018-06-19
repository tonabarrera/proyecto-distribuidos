#include "SocketDatagrama.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Archivo.h"

struct Mensaje {
    char numTel[11];
    char rfc[14];
    short int partido;
};
using namespace std;

int main(int argc, char *argv[])
{
    if(argc!=2) {
        printf("Modo de uso: servidor puerto");
        exit(1);
    }
    int port=atoi(argv[1]);
    struct Mensaje *votante;
    SocketDatagrama socket(port);
    int recibido=0, res;
    Archivo archivo("database.txt", O_WRONLY|O_TRUNC|O_CREAT, 0666);
    while (true) {
        printf("%s\n", "Esperando...");
        PaqueteDatagrama cliente(sizeof(struct Mensaje));
        recibido=socket.recibe(cliente);
        if(recibido<0)
          continue;
        votante = (struct Mensaje *)cliente.obtieneDatos();
        printf("Numero Telefonico: %s\n", votante->numTel); 
        printf("RFC: %s\n", votante->rfc);
        printf("Partido: %d\n",votante->partido);
        res=8;
        PaqueteDatagrama servidor((char *) &res,sizeof(int),cliente.obtieneDireccion(),cliente.obtienePuerto());
        socket.envia(servidor);
        printf("Escribio %lu \n", archivo.escribe(votante, sizeof(struct Mensaje)));
    }
    return 0;
}
