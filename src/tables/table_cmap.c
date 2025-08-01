#include "tables/table_cmap.h"
#include "misc/data_read.h"

#include <endian.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tables/table_cmap.h"


//// TABLE "cmap"
void readCmapFormat0(int fd, t_cmap_format0* subtable);
void readCmapFormat4(int fd, t_cmap_format4* subtable);

void readCmapSubtable(int fd, t_cmap_subtable* subtable);

void readCmapTable(int fd, t_cmap* cmap, t_table_directories* table_directories)
{
    lseek(fd, table_directories->directories[TAG_CMAP].offset, SEEK_SET);

    //  read version & number of subtables
    read(fd, cmap, sizeof(cmap->version) + sizeof(cmap->numberSubtables));
    cmap->version = be16toh(cmap->version);
    cmap->numberSubtables = be16toh(cmap->numberSubtables);

    cmap->subtables = (t_cmap_subtable*)malloc(cmap->numberSubtables * sizeof(t_cmap_subtable));

    for (size_t i = 0; i < cmap->numberSubtables; i++)
    {
        // read plaformID, platformSpecificID & offset
        read(
            fd, &(cmap->subtables[i]),
            sizeof(cmap->subtables[i].platformID) +
            sizeof(cmap->subtables[i].platformSpecificID) +
            sizeof(cmap->subtables[i].offset));
        cmap->subtables[i].platformID = be16toh(cmap->subtables[i].platformID);
        cmap->subtables[i].platformSpecificID = be16toh(cmap->subtables[i].platformSpecificID);
        cmap->subtables[i].offset = be32toh(cmap->subtables[i].offset);
    }

    for (size_t i = 0; i < cmap->numberSubtables; i++)
    {
        lseek(fd, cmap->subtables[i].offset + table_directories->directories[TAG_CMAP].offset, SEEK_SET);
        readCmapSubtable(fd, &(cmap->subtables)[i]);
    }
}

void closeCmapTable(t_cmap* cmap)
{
    for (size_t i = 0; i < cmap->numberSubtables; i++)
    {
        switch (cmap->subtables[i].format)
        {
        case 0:
            break;
        case 4:
            free(cmap->subtables[i].content.format4.endCode);
            free(cmap->subtables[i].content.format4.startCode);
            free(cmap->subtables[i].content.format4.idDelta);
            free(cmap->subtables[i].content.format4.idRangeOffset);
            free(cmap->subtables[i].content.format4.glyphIndexArray);
            break;
        default:
            break;
        }
    }
    free(cmap->subtables);
}

void printCmapTable(t_cmap* cmap)
{
    printf("====> cmap <====\n");
    printf("===> version             : %u\n", cmap->version);
    printf("===> number of subtables : %u\n", cmap->numberSubtables);
    for (size_t i = 0; i < cmap->numberSubtables; i++)
    {
        printf("    ====> %zu <====\n", i);
        printf("    ===> platformID         : %u\n", cmap->subtables[i].platformID);
        printf("    ===> platformSpecificID : %u\n", cmap->subtables[i].platformSpecificID);
        printf("    ===> offset             : %u\n", cmap->subtables[i].offset);
        printf("    ===> format             : %u\n", cmap->subtables[i].format);
        switch (cmap->subtables[i].format)
        {
        case 0:
            printf("    ===> length             : %u\n", cmap->subtables[i].content.format0.length);
            printf("    ===> language           : %u\n", cmap->subtables[i].content.format0.language);
            break;
        case 4:
            printf("    ===> length             : %u\n", cmap->subtables[i].content.format4.length);
            printf("    ===> language           : %u\n", cmap->subtables[i].content.format4.language);
            printf("    ===> segCountX2         : %u\n", cmap->subtables[i].content.format4.segCountX2);
            printf("    ===> searchRange        : %u\n", cmap->subtables[i].content.format4.searchRange);
            printf("    ===> entrySelector      : %u\n", cmap->subtables[i].content.format4.entrySelector);
            printf("    ===> rangeShift         : %u\n", cmap->subtables[i].content.format4.rangeShift);
            break;
        default:
            break;
        }
    }
}


size_t getFormat0GlyphIndex(uint16_t character, t_cmap_format0* subtable);
size_t getFormat4GlyphIndex(uint16_t character, t_cmap_format4* subtable);


size_t getGlyphIndex(unsigned int character, t_cmap* cmap)
{
    t_cmap_subtable* cmap_subtable = NULL;
    for (size_t i = 0; i < cmap->numberSubtables; i++) {
        if (cmap->subtables[i].platformID == 0) { // get unicode subtable
            cmap_subtable = &(cmap->subtables[i]);
        }
    }
    assert(cmap_subtable != NULL);

    switch (cmap_subtable->format)
    {
    case 0:
        return getFormat0GlyphIndex(character, &(cmap_subtable->content.format0));
    case 4:
        return getFormat4GlyphIndex(character, &(cmap_subtable->content.format4));
    default:
        return 0;
    }
}
// private functions

void readCmapFormat0(int fd, t_cmap_format0* subtable)
{
    read(fd, subtable, sizeof(t_cmap_format0));
    subtable->length = be16toh(subtable->length);
    subtable->language = be16toh(subtable->language);
}
void readCmapFormat4(int fd, t_cmap_format4* subtable)
{
    // read length, language, segCountX2, searchRange, entrySelector, rangeShift
    read(fd, subtable, 6 * sizeof(uint16_t));
    subtable->length = be16toh(subtable->length); // begin at "format" parameter
    subtable->language = be16toh(subtable->language);
    subtable->segCountX2 = be16toh(subtable->segCountX2);
    subtable->searchRange = be16toh(subtable->searchRange);
    subtable->entrySelector = be16toh(subtable->entrySelector);
    subtable->rangeShift = be16toh(subtable->rangeShift);

    size_t segCount = subtable->segCountX2 / 2;
    subtable->endCode = readArray16(fd, segCount);

    read(fd, &(subtable->reservedPad), sizeof(uint16_t));

    subtable->startCode = readArray16(fd, segCount);

    subtable->idDelta = readArray16(fd, segCount);

    subtable->idRangeOffset = readArray16(fd, segCount);

    size_t glyfIndexArraysize = subtable->length / 2 - 4 * segCount - 8;
    subtable->glyphIndexArray = readArray16(fd, glyfIndexArraysize);
}

void readCmapSubtable(int fd, t_cmap_subtable* subtable)
{
    read(fd, &(subtable->format), sizeof(subtable->format));
    subtable->format = be16toh(subtable->format);
    switch (subtable->format)
    {
    case 0:
        readCmapFormat0(fd, &(subtable->content.format0));
        break;
    case 4:
        readCmapFormat4(fd, &(subtable->content.format4));
        break;
    default:
        printf("Unsupported cmap format: %u\n", subtable->format);
        break;
    }
}

size_t getFormat0GlyphIndex(uint16_t character, t_cmap_format0* subtable)
{
    // TO BE DEFINED
    return subtable->glyphIndexArray[character];
}

size_t getFormat4GlyphIndex(uint16_t character, t_cmap_format4* subtable)
{
    uint16_t segcount = subtable->segCountX2 / 2;
    for (size_t i = 0; i < segcount; i++) {
        if (character < subtable->startCode[i])
            return 0;

        if (character <= subtable->endCode[i]) {
            if (subtable->idRangeOffset[i] != 0) {
                size_t glyphIndexAddress = subtable->idRangeOffset[i] / 2 + (character - subtable->startCode[i]) - (segcount - i);
                if (subtable->glyphIndexArray[glyphIndexAddress] != 0)
                    return subtable->glyphIndexArray[glyphIndexAddress] + subtable->idDelta[i];
                else
                    return 0;

            }
            else {
                return subtable->idDelta[i] + character;
            }
        }
    }

    return 0;
}
