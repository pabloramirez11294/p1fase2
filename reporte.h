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
    void GenerarIMGDOT(const char *Lenguaje, const char *Ruta_Arcivo, const char *Ruta_Img);
};

#endif // REPORTE_H
