#include "tables/table_directories.h"

#include <endian.h>
#include <stdio.h>
#include <unistd.h>

//// table directory

void readTableDirectory(int fd, t_table_directory *table_directory)
{
    read(fd, table_directory, sizeof(t_table_directory));
    table_directory->checksum = be32toh(table_directory->checksum);
    table_directory->offset = be32toh(table_directory->offset);
    table_directory->length = be32toh(table_directory->length);
}

void initTableDirectories(t_table_directories *dir)
{
    for (size_t i = 0; i < NUMBER_OF_TAG; i++)
    {
        dir->is_present[i] = 0;
    }
}

void printTableDirectory(e_tag tag, t_table_directory *table_directory)
{
    printf("   =====> %.*s <=====\n", 4, tagEToS[tag]);
    printf("   ==> checksum : %u\n", table_directory->checksum);
    printf("   ==> offset   : %u\n", table_directory->offset);
    printf("   ==> length   : %u\n", table_directory->length);
}
