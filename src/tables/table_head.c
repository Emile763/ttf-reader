#include "tables/table_head.h"

#include <endian.h>
#include <stdio.h>
#include <unistd.h>

void readHeadTable(int fd, t_head *head, t_table_directories *dir)
{
    lseek(fd, dir->directories[TAG_HEAD].offset, SEEK_SET);
    read(fd, head, sizeof(t_head));
    head->version = be32toh(head->version);
    head->fontRevision = be32toh(head->fontRevision);
    head->checkSumAdjustment = be32toh(head->checkSumAdjustment);
    head->magicNumber = be32toh(head->magicNumber);
    head->flags = be16toh(head->flags);
    head->unitsPerEm = be16toh(head->unitsPerEm);
    head->created = be64toh(head->created);
    head->modified = be64toh(head->modified);
    head->xMin = be16toh(head->xMin);
    head->yMin = be16toh(head->yMin);
    head->xMax = be16toh(head->xMax);
    head->yMax = be16toh(head->yMax);
    head->macStyle = be16toh(head->macStyle);
    head->lowestRecPPEM = be16toh(head->lowestRecPPEM);
    head->fontDirectionHint = be16toh(head->fontDirectionHint);
    head->indexToLocFormat = be16toh(head->indexToLocFormat);
    head->glyphDataFormat = be16toh(head->glyphDataFormat);
}

void printHeadTable(t_head *head)
{
    printf("=====> head <=====\n");
    printf("===> version            :%d\n", head->version);
    printf("===> fontRevision       :%d\n", head->fontRevision);
    printf("===> checkSumAdjustment :%u\n", head->checkSumAdjustment);
    printf("===> magicNumber        :%u\n", head->magicNumber);
    printf("===> flags              :%u\n", head->flags);
    printf("===> unitsPerEm         :%u\n", head->unitsPerEm);
    printf("===> created            :%ld\n", head->created);
    printf("===> modified           :%ld\n", head->modified);
    printf("===> xMin               :%d\n", head->xMin);
    printf("===> yMin               :%d\n", head->yMin);
    printf("===> xMax               :%d\n", head->xMax);
    printf("===> yMax               :%d\n", head->yMax);
    printf("===> macStyle           :%u\n", head->macStyle);
    printf("===> lowestRecPPEM      :%u\n", head->lowestRecPPEM);
    printf("===> indexToLocFormat   :%d\n", head->glyphDataFormat);
    printf("===> glyphDataFormat    :%d\n", head->glyphDataFormat);
}
