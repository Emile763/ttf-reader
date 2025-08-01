#ifndef TABLE_MAXP_H
#define TABLE_MAXP_H

#include "misc/ttf_types.h"
#include "tables/table_directories.h"

//// TABLE "maxp"
typedef struct
{
    Fixed version;
    uint16_t numGlyphs;
    uint16_t maxPoints;
    uint16_t maxContours;
    uint16_t maxComponentPoints;
    uint16_t maxZones;
    uint16_t maxTwilightPoints;
    uint16_t maxStorage;
    uint16_t maxFunctionDefs;
    uint16_t maxInstructionDefs;
    uint16_t maxStackElements;
    uint16_t maxComponentElements;
    uint16_t maxComponentDepth;
} t_maxp;

// typedef struct
// {
//     Fixed version;
//     uint16_t numGlyphs;
// } t_maxp_postscript;

void readMaxpTable(int fd, t_maxp *maxp, t_table_directories *dir);
void printMaxpTable(t_maxp *maxp);

#endif