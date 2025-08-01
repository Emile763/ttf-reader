#ifndef TABLE_GLYPH_H
#define TABLE_GLYPH_H
#include "tables/table_directories.h"
#include "tables/table_maxp.h"
#include "tables/table_cmap.h"
#include "tables/table_loca.h"

typedef struct __attribute__((packed))
{
    uint16_t* endPtsOfContours; //size: numberOfContours
    uint16_t instructionLength;
    uint8_t* instructions; // size: instructionLength

    size_t numberOfPoints;
    int* onCurve;           // size of numberOfPoint
    int16_t* xCoordinates;  // size of numberOfPoint
    int16_t* yCoordinates;  // size of numberOfPoint

} t_simple_glyph;




typedef struct __attribute__((packed))
{
    int16_t number_of_contours;
    FWord xMin;
    FWord yMin;
    FWord xMax;
    FWord yMax;
    
    size_t number_of_parts;
    t_simple_glyph *parts;

} t_glyph;

typedef struct
{
    size_t number_of_glyphs;
    t_glyph* glyphs;
} t_glyph_array;

void readGlyphTable(int fd, t_glyph_array* glyphs, t_table_directories* table_directories, t_maxp* maxp, t_loca *loca);

// void printGlyphTable(t_glyph_array* glyph);

void closeGlyphTable(t_glyph_array* glyph);

void closeGlyph(t_glyph* glyph);

void readCharGlyph(int fd, t_glyph* glyph, unsigned int character,  t_table_directories* table_directories, t_cmap* cmap, t_loca* loca);
#endif