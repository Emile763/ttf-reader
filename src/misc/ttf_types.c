#include "misc/ttf_types.h"
#include "misc/ttf_types.h"

const unsigned int NUMBER_OF_REQUIRED_TAG = 9;

const char tagEToS[NUMBER_OF_TAG][5] = {
    "cmap", "glyf", "head", "hhea",
    "hmtx", "loca", "maxp", "name",
    "post", "cvt ", "fpgm", "hdmx", 
    "kern", "OS/2", "prep"};

const uint32_t tagEtoUint32[NUMBER_OF_TAG] = {
    'c' | 'm' << 8 | 'a' << 16 | 'p' << 24,
    'g' | 'l' << 8 | 'y' << 16 | 'f' << 24,
    'h' | 'e' << 8 | 'a' << 16 | 'd' << 24,
    'h' | 'h' << 8 | 'e' << 16 | 'a' << 24,
    'h' | 'm' << 8 | 't' << 16 | 'x' << 24,
    'l' | 'o' << 8 | 'c' << 16 | 'a' << 24,
    'm' | 'a' << 8 | 'x' << 16 | 'p' << 24,
    'n' | 'a' << 8 | 'm' << 16 | 'e' << 24,
    'p' | 'o' << 8 | 's' << 16 | 't' << 24,
    'c' | 'v' << 8 | 't' << 16 | ' ' << 24,
    'f' | 'p' << 8 | 'g' << 16 | 'm' << 24,
    'h' | 'd' << 8 | 'm' << 16 | 'x' << 24,
    'k' | 'e' << 8 | 'r' << 16 | 'n' << 24,
    'O' | 'S' << 8 | '/' << 16 | '2' << 24,
    'p' | 'r' << 8 | 'e' << 16 | 'p' << 24};

e_tag tagToEnum(uint32_t tag)
{
    switch (tag)
    {
    case (uint32_t)1885433187U:
        return TAG_CMAP;
    case (uint32_t)1719233639U:
        return TAG_GLYF;
    case (uint32_t)1684104552U:
        return TAG_HEAD;
    case (uint32_t)1634035816U:
        return TAG_HHEA;
    case (uint32_t)2020896104U:
        return TAG_HMTX;
    case (uint32_t)1633906540U:
        return TAG_LOCA;
    case (uint32_t)1886937453U:
        return TAG_MAXP;
    case (uint32_t)1701667182U:
        return TAG_NAME;
    case (uint32_t)1953722224U:
        return TAG_POST;
    case (uint32_t)544503395U:
        return TAG_CVT;
    case (uint32_t)1835495526U:
        return TAG_FPGM;
    case (uint32_t)2020435048U:
        return TAG_HDMX;
    case (uint32_t)1852990827U:
        return TAG_KERN;
    case (uint32_t)841962319U:
        return TAG_OS2;
    case (uint32_t)1885696624U:
        return TAG_PREP;
    default:
        return TAG_UNKNOWN;
    }
}


float F2DOT14toFloat(F2DOT14 x)
{

    int16_t i = ((x & 0x4000) >> 14);
    if(x & 0x8000)
        i |= 0xfffe;

    float y = i + (float) (x & 0x3fff) / 0x3fff;
    
    return y;
}