#ifndef PAQUETEDATAGRAGAMA_H
#define PAQUETEDATAGRAGAMA_H

//includes.

class PaqueteDatagrama {
    public:
        PaqueteDatagrama(char *datos, unsigned int longitud, char *ip, int puerto );
        PaqueteDatagrama(unsigned int longitud);
        ~PaqueteDatagrama();
        char *obtieneDireccion();          
        unsigned int obtieneLongitud();
        int obtienePuerto();
        char *obtieneDatos();
        void inicializaPuerto(int puerto);
        void inicializaIp(char *ip);
        void inicializaDatos(char *datos);
        void inicializaLongitud(unsigned int longitud);
    private:
        char *datos;        //Almacena los datos
        char ip[16];        //Almacena la IP 
        unsigned int longitud; //Longitud de la cadena de datos
        int puerto;          //Almacena el puerto
};  


#endif
