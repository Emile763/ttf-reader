#ifndef TABLE_HEAD_H
#define TABLE_HEAD_H

#include "misc/ttf_types.h"
#include "tables/table_directories.h"

typedef struct __attribute__((packed))
{
    Fixed version;
    Fixed fontRevision;
    uint32_t checkSumAdjustment;
    uint32_t magicNumber; // 0x5F0F3CF5
    uint16_t flags;
    uint16_t unitsPerEm;
    longDateTime created;
    longDateTime modified;
    FWord xMin;
    FWord yMin;
    FWord xMax;
    FWord yMax;
    uint16_t macStyle;
    uint16_t lowestRecPPEM;
    int16_t fontDirectionHint;
    int16_t indexToLocFormat;
    int16_t glyphDataFormat;
} t_head;

void readHeadTable(int fd, t_head *head, t_table_directories *dir);

void printHeadTable(t_head *head);



#endif