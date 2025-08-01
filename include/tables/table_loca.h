#ifndef TABLE_LOCA_H
#define TABLE_LOCA_H

#include "tables/table_directories.h"
#include "tables/table_head.h"
#include "tables/table_maxp.h"

#include <stdlib.h>

/// table loca

typedef struct
{
    // false => offsets in word && offset type = uint16
    // true => offsets in byte && offset type = uint32
    int longOffset;
    size_t size;
    void* content;

} t_loca;

void readLocaTable(int fd, t_loca* loca, t_table_directories* table_directories, t_head* head, t_maxp* maxp);

void printLocaTable(t_loca* loca);

void closeLocaTable(t_loca* loca);

size_t getGlyphOffset(size_t glyphIndex, t_loca* loca);

#endif