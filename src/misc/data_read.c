#include "misc/data_read.h"
#include <stdlib.h>
#include <endian.h>
void *readArray(int fd, size_t size)
{
    void *array = malloc(size);
    read(fd, array, size);
    return array;
}

uint16_t *readArray16(int fd, size_t size)
{
    uint16_t *array = (uint16_t *)readArray(fd, sizeof(uint16_t) * size);
    for (size_t j = 0; j < size; j++)
    {
        array[j] = be16toh(array[j]);
    }
    return array;
}
uint32_t *readArray32(int fd, size_t size)
{
    uint32_t *array = (uint32_t *)readArray(fd, sizeof(uint32_t) * size);
    for (size_t j = 0; j < size; j++)
    {
        array[j] = be32toh(array[j]);
    }
    return array;
}
