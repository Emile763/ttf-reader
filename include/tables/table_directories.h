#ifndef TABLE_DIRECTORIES_H
#define TABLE_DIRECTORIES_H
#include "misc/ttf_types.h"


//// table directory
typedef struct __attribute__((packed))
{
    uint32_t checksum;
    uint32_t offset;
    uint32_t length;
} t_table_directory;

void readTableDirectory(int fd, t_table_directory *table_directory);



typedef struct
{
    t_table_directory directories[NUMBER_OF_TAG];
    int is_present[NUMBER_OF_TAG];
} t_table_directories;

void initTableDirectories(t_table_directories *dir);

void printTableDirectory(e_tag tag, t_table_directory *table_directory);

#endif