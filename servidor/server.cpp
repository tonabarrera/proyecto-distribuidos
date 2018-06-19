#include "Archivo.h"
#include <cstring>


struct Voto {
    char RFC[14];
    char numTel[11];
    short int partido;
};

void writeDB(char *message);
char *readDB(int sizeMessage);

int main(int argc, char const *argv[]){
    struct Voto *voto = (struct Voto *) malloc(sizeof(struct Voto));
    struct Voto *voto2 = (struct Voto *) malloc(sizeof(struct Voto));

    memset(voto->numTel, 0, 11);
    memset(voto->RFC, 0, 14);
    memcpy(voto->numTel, "1234567890", 10);
    memcpy(voto->RFC, "ACBV103401123", 13);

    memset(voto2->numTel, 0, 11);
    memset(voto2->RFC, 0, 14);
    memcpy(voto2->numTel, "3214567890", 10);
    memcpy(voto2->RFC, "ABCV103401123", 13);
    voto2->partido = 2;
    voto->partido = 1;
    printf("%s\n",voto->RFC);
    printf("%s\n",voto->numTel);
    printf("%d\n",voto->partido);
    Archivo archivo("database.txt", O_WRONLY|O_TRUNC|O_CREAT, 0666);
    printf("Escribio %lu \n", archivo.escribe(voto, sizeof(struct Voto)));

    printf("Escribio voto 2 %lu \n", archivo.escribe(voto2, sizeof(struct Voto)));

    struct Voto *otro = (struct Voto *) malloc(sizeof(struct Voto));
    Archivo lectura("database.txt");

    printf("Se leyo: %lu\n", lectura.lee(sizeof(struct Voto)));
    printf("Se leyo: %lu\n", lectura.lee(sizeof(struct Voto)));
    memcpy(otro, lectura.get_contenido()+sizeof(struct Voto), sizeof(struct Voto));
    printf("%s\n",otro->RFC);
    printf("%s\n",otro->numTel);
    printf("%d\n",otro->partido);
    lectura.cerrar();
    
    //writeDB((char *)&voto);
    //memcpy((char *)&otroVoto,readDB(sizeof(voto)),sizeof(voto));
    //cout<<otroVoto.RFC<<"\n";
    //cout<<otroVoto.partido<<"\n";
    //cout<<otroVoto.numTel<<"\n";
    return 0;
}
/*
char *readDB(int sizeMessage){
    ifstream readFile("database.txt");
    char *output = new char[sizeMessage];
    readFile.read(output,(sizeMessage));
    readFile.close();
    return output;
}

void writeDB(char *message){
    ofstream myfile;
    myfile.open("database.txt",ios::app);
    myfile << message;
    myfile.close();
}*/