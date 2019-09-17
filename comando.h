#ifndef COMANDO_H
#define COMANDO_H
#include"funciones.h"
#include"reporte.h"
class Comando
{
public:
    //variables
    Funciones funciones;
    MountDisk mountDisk[20];
    Reporte reporte;
    Log_usuario log_usuario;
    vector<Usuario> lista_u;
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
};

#endif // COMANDO_H
