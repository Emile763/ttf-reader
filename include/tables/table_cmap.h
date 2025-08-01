#ifndef TABLE_CMAP_H
#define TABLE_CMAP_H

#include <stddef.h>
#include "misc/ttf_types.h"
#include "tables/table_directories.h"
//// TABLE "cmap"

typedef struct __attribute__((packed))
{
    uint16_t length;
    uint16_t language;
    uint8_t glyphIndexArray[256];
} t_cmap_format0;

typedef struct __attribute__((packed))
{
    uint16_t length;
    uint16_t language;
    uint16_t segCountX2;
    uint16_t searchRange;
    uint16_t entrySelector;
    uint16_t rangeShift;
    uint16_t *endCode;         // size --> segCount
    uint16_t reservedPad;      //
    uint16_t *startCode;       // size --> segCount
    uint16_t *idDelta;         // size --> segCount
    uint16_t *idRangeOffset;   // size --> segCount
    uint16_t *glyphIndexArray; // size --> variable
} t_cmap_format4;

typedef union
{
    t_cmap_format0 format0;
    t_cmap_format4 format4;
} u_cmap_subtable_content;

typedef struct __attribute__((packed))
{
    uint16_t platformID;
    uint16_t platformSpecificID;
    uint32_t offset;
    uint16_t format;
    u_cmap_subtable_content content;
} t_cmap_subtable;

typedef struct __attribute__((packed))
{
    uint16_t version;
    uint16_t numberSubtables;
    t_cmap_subtable *subtables;
} t_cmap;

void readCmapTable(int fd, t_cmap *cmap, t_table_directories *table_directories);
void closeCmapTable(t_cmap *cmap);

void printCmapTable(t_cmap *cmap);

size_t getGlyphIndex(unsigned int character, t_cmap *cmap);

#endif