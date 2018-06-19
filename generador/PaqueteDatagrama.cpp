//#include <iostream>
#include "PaqueteDatagrama.h"
#include <string.h>

//para crear un paquete de envío tipo datagrama.
PaqueteDatagrama::PaqueteDatagrama(char *datos, unsigned int longitud, char *ip, int puerto ){
	//Primero se reserva memoria para datos.
	this->datos  = new char[longitud]; 
//	this->datos = datos; //Se asigna el valor recibido.
	memcpy(this->datos,datos,longitud);
	this->longitud = longitud;
	//this->ip=nullptr;
	//this->ip  = new char[16]; 
	for(int i=0;i<16;i++){
		this->ip[i]=ip[i];
	}
	//this->ip = ip;
	this->puerto = puerto;
}

PaqueteDatagrama::PaqueteDatagrama(unsigned int longitud){
	//Reserva #longitud bytes para el mensaje.
	this->datos= NULL;
	this->datos  = new char[longitud]; 
	this->longitud = longitud;
	//this->ip  = new char[16]; 
}


PaqueteDatagrama::~PaqueteDatagrama(){  
//	delete [] this->datos;
//	delete [] this->ip;
    
}

char* PaqueteDatagrama::obtieneDireccion(){
	return ip;
}          

unsigned int PaqueteDatagrama::obtieneLongitud(){
	return longitud;
}          
int PaqueteDatagrama::obtienePuerto(){
	return puerto;
}          
char* PaqueteDatagrama::obtieneDatos(){
	return datos;
}          
void PaqueteDatagrama::inicializaPuerto(int puerto){
	this->puerto=puerto;
}          
void PaqueteDatagrama::inicializaIp(char *ip){
	
	for(int i=0;i<16;i++){
		this->ip[i]=ip[i];
	}
}          
void PaqueteDatagrama::inicializaDatos(char *datos){
	memcpy(this->datos,datos,this->longitud);
}          
