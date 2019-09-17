#ifndef REPORTE_H
#define REPORTE_H
#include"librerias.h"
#include"funciones.h"
#include<fstream>
class Reporte
{
public:
    Funciones funciones;
    void RepMbr(const char* path, const char *pathRep);
    void RepDisk(const char* path, const char *pathRep);
    void RepBm(const char* path, const char *pathRep, string id, MountDisk mountDisk[], bool esInodo);
    void RepBlock(const char* path, const char *pathRep, string id, MountDisk mountDisk[]);
    void RepInode(const char* path, const char *pathRep, string id, MountDisk mountDisk[]);
    void GenerarIMGDOT(const char *Lenguaje, const char *Ruta_Arcivo, const char *Ruta_Img);

    string inodoCarpeta(string path, Inodo inodo, int pos, string *text, int *cont);
    string inodoArchivo(string path,Inodo inodo,int pos,string *text,int *cont);
};

#endif // REPORTE_H
