#include "tables/table_glyph.h"

#include <unistd.h>
#include <stdio.h>
#include "misc/data_read.h"
#include "tables/table_glyph.h"

void readGlyph(int fd, t_glyph* glyph, t_table_directories* table_directories, t_loca* loca);

void readGlyphTable(int fd, t_glyph_array* glyphs, t_table_directories* table_directories, t_maxp* maxp, t_loca* loca)
{
    lseek(fd, table_directories->directories[TAG_GLYF].offset, SEEK_SET);

    glyphs->number_of_glyphs = maxp->numGlyphs;
    glyphs->glyphs = (t_glyph*)malloc(sizeof(t_glyph) * glyphs->number_of_glyphs);

    for (size_t i = 0; i < maxp->numGlyphs; i++)
    {
        readGlyph(fd, &glyphs->glyphs[i], table_directories, loca);
    }
}

// void printGlyphTable(t_glyph_array* glyph)
// {
// }

void closeGlyphTable(t_glyph_array* glyphs)
{
    for (size_t i = 0; i < glyphs->number_of_glyphs; i++) {
        closeGlyph(&(glyphs->glyphs[i]));
    }
}

void closeGlyph(t_glyph* glyph) {
    for (size_t i = 0; i < glyph->number_of_parts; i++) {
        free(glyph->parts[i].endPtsOfContours);
        free(glyph->parts[i].instructions);
        free(glyph->parts[i].onCurve);
        free(glyph->parts[i].xCoordinates);
        free(glyph->parts[i].yCoordinates);
    }
    if (glyph->parts != NULL)
        free(glyph->parts);
}

void readCharGlyph(int fd, t_glyph* glyph, unsigned int character, t_table_directories* table_directories, t_cmap* cmap, t_loca* loca)
{
    // printf("============= %c =============\n", (char)character);
    // printf("=== char code         : %u\n", character);
    size_t glyphIndex = getGlyphIndex(character, cmap);
    printf("=== glyph index       :%zu\n", glyphIndex);
    size_t glyphOffset = getGlyphOffset(glyphIndex, loca);
    printf("=== glyph offset      :%zu\n", glyphOffset);

    lseek(fd, table_directories->directories[TAG_GLYF].offset + glyphOffset, SEEK_SET);
    readGlyph(fd, glyph, table_directories, loca);
}


// private

typedef struct
{
    size_t flags_size;
    size_t xCoordinates_size; //in bytes
    size_t yCoordinates_size; //in bytes
}t_flags_x_y_sizes;


typedef struct
{
    uint8_t* flags;
    void* xCoordinates;
    void* yCoordinates;
} t_packed_coordinates;


t_flags_x_y_sizes computeSizes(int fd, size_t numberOfPoints) {

    t_flags_x_y_sizes sizes;
    sizes.flags_size = 0;
    sizes.xCoordinates_size = 0;
    sizes.yCoordinates_size = 0;

    size_t file_offset = lseek(fd, 0, SEEK_CUR);
    uint8_t flag;
    uint8_t repeatCount = 0;
    for (size_t i = 0; i < numberOfPoints; i++) {

        if (repeatCount > 0) {
            repeatCount--;
        }
        else {
            read(fd, &flag, sizeof(flag));
            sizes.flags_size++;
            if (flag & 0b00001000) { // if flags is re
                read(fd, &repeatCount, sizeof(repeatCount));
                sizes.flags_size++;
            }
        }

        if (flag & 0b00000010) { // x short vector
            sizes.xCoordinates_size += sizeof(uint8_t);
        }
        else if (~flag & 0b00010000) {
            sizes.xCoordinates_size += sizeof(int16_t);
        }

        if (flag & 0b00000100) { // y short vector
            sizes.yCoordinates_size += sizeof(uint8_t);
        }
        else if (~flag & 0b00100000) {
            sizes.yCoordinates_size += sizeof(int16_t);
        }

    }

    lseek(fd, file_offset, SEEK_SET);
    return sizes;
}

