#ifndef TYPES_H
#define TYPES_H

typedef unsigned char byte;
typedef unsigned char uint8_t;

typedef struct Color {
        byte red;
        byte green;
        byte blue;
} Color;

typedef struct ColorPrecision {
        float red;
        float green;
        float blue;
} ColorPrecision;

#endif