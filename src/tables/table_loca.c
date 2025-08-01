#include "tables/table_loca.h"
#include "misc/data_read.h"

#include <unistd.h>
#include <stdio.h>
#include "tables/table_loca.h"
/// table loca

void readLocaTable(int fd, t_loca* loca, t_table_directories* table_directories, t_head* head, t_maxp* maxp)
{
    lseek(fd, table_directories->directories[TAG_LOCA].offset, SEEK_SET);
    loca->longOffset = head->indexToLocFormat;
    loca->size = maxp->numGlyphs;
    if (loca->longOffset)
    {
        loca->content = readArray32(fd, loca->size);
    }
    else
    {
        loca->content = readArray16(fd, loca->size);
    }
}

void printLocaTable(t_loca* loca)
{
    printf("====> loca <====\n");

    if (loca->longOffset) {
        uint32_t* array = (uint32_t*)loca->content;
        for (size_t i = 0; i < loca->size; i++) {
            printf("=> Index %zu: offset = %u\n", i, array[i]);
        }
    }
    else {
        uint16_t* array = (uint16_t*)loca->content;
        for (size_t i = 0; i < loca->size; i++) {
            printf("=> Index %zu: offset = %u\n", i, array[i]);
        }
    }

}

void closeLocaTable(t_loca* loca)
{
    free(loca->content);
}

size_t getGlyphOffset(size_t glyphIndex, t_loca* loca)
{
    if (loca->longOffset) {
        return ((uint32_t*)(loca->content))[glyphIndex];
    }
    return ((uint16_t*)(loca->content))[glyphIndex] * 2;

}
