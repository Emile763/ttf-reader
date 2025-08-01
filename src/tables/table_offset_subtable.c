#include "tables/table_offset_subtable.h"

#include <endian.h>
#include <stdio.h>
#include <unistd.h>

//// 
void readOffsetSubtable(int fd, t_offset_subtable *offset_subtable)
{
    lseek(fd, 0, SEEK_SET);
    read(fd, offset_subtable, sizeof(t_offset_subtable));
    offset_subtable->scalerType = be32toh(offset_subtable->scalerType);
    offset_subtable->numTables = be16toh(offset_subtable->numTables);
    offset_subtable->searchRange = be16toh(offset_subtable->searchRange);
    offset_subtable->entrySelector = be16toh(offset_subtable->entrySelector);
    offset_subtable->rangeShift = be16toh(offset_subtable->rangeShift);
}

void printOffsetSubtable(t_offset_subtable *offset_subtable)
{
    printf("---- The offset subtable ----\n");
    printf("==> Scaler type: %u\n", offset_subtable->scalerType);
    printf("==> Number of tables: %u\n", offset_subtable->numTables);
    printf("==> Search range: %u\n", offset_subtable->searchRange);
    printf("==> Entry selector: %u\n", offset_subtable->entrySelector);
    printf("==> Range shift: %u\n\n", offset_subtable->rangeShift);
}
