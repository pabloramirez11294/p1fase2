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
    string convertToString(char* a, int size);
    StringVector Explode(const std::string & str, char separator );
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
    void setBitMapsInicio(int n,int bm_inodo,int bm_bloque);
    void getBitmap(int n,const char* location, int posInicial,char* bitmap);
    void setBitMapsInicio(int n,int posI_inodo,int posI_bloque,string path);
    Superbloque getSuperbloque(string path,string partName);
    void setSuperbloque(string path,Superbloque superbloque,int pos);
    bool formatearTabla_inodos_bloques(string path,Superbloque superbloque);
    bool ingresarInodo(string path, Inodo inodo, int pos, int num);
    bool ingresar_bitmap(string path, int pos, int num);

    bool ingresarBloqueCarpeta(string path, Bcarpeta carpeta, int pos, int num);
    bool ingresarBloqueArchivo(string path, Barchivo carpeta, int pos, int num);

    Bcarpeta getBcarpeta(string path,int pos, int num);
    Barchivo getBarchivo(string path,int pos, int num);
    Bapuntador getBapuntador(string path,int pos, int num);
    Inodo getInode(string path,int pos, int num);
    bool setBloque(Inodo inodo, string txt, Superbloque superbloque, string path, int num_inodo);
    int bloqueLibre(string path, Superbloque superbloque);
    int inodoLibre(string path, Superbloque superbloque);
    string leerArchivo(string path, Superbloque superbloque,string ruta);
    int buscarInodoRaiz(string path, Superbloque superbloque,string ruta);
    int buscarInodoCarpeta(string path, string nombre, Superbloque superbloque, Inodo raiz);
    bool Fmkdir(Inodo inodo, string dir, string path, Superbloque superbloque);
    int buscartInodoLibreDentroInodoCarpeta(string path,Superbloque superbloque,Inodo inodoAux);
};

#endif // FUNCIONES_H
