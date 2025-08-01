#include "tables/table_maxp.h"

#include <endian.h>
#include <unistd.h>
#include <stdio.h>

//// TABLE "maxp"
void readMaxpTable(int fd, t_maxp *maxp, t_table_directories *dir)
{
    lseek(fd, dir->directories[TAG_MAXP].offset, SEEK_SET);
    read(fd, maxp, sizeof(t_maxp));
    maxp->version = be32toh(maxp->version);
    maxp->numGlyphs = be16toh(maxp->numGlyphs);
    maxp->maxPoints = be16toh(maxp->maxPoints);
    maxp->maxContours = be16toh(maxp->maxContours);
    maxp->maxComponentPoints = be16toh(maxp->maxComponentPoints);
    maxp->maxZones = be16toh(maxp->maxZones);
    maxp->maxTwilightPoints = be16toh(maxp->maxTwilightPoints);
    maxp->maxStorage = be16toh(maxp->maxStorage);
    maxp->maxFunctionDefs = be16toh(maxp->maxFunctionDefs);
    maxp->maxInstructionDefs = be16toh(maxp->maxInstructionDefs);
    maxp->maxStackElements = be16toh(maxp->maxStackElements);
    maxp->maxComponentElements = be16toh(maxp->maxComponentElements);
    maxp->maxComponentDepth = be16toh(maxp->maxComponentDepth);
}

void printMaxpTable(t_maxp *maxp)
{
    printf("=====> maxp <=====\n");
    printf("===> version                :%u\n", maxp->version);
    printf("===> numGlyphs              :%d\n", maxp->numGlyphs);
    printf("===> maxPoints              :%d\n", maxp->maxPoints);
    printf("===> maxContours            :%u\n", maxp->maxContours);
    printf("===> maxComponentPoints     :%d\n", maxp->maxComponentPoints);
    printf("===> maxZones               :%d\n", maxp->maxZones);
    printf("===> maxTwilightPoints      :%d\n", maxp->maxTwilightPoints);
    printf("===> maxStorage             :%d\n", maxp->maxStorage);
    printf("===> maxFunctionDefs        :%d\n", maxp->maxFunctionDefs);
    printf("===> maxInstructionDefs     :%d\n", maxp->maxInstructionDefs);
    printf("===> maxStackElements       :%d\n", maxp->maxStackElements);
    printf("===> maxComponentElements   :%d\n", maxp->maxComponentElements);
    printf("===> maxComponentDepth      :%d\n", maxp->maxComponentDepth);
}