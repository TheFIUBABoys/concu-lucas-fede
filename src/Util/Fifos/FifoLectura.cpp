#include "FifoLectura.h"

FifoLectura::FifoLectura(const std::string nombre) : Fifo(nombre) {
}

FifoLectura::~FifoLectura() {
}

void FifoLectura::abrir() {
    fd = open(nombre.c_str(), O_RDONLY);
    if (fd < 0){
        perror("Error abriendo fifo escritura");
    }
}

ssize_t FifoLectura::leer(void *buffer, const ssize_t buffsize) const {
    return read(fd, buffer, buffsize);
}