void computePointsCoordinates(size_t number_of_points, int16_t* x, int16_t* y, int* onCurve, t_packed_coordinates packed_coordinates) {

    size_t flag_index = 0;
    size_t x_offset = 0;
    size_t y_offset = 0;
    int16_t last_x = 0;
    int16_t last_y = 0;
    int16_t dx = 0;
    int16_t dy = 0;
    size_t repeatCount = 0;

    uint8_t flag;
    for (size_t i = 0; i < number_of_points; i++) {

        if (repeatCount > 0) {
            repeatCount--;
        }
        else {
            flag = packed_coordinates.flags[flag_index];
            flag_index++;
            if (flag & 0b00001000) { // if flags is re
                repeatCount = packed_coordinates.flags[flag_index];
                flag_index++;
            }
        }

        if (flag & 0b00000001) {
            onCurve[i] = 1;
        }

        if (flag & 0b00000010) { // x short vector
            dx = *((uint8_t*)packed_coordinates.xCoordinates + x_offset);
            if (~flag & 0b00010000) {
                dx = -1 * dx;
            }
            x_offset += sizeof(uint8_t);
        }
        else if (flag & 0b00010000) {
            dx = 0;
        }
        else {
            dx = *(int16_t*)((uint8_t*)packed_coordinates.xCoordinates + x_offset);
            dx = be16toh(dx);
            x_offset += sizeof(int16_t);
        }

        if (flag & 0b00000100) { // y short vector
            dy = *((uint8_t*)packed_coordinates.yCoordinates + y_offset);
            if (~flag & 0b00100000) {
                dy = -1 * dy;
            }
            y_offset += sizeof(uint8_t);
        }
        else if (flag & 0b00100000) {
            dy = 0;
        }
        else {
            dy = *(int16_t*)((uint8_t*)packed_coordinates.yCoordinates + y_offset);
            dy = be16toh(dy);
            y_offset += sizeof(int16_t);
        }



        x[i] = dx + last_x;
        y[i] = dy + last_y;
        last_x = x[i];
        last_y = y[i];


    }

}

void readSimpleGlyph(int fd, t_simple_glyph* glyph, int16_t numberOfContours) {
    glyph->endPtsOfContours = readArray16(fd, numberOfContours);
    read(fd, &(glyph->instructionLength), sizeof(uint16_t));
    glyph->instructionLength = be16toh(glyph->instructionLength);

    glyph->instructions = (uint8_t*)readArray(fd, glyph->instructionLength);

    glyph->numberOfPoints = glyph->endPtsOfContours[numberOfContours - 1] + 1;
    glyph->xCoordinates = (int16_t*)malloc(glyph->numberOfPoints * sizeof(glyph->xCoordinates));
    glyph->yCoordinates = (int16_t*)malloc(glyph->numberOfPoints * sizeof(glyph->yCoordinates));
    glyph->onCurve = (int*)malloc(glyph->numberOfPoints * sizeof(glyph->onCurve));


    t_flags_x_y_sizes packed_sizes = computeSizes(fd, glyph->numberOfPoints);

    t_packed_coordinates packed_coordinates;
    packed_coordinates.flags = (uint8_t*)readArray(fd, packed_sizes.flags_size);
    packed_coordinates.xCoordinates = readArray(fd, packed_sizes.xCoordinates_size);
    packed_coordinates.yCoordinates = readArray(fd, packed_sizes.yCoordinates_size);


    computePointsCoordinates(glyph->numberOfPoints, glyph->xCoordinates, glyph->yCoordinates, glyph->onCurve, packed_coordinates);

    // printf("=== number_of_points  : %zu\n", glyph->numberOfPoints);

    free(packed_coordinates.flags);
    free(packed_coordinates.xCoordinates);
    free(packed_coordinates.yCoordinates);

}
#define ARG_1_AND_2_ARE_WORDS 0x0001
#define ARGS_ARE_XY_VALUES 0x0002
#define ROUND_XY_TO_GRID 0x0004
#define WE_HAVE_A_SCALE 0x0008
#define MORE_COMPONENTS 0x0020
#define WE_HAVE_AN_X_AND_Y_SCALE 0x0040
#define WE_HAVE_A_TWO_BY_TWO 0x0080
#define WE_HAVE_INSTRUCTIONS 0x0100
#define USE_MY_METRICS 0x0200
#define OVERLAP_COMPOUND 0x0400
#define SCALED_COMPONENT_OFFSET 0x0800
#define UNSCALED_COMPONENT_OFFSET 0x1000


