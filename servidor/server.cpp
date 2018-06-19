#include <iostream>
#include <string.h>
#include <string>
#include <fstream>

using namespace std;

struct Voto
{
	char RFC[13];
	char numTel[10];
	short int partido;
}voto,otroVoto;

void writeDB(char *message);
char *readDB(int sizeMessage);

int main(int argc, char const *argv[])
{
	char *algo = new char(sizeof(struct Voto));
	strcpy(voto.numTel,(char *)"1234567890");
	strcpy(voto.RFC,(char *)"ACBV103401123");
	voto.partido = 1;
	printf("%s\n",voto.RFC);
	printf("%s\n",voto.numTel);
	printf("%d\n",voto.partido);
	writeDB((char *)&voto);
	memcpy((char *)&otroVoto,readDB(sizeof(voto)),sizeof(voto));
	//cout<<otroVoto.RFC<<"\n";
	//cout<<otroVoto.partido<<"\n";
	//cout<<otroVoto.numTel<<"\n";
	return 0;
}

char *readDB(int sizeMessage){
	ifstream readFile("database.txt");
	char *output = new char[sizeMessage];
	int buffer = 10;
	int i=0;
	readFile.read(output,(sizeMessage));
	readFile.close();
	return output;
}

void writeDB(char *message){
	ofstream myfile;
	myfile.open("database.txt",ios::app);
	myfile << message;
	myfile.close();
}