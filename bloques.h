#ifndef BLOQUES_H
#define BLOQUES_H
struct Content
{
    char name[12];
    int b_inodo;
};

struct Bcarpeta
{
    Content b_content[4];
};

struct Barchivo
{
    char b_content[64];
};

struct Bapuntador
{
    int b_pointers[16];
};

#endif // BLOQUES_H