// TODO case when not ARGS_ARE_XY_VALUES
size_t readCompoundGlyph(int fd, t_glyph* glyph, t_table_directories* table_directories, t_loca *loca) {
    size_t number_of_parts = 0;
    uint16_t flags = 0;
    uint16_t glyphIndex;
    int32_t arg1;
    int32_t arg2;
    float xscalef;
    float yscalef;
    float scale01f;
    float scale10f;

    do {
        read(fd, &flags, sizeof(flags));
        flags = be16toh(flags);

        read(fd, &glyphIndex, sizeof(glyphIndex));
        glyphIndex = be16toh(glyphIndex);


        if (flags & ARG_1_AND_2_ARE_WORDS) {
            if (flags & ARGS_ARE_XY_VALUES) {
                int16_t argument1;
                int16_t argument2;

                read(fd, &argument1, sizeof(argument1));
                argument1 = be16toh(argument1);

                read(fd, &argument2, sizeof(argument2));
                argument2 = be16toh(argument2);

                arg1 = (int32_t)argument1;
                arg2 = (int32_t)argument2;
            }
            else {
                FWord argument1;
                FWord argument2;
                read(fd, &argument1, sizeof(argument1));
                argument1 = be16toh(argument1);

                read(fd, &argument2, sizeof(argument2));
                argument2 = be16toh(argument2);

                arg1 = (int32_t)argument1;
                arg2 = (int32_t)argument2;
            }

        }
        else {

            if (flags & ARGS_ARE_XY_VALUES) {
                int8_t argument1;
                int8_t argument2;

                read(fd, &argument1, sizeof(argument1));

                read(fd, &argument2, sizeof(argument2));

                arg1 = (int32_t)argument1;
                arg2 = (int32_t)argument2;
            }
            else {
                uint8_t argument1;
                uint8_t argument2;
                read(fd, &argument1, sizeof(argument1));

                read(fd, &argument2, sizeof(argument2));

                arg1 = (int32_t)argument1;
                arg2 = (int32_t)argument2;
            }
        }

        if (flags & WE_HAVE_A_SCALE) {
            F2DOT14  scale;    /* Format 2.14 */
            read(fd, &scale, sizeof(scale));
            scale = be16toh(scale);

            xscalef = F2DOT14toFloat(scale);
            yscalef = xscalef;
            scale01f = 0.f;
            scale10f = 0.f;
        }
        else if (flags & WE_HAVE_AN_X_AND_Y_SCALE) {
            F2DOT14  xscale;    /* Format 2.14 */
            F2DOT14  yscale;    /* Format 2.14 */
            read(fd, &xscale, sizeof(xscale));
            xscale = be16toh(xscale);
            read(fd, &yscale, sizeof(yscale));
            yscale = be16toh(yscale);

            xscalef = F2DOT14toFloat(xscale);
            yscalef = F2DOT14toFloat(yscale);
            scale01f = 0.f;
            scale10f = 0.f;
        }
        else if (flags & WE_HAVE_A_TWO_BY_TWO) {
            F2DOT14  xscale;    /* Format 2.14 */
            F2DOT14  scale01;   /* Format 2.14 */
            F2DOT14  scale10;   /* Format 2.14 */
            F2DOT14  yscale;    /* Format 2.14 */

            read(fd, &xscale, sizeof(xscale));
            xscale = be16toh(xscale);
            read(fd, &scale01, sizeof(scale01));
            scale01 = be16toh(scale01);
            read(fd, &scale10, sizeof(scale10));
            scale10 = be16toh(scale10);
            read(fd, &yscale, sizeof(yscale));
            yscale = be16toh(yscale);

            xscalef = F2DOT14toFloat(xscale);
            yscalef = F2DOT14toFloat(yscale);
            scale01f = F2DOT14toFloat(scale01);
            scale10f = F2DOT14toFloat(scale10);
        }
        else {
            xscalef = 1.0f;
            yscalef = 1.0f;
            scale01f = 0.f;
            scale10f = 0.f;
        }

        if (flags & ARGS_ARE_XY_VALUES) {
            if (flags & SCALED_COMPONENT_OFFSET) {
                arg1 = arg1 * xscalef + arg2 * scale10f;
                arg2 = arg1 * scale01f + arg2 * yscalef;
            }
        }

        off_t file_offset = lseek(fd, 0, SEEK_CUR);

        size_t glyph_offset = getGlyphOffset(glyphIndex, loca);
        lseek(fd, table_directories->directories[TAG_GLYF].offset + glyph_offset, SEEK_SET);
        t_glyph partial_glyph;
        read(fd, &partial_glyph, sizeof(int16_t) * 5);
        partial_glyph.number_of_contours = be16toh(partial_glyph.number_of_contours);

        readSimpleGlyph(fd, &glyph->parts[number_of_parts], partial_glyph.number_of_contours);

        for (size_t i = 0; i < glyph->parts[number_of_parts].numberOfPoints; i++) {
            glyph->parts[number_of_parts].xCoordinates[i] = glyph->parts[number_of_parts].xCoordinates[i] * xscalef  + glyph->parts[number_of_parts].yCoordinates[i] * scale10f;
            glyph->parts[number_of_parts].yCoordinates[i] = glyph->parts[number_of_parts].xCoordinates[i] * scale01f + glyph->parts[number_of_parts].yCoordinates[i] * yscalef;
        }

        if (ARGS_ARE_XY_VALUES) {
            for (size_t i = 0; i < glyph->parts[number_of_parts].numberOfPoints; i++) {
                glyph->parts[number_of_parts].xCoordinates[i] += arg1;
                glyph->parts[number_of_parts].yCoordinates[i] += arg2;
            }
        }

        number_of_parts++;


        lseek(fd, file_offset, SEEK_SET);
    } while (flags & MORE_COMPONENTS);

    // if (flags & WE_HAVE_INSTRUCTIONS) {
    //     uint16 numInstr
    //     uint8 instr[numInstr]
    // }
    return number_of_parts;
}





#define MAX_COMPOUND 256

void readGlyph(int fd, t_glyph* glyph, t_table_directories* table_directories, t_loca *loca) {
    read(fd, glyph, sizeof(int16_t) * 5);
    glyph->number_of_contours = be16toh(glyph->number_of_contours);
    glyph->xMin = be16toh(glyph->xMin);
    glyph->yMin = be16toh(glyph->yMin);
    glyph->xMax = be16toh(glyph->xMax);
    glyph->yMax = be16toh(glyph->yMax);


    if (glyph->number_of_contours > 0) {
        glyph->number_of_parts = 1;
        glyph->parts = (t_simple_glyph*)malloc(sizeof(t_simple_glyph));
        readSimpleGlyph(fd, glyph->parts, glyph->number_of_contours);
    }
    else {
        glyph->number_of_parts = 0;
        glyph->parts = (t_simple_glyph*)malloc(MAX_COMPOUND * sizeof(t_simple_glyph));
        glyph->number_of_parts = readCompoundGlyph(fd, glyph, table_directories, loca);
        t_simple_glyph* temp = (t_simple_glyph*)realloc(glyph->parts, glyph->number_of_parts * sizeof(t_simple_glyph));
        glyph->parts = temp;
    }
}