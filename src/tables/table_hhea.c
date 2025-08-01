#include "tables/table_hhea.h"

#include <endian.h>
#include <stdio.h>
#include <unistd.h>

//// TABLE "hhea"

void readHheaTable(int fd, t_hhea *hhea, t_table_directories *dir)
{
    lseek(fd, dir->directories[TAG_HHEA].offset, SEEK_SET);
    read(fd, hhea, sizeof(t_hhea));
    hhea->version = be32toh(hhea->version);
    hhea->ascent = be16toh(hhea->ascent);
    hhea->descent = be16toh(hhea->descent);
    hhea->advanceWidthMax = be16toh(hhea->advanceWidthMax);
    hhea->minLeftSideBearing = be16toh(hhea->minLeftSideBearing);
    hhea->minRightSideBearing = be16toh(hhea->minRightSideBearing);
    hhea->xMaxExtent = be16toh(hhea->xMaxExtent);
    hhea->caretSlopeRise = be16toh(hhea->caretSlopeRise);
    hhea->caretSlopeRun = be16toh(hhea->caretSlopeRun);
    hhea->caretOffset = be16toh(hhea->caretOffset);
    hhea->metricDataFormat = be16toh(hhea->metricDataFormat);
    hhea->numOfLongHorMetrics = be16toh(hhea->numOfLongHorMetrics);
}

void printHheaTable(t_hhea *hhea)
{
    printf("=====> hhea <=====\n");
    printf("===> version                :%u\n", hhea->version);
    printf("===> ascent                 :%d\n", hhea->ascent);
    printf("===> descent                :%d\n", hhea->descent);
    printf("===> advanceWidthMax        :%u\n", hhea->advanceWidthMax);
    printf("===> minLeftSideBearing     :%d\n", hhea->minLeftSideBearing);
    printf("===> minRightSideBearing    :%d\n", hhea->minRightSideBearing);
    printf("===> xMaxExtent             :%d\n", hhea->xMaxExtent);
    printf("===> caretSlopeRise         :%d\n", hhea->caretSlopeRise);
    printf("===> caretSlopeRun          :%d\n", hhea->caretSlopeRun);
    printf("===> caretOffset            :%d\n", hhea->caretOffset);
    printf("===> metricDataFormat       :%d\n", hhea->metricDataFormat);
    printf("===> numOfLongHorMetrics    :%u\n", hhea->numOfLongHorMetrics);
}
