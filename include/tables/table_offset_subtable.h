#ifndef TABLE_OFFSET_SUBTABLE_H
#define TABLE_OFFSET_SUBTABLE_H

#include "misc/ttf_types.h"

//// offset subtable
typedef struct __attribute__((packed))
{
    uint32_t scalerType;
    uint16_t numTables;
    uint16_t searchRange;
    uint16_t entrySelector;
    uint16_t rangeShift;
} t_offset_subtable;

// read the offset subtable
// the file cursor is moved to the end of the offset_subtable
void readOffsetSubtable(int fd, t_offset_subtable *offset_subtable);

void printOffsetSubtable(t_offset_subtable *offset_subtable);

#endif