#ifndef TABLE_NAME
#define TABLE_NAME

#include "misc/ttf_types.h"
#include "tables/table_directories.h"
//// TABLE "name"
typedef struct __attribute__((packed))
{
    uint16_t platformId;
    uint16_t platformSpecificId;
    uint16_t languageId;
    uint16_t nameId;
    uint16_t length;
    uint16_t offset;
} t_name_records;

typedef struct __attribute__((packed))
{
    uint16_t format;
    uint16_t count;
    uint16_t stringOffset;
    t_name_records *nameRecord;
    char *name;
} t_name;


void readNameTable(int fd, t_name *name, t_table_directories *dir);

void printNameTable(t_name *name);

void closeNameTable(t_name *name);

#endif