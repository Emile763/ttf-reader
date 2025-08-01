#include "tables/table_name.h"
#include "tables/table_directories.h"

#include <endian.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//// TABLE "name"

void readNameTable(int fd, t_name *name, t_table_directories *dir)
{
    // Move file cursor to the beginning of the head table
    lseek(fd, dir->directories[TAG_NAME].offset, SEEK_SET);

    // read: format, count & stringOffset
    read(fd, name, 3 * sizeof(uint16_t));
    name->format = be16toh(name->format);
    name->count = be16toh(name->count);
    name->stringOffset = be16toh(name->stringOffset);

    // Allocate memory for all the name records
    size_t nameRecord_size = name->count * sizeof(t_name_records);
    name->nameRecord = (t_name_records *)malloc(nameRecord_size);
    read(fd, name->nameRecord, nameRecord_size);

    for (size_t i = 0; i < name->count; i++)
    {
        name->nameRecord[i].platformId = be16toh(name->nameRecord[i].platformId);
        name->nameRecord[i].platformSpecificId = be16toh(name->nameRecord[i].platformSpecificId);
        name->nameRecord[i].languageId = be16toh(name->nameRecord[i].languageId);
        name->nameRecord[i].nameId = be16toh(name->nameRecord[i].nameId);
        name->nameRecord[i].length = be16toh(name->nameRecord[i].length);
        name->nameRecord[i].offset = be16toh(name->nameRecord[i].offset);
    }

    size_t name_size = dir->directories[TAG_NAME].length - nameRecord_size - 3 * sizeof(uint16_t);
    name->name = (char *)malloc(name_size);
    read(fd, name->name, name_size);
}

void printNameTable(t_name *name)
{
    printf("=====> name <=====\n");
    printf("===> format        : %u\n", name->format);
    printf("===> count         : %u\n", name->count);
    printf("===> string offset : %u\n", name->stringOffset);

    for (size_t i = 0; i < name->count; i++)
    {
        printf("    =====> %zu <=====\n", i);
        printf("    ===> platformId         :%u\n", name->nameRecord[i].platformId);
        printf("    ===> platformSpecificId :%u\n", name->nameRecord[i].platformSpecificId);
        printf("    ===> language           :%u\n", name->nameRecord[i].languageId);
        printf("    ===> nameId             :%u\n", name->nameRecord[i].nameId);
        printf("    ===> length             :%u\n", name->nameRecord[i].length);
        printf("    ===> offset             :%u\n", name->nameRecord[i].offset);
        printf("Content :\n ");
        write(STDOUT_FILENO, name->name + name->nameRecord[i].offset, name->nameRecord[i].length);
        printf("\n");
    }
}

void closeNameTable(t_name *name)
{
    free(name->nameRecord);
    free(name->name);
}
