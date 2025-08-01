#ifndef TABLE_HHEA_H
#define TABLE_HHEA_H

#include "misc/ttf_types.h"
#include "tables/table_directories.h"
//// TABLE "hhea"
typedef struct __attribute__((packed))
{
    Fixed version;
    FWord ascent;
    FWord descent;
    uFWord advanceWidthMax;
    FWord minLeftSideBearing;
    FWord minRightSideBearing;
    FWord xMaxExtent;
    int16_t caretSlopeRise;
    int16_t caretSlopeRun;
    FWord caretOffset;
    int16_t reserved[4];
    int16_t metricDataFormat;
    uint16_t numOfLongHorMetrics;
} t_hhea;

void readHheaTable(int fd, t_hhea *hhea, t_table_directories *dir);
void printHheaTable(t_hhea *hhea);

#endif