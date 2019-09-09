#ifndef FUNCIONES_H
#define FUNCIONES_H
#include"librerias.h"
#include<math.h>
class Funciones
{
public:
    //variables
    int numAl;
    Funciones();
    //funciones varias
    int numAleatorio();
    void createDirs(const char* path);

    //funciones sobre estructs
    Partition getNewPartition();
    MBR getNewMBR();
    MBR leerMBR(const char* location);
    bool escribirMBR(string path, MBR mbr);
    int existPartition(MBR mbr,string name);
    Partition* partitionDisponible(MBR *mbr);
    Partition* get_partition_name(MBR *mbr,string name);
    bool existExtendida(MBR mbr);
    //raid
    void raid(const char* path);


    vector<Partition> ordenarParticiones(MBR mbr);
    vector<Espacio> obtenerEspacios(MBR mbr);
    vector<Espacio> ascendete(vector<Espacio> espacios);

    int getDskMountIndex(const char* path,MountDisk *mountDisk);

    MountDisk existIdPartMount(string id,MountDisk *mountDisk);
    string getPathByID(const char*id,string *partName, MountDisk *mountDisk);
    int numEstructuras(int size_t);
};

#endif // FUNCIONES_H
