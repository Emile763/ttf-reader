#ifndef TTF_TYPES_H
#define TTF_TYPES_H

#include <stdint.h>

typedef uint32_t Fixed;
typedef int64_t longDateTime;
typedef int16_t FWord;
typedef uint16_t uFWord;

typedef uint16_t F2DOT14;

typedef enum
{
    // required
    TAG_CMAP,
    TAG_GLYF,
    TAG_HEAD,
    TAG_HHEA,
    TAG_HMTX,
    TAG_LOCA,
    TAG_MAXP,
    TAG_NAME,
    TAG_POST,
    // optional
    TAG_CVT,
    TAG_FPGM,
    TAG_HDMX,
    TAG_KERN,
    TAG_OS2,
    TAG_PREP,
    NUMBER_OF_TAG,
    TAG_UNKNOWN
} e_tag; // 14 tags

extern const unsigned int NUMBER_OF_REQUIRED_TAG;

extern const char tagEToS[NUMBER_OF_TAG][5];
extern const uint32_t tagEtoUint32[NUMBER_OF_TAG];

e_tag tagToEnum(uint32_t tag);

float F2DOT14toFloat(F2DOT14 x);

#endif