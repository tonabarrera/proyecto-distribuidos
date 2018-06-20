#include "Archivo.h"

Archivo::Archivo(string filename) {
    num_bytes = 0;
    nombre_archivo = filename;
    fd = open(nombre_archivo.c_str(), O_RDONLY);

    if (fd < 1)
        printf("-- %s %s--\n", "ERROR AL ABRIR", nombre_archivo.c_str());
}

Archivo::Archivo(string filename, int banderas, mode_t modo) {
    num_bytes = 0;
    nombre_archivo = filename;
    fd = open(nombre_archivo.c_str(), banderas, modo);

    if (fd < 1)
        printf("-- %s %s--\n", "ERROR AL ABRIR", nombre_archivo.c_str());
}

size_t Archivo::lee(size_t nbytes) {
    //intf("num_bytes: %lu\n", num_bytes);
    contenido = (char*) realloc(contenido, nbytes);
    size_t n = read(fd, contenido, nbytes);
    num_bytes += n;
    return n;
}

size_t Archivo::escribe(void *buffer, size_t nbytes) {
    size_t bytes = write(fd, buffer, nbytes);
    return bytes;
}

size_t Archivo::get_num_bytes() {
    return num_bytes;
}

const char* Archivo::get_contenido() {
    return contenido;
}

void Archivo::cerrar() {
    close(fd);
}

Archivo::~Archivo() {
    close(fd);
    //free(contenido);
    contenido = NULL;
}


