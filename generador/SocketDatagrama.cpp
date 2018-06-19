#include "SocketDatagrama.h" 

#include <sys/types.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <strings.h>

#include <string.h>

#include <errno.h>
#include <stdio.h>
#include <iostream>
using namespace std;

SocketDatagrama::SocketDatagrama(int puertoLocal){
	s = socket(AF_INET, SOCK_DGRAM, 0);
	bzero((char*)&direccionLocal, sizeof(direccionLocal));
	direccionLocal.sin_family = AF_INET;
	direccionLocal.sin_addr.s_addr = INADDR_ANY;
	direccionLocal.sin_port = htons(puertoLocal);
	bind(s, (struct sockaddr*)&direccionLocal, sizeof(direccionLocal));
}


SocketDatagrama::~SocketDatagrama(){
	close(s);
}
 
int SocketDatagrama::recibe(PaqueteDatagrama &p){
	unsigned int clilen = sizeof(direccionForanea);
	bzero((char*)&direccionForanea, sizeof(direccionForanea));
	int respuesta = recvfrom(s, p.obtieneDatos(), p.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, &clilen);
	p.inicializaPuerto(ntohs(direccionForanea.sin_port));
	p.inicializaIp(inet_ntoa(direccionForanea.sin_addr));

	return respuesta;
}


int SocketDatagrama::recibeTimeout(PaqueteDatagrama &p, time_t segundos, suseconds_t microsegundos){
	int tam = sizeof(direccionLocal);
	unsigned int datos[p.obtieneLongitud()];
	int puerto;
	int n;
	//struct timeval timeout;

	timeout.tv_sec = segundos;
	timeout.tv_usec = microsegundos;

	setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout));

	n = recvfrom(s,(char *)&datos,p.obtieneLongitud(),0,(struct sockaddr *)&direccionLocal,(socklen_t *)&tam);
	if(n<0){
		if(errno == EWOULDBLOCK){
			
			fprintf(stderr, "Tiempo para recepcion agotado\n");
			return -1;
		}
		else{
			fprintf(stderr, "Error en recvfrom\n");
			return 0;
		}
	}
			p.inicializaDatos((char *)&datos);
			p.inicializaIp(inet_ntoa(direccionLocal.sin_addr));
			puerto = direccionLocal.sin_port;
			puerto = ntohs(puerto);
			p.inicializaPuerto(puerto);
	return n;
}

int SocketDatagrama::envia(PaqueteDatagrama &p){
	
	bzero((char *)&direccionForanea, sizeof(direccionForanea));
  	direccionForanea.sin_family = AF_INET;
  	direccionForanea.sin_addr.s_addr = inet_addr(p.obtieneDireccion());
  	direccionForanea.sin_port = htons(p.obtienePuerto());


	/*
	int ans;
	memcpy((void*)&ans, (const void *)p.obtieneDatos() , sizeof(int) );
	cout<<ans<<endl;
	*/
	

  	return sendto(s,p.obtieneDatos(), p.obtieneLongitud(), 0, (struct sockaddr *) &direccionForanea, sizeof(direccionForanea));
}



