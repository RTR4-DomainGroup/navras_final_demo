#pragma once

typedef struct Color
{
    float r;
    float g;
    float b;
} COLOR;

typedef struct Vertex
{
    float x;
    float y;
    float z;
    float w;
} VERTEX;

typedef struct Quad
{
    VERTEX tr;
    VERTEX tl;
    VERTEX bl;
    VERTEX br;
} QUAD;

typedef void (*draw_FnPtr)(void);

typedef struct Model
{
    char name;
    VERTEX sPos; // starting position
    VERTEX ePos; // ending position
    draw_FnPtr fnptrDraw;
} MODEL;

typedef struct Texture
{
    unsigned int id;
    unsigned int height;
    unsigned int width;
} TEXTURE;

// typedefs
typedef VERTEX TRANFORM;
