#include "Archivo.h"
#include <string>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>	
#include <iostream>
using namespace std;
int ct=0;
class Archivo
{
	private:
		string nombreArchivo;
		int fd;
		char *contenido;
		size_t num_bytes;
	public:
		Archivo(string filename);
		Archivo(string filename,int banderas,mode_t modo);
		size_t lee(size_t nbytes);
		size_t escribe(void *buffer, size_t nbytes);
		size_t obtieneNum_bytes();		
		const char *get_contenido();
		~Archivo();
};

Archivo::Archivo(string filename)
{
	contenido=NULL;
	nombreArchivo=filename;
	fd=open((char *)(nombreArchivo.c_str()),O_RDONLY);
	if(fd==-1)
	{
		perror((char *)(nombreArchivo.c_str()));
		exit(-1);
	}
}
Archivo::Archivo(string filename,int banderas,mode_t modo)
{
	nombreArchivo=filename;
	fd=open((char*)(nombreArchivo.c_str()),modo,banderas);
	if(fd==-1)
	{
		perror((char *)(nombreArchivo.c_str()));
		exit(-1);

	}
}
size_t Archivo::lee(size_t nbytes)
{
	contenido=(char *)realloc(contenido,nbytes*sizeof(char));
	num_bytes=read(fd,contenido,nbytes);
	return num_bytes;
}
size_t Archivo::escribe(void *buffer, size_t nbytes)
{
	num_bytes=write(fd,buffer,nbytes);
	return num_bytes;
}
size_t Archivo::obtieneNum_bytes()
{
	return num_bytes;
}
const char *Archivo::get_contenido()
{
	return contenido;
}
Archivo::~Archivo()
{
	close(fd);
	delete contenido;
}
