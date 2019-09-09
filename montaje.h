#ifndef MONTAJE_H
#define MONTAJE_H
#include<string>
#include<vector>
using namespace std;
struct MountPart{
    char idPart[100]="";
    char idMount[100]="";
};
struct MountDisk{
    struct MountPart mountPart[20];
    char idDisk[100] = "";
    int countParts = 0;
};
#endif // MONTAJE_H
