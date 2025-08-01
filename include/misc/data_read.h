#ifndef DATA_READ_H
#define DATA_READ_H

#include <unistd.h>
#include <stdint.h>

void *readArray(int fd, size_t size);

uint16_t *readArray16(int fd, size_t size);

uint32_t *readArray32(int fd, size_t size);



#endif