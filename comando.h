#ifndef COMANDO_H
#define COMANDO_H
#include"funciones.h"
#include"reporte.h"
using namespace std;
class Comando
{
public:
    //variables
    Funciones funciones;
    MountDisk mountDisk[20];
    Reporte reporte;
    Log_usuario log_usuario;

    //funciones
    void Mkdisk(string path, int sizeB, char unit, string fit);
    void Rmdisk(string path);
    void Fdisk(string path, string name, int sizeB, char unit, char type, char fit);
    void FdiskDelete(string path, string name, string deleteType);
    void FdiskAdd(string name,string path,char unit,int add);
    void Mount(string path, string name);
    void UnMountPartition(const char* id);
    void Rep(string id,string path,string name);
    void mkfs(string id, string type);
    void login(string id,string usr,string pwd);
    void logout();
    void mkgrp(string name);
    void rmgrp(string name);
    void mkusr(string grp, string usr, string pwd);
    void rmusr(string name);
    void mkdir(string dir, bool p);
    void mkfile(string dir, bool p,string cont, int size);
    void cat(string file);
    StringVector recovery(string id);
    void loss(string id);
};

#endif // COMANDO_H
