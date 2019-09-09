#include "funciones.h"

Funciones::Funciones(){
    numAl=0;
}

int Funciones::numAleatorio(){
    numAl++;
    srand(time(NULL));
    int test = rand();
    return test+numAl;
}
void Funciones::createDirs(const char* path)
{
    char entrada[1000];memset(entrada,0,sizeof entrada);
    int j=0;
    int i=0;
    while( true){
        char tok = path[j++];
        entrada[i++] = tok;
        if(tok=='/')
        {
            mkdir(entrada,0777);
        }
        if(tok=='\0')
        {
            return;
        }
    }
}


Partition Funciones::getNewPartition()
{
    Partition part;
    part.part_status = 0;
    part.part_type = 0;
    part.part_fit = 0;
    part.part_start = 0;
    part.part_size = 0;
    memset(part.part_name,0,sizeof part.part_name);
    return part;
}

MBR Funciones::getNewMBR()
{
    MBR newMBR;
    newMBR.mbr_tamano = 0;
    newMBR.mbr_fecha_creacion = 0;
    newMBR.mbr_disk_signature = 0;
    newMBR.disk_fit = 0;
    newMBR.mbr_partition1 = getNewPartition();
    newMBR.mbr_partition2 = getNewPartition();
    newMBR.mbr_partition3 = getNewPartition();
    newMBR.mbr_partition4 = getNewPartition();

    return newMBR;
}


MBR Funciones::leerMBR(const char* location){
    MBR mbr=getNewMBR();
    FILE *ptr_file;
    ptr_file = fopen(location,"rb");
    if(!ptr_file)
    {
        return mbr;
    }
    fread(&mbr,sizeof(MBR),1,ptr_file);
    fclose(ptr_file);
    return mbr;
}

bool Funciones::escribirMBR(string path,MBR mbr){
    FILE *ptr_file;
    ptr_file = fopen(path.c_str(),"rb+");
    if(!ptr_file)
    {
        return false;
    }
    fseek(ptr_file,0,SEEK_SET);
    fwrite(&mbr,sizeof(MBR),1,ptr_file);
    fclose(ptr_file);
    this->raid(path.c_str());
    return true;
}

int Funciones::existPartition(MBR mbr,string name)
{
    if(strcmp(mbr.mbr_partition1.part_name,name.c_str()) == 0 && mbr.mbr_partition1.part_status =='1')
        return 1;
    if(strcmp(mbr.mbr_partition2.part_name,name.c_str()) == 0 && mbr.mbr_partition2.part_status =='1')
        return 2;
    if(strcmp(mbr.mbr_partition3.part_name,name.c_str()) == 0 && mbr.mbr_partition3.part_status =='1')
        return 3;
    if(strcmp(mbr.mbr_partition4.part_name,name.c_str()) == 0 && mbr.mbr_partition4.part_status =='1')
        return 4;
    return 0;
}

Partition* Funciones::partitionDisponible(MBR *mbr){
    if(mbr->mbr_partition1.part_status != '1' )
        return &mbr->mbr_partition1;
    if(mbr->mbr_partition2.part_status != '1' )
        return &mbr->mbr_partition2;
    if(mbr->mbr_partition3.part_status != '1' )
        return &mbr->mbr_partition3;
    if(mbr->mbr_partition4.part_status != '1' )
        return &mbr->mbr_partition4;

    return NULL;
}

Partition* Funciones::get_partition_name(MBR *mbr, string name){
    if(mbr->mbr_partition1.part_status == '1' && name.compare(mbr->mbr_partition1.part_name)==0)
        return &mbr->mbr_partition1;
    if(mbr->mbr_partition2.part_status == '1' && name.compare(mbr->mbr_partition2.part_name)==0)
        return &mbr->mbr_partition2;
    if(mbr->mbr_partition3.part_status == '1' && name.compare(mbr->mbr_partition3.part_name)==0)
        return &mbr->mbr_partition3;
    if(mbr->mbr_partition4.part_status == '1'&& name.compare(mbr->mbr_partition4.part_name)==0 )
        return &mbr->mbr_partition4;

    return NULL;
}

bool Funciones::existExtendida(MBR mbr){
    if(mbr.mbr_partition1.part_type=='E' || mbr.mbr_partition2.part_type=='E'
            || mbr.mbr_partition3.part_type=='E' || mbr.mbr_partition4.part_type=='E'){
        return true;
    }
    return false;
}
void Funciones::raid(const char *path){
    string str=path;
    str+="_ra1";
    int pos = str.find_first_of(".");
    str[pos++]='_';
    str[pos++]='r';
    str[pos++]='a';
    str[pos++]='1';
    str[pos++]='.';
    str[pos++]='d';
    str[pos++]='i';
    str[pos++]='s';
    str[pos++]='k';

    remove(str.c_str());

    FILE *entrada;
    FILE *salida;
    int ch;

    if ((entrada = fopen(path, "rb")) == NULL){
      perror(path);
      return;
    }
    if ((salida = fopen(str.c_str(), "wb")) == NULL){
      perror(str.c_str());
      return;
    }

    while ((ch = fgetc(entrada)) != EOF)
      fputc(ch, salida);

    fclose(entrada);
    fclose(salida);

}

vector<Partition> Funciones::ordenarParticiones(MBR mbr){
    Partition aux,parMBR;
    parMBR.part_size=sizeof(MBR);
    parMBR.part_start=0;
    parMBR.part_status='1';
    parMBR.part_name[0]='M';
    parMBR.part_name[1]='B';
    parMBR.part_name[2]='R';
    parMBR.part_name[3]='\0';
    parMBR.part_type='M';
    vector<Partition> part={parMBR,mbr.mbr_partition1,mbr.mbr_partition2,mbr.mbr_partition3,mbr.mbr_partition4};
    int n=part.size();



    //ocultar particiones desactivadas
    for (int i = 0; i < n ; i++){
        if(part[i].part_status!='1'){
            part[i].part_size=0;
            part[i].part_start=-1;
        }
    }

    //orden ascendente

    for (int i = 0; i < n - 1; i++) {
          for (int j = 0; j < n - 1; j++) {
              if (part[j].part_start > part[j + 1].part_start) {
                  aux = part[j];
                  part[j] = part[j + 1];
                  part[j + 1] = aux;
              }
          }
      }
    return part;
}

vector<Espacio> Funciones::obtenerEspacios(MBR mbr){
    Partition aux,parMBR;
    parMBR.part_size=sizeof(MBR);
    parMBR.part_start=0;
    parMBR.part_status='1';
    vector<Espacio> espacio;
    vector<Partition> part={parMBR,mbr.mbr_partition1,mbr.mbr_partition2,mbr.mbr_partition3,mbr.mbr_partition4};
    int sizeGlobal=mbr.mbr_tamano;
    int n=part.size();

    //si todas las particiones estan vacias
    if(mbr.mbr_partition1.part_status!='1' && mbr.mbr_partition2.part_status!='1' && mbr.mbr_partition3.part_status!='1'
            && mbr.mbr_partition4.part_status!='1'){
        Espacio nuevo;
        nuevo.pos=sizeof(MBR);
        nuevo.size=sizeGlobal-nuevo.pos;
        espacio.insert(espacio.begin(),nuevo);
        return espacio;
    }

    //ocultar particiones desactivadas
    for (int i = 0; i < n ; i++){
        if(part[i].part_status!='1'){
            part[i].part_size=0;
            part[i].part_start=-1;
        }
    }

    //orden ascendente

    for (int i = 0; i < n - 1; i++) {
          for (int j = 0; j < n - 1; j++) {
              if (part[j].part_start > part[j + 1].part_start) {
                  aux = part[j];
                  part[j] = part[j + 1];
                  part[j + 1] = aux;
              }
          }
      }

    //encontrar espacios
    for (int i = 0; i < n; ++i){
        if(part[i].part_size==0)
            continue;

        int posi = part[i].part_start;
        int size = part[i].part_size;
        int limit = posi + size;
        if(i==n-1){
            if(sizeGlobal <= limit)
                break;
            Espacio nuevo;
            int size2 = sizeGlobal-limit;
            nuevo.pos=limit;
            nuevo.size=size2;
            espacio.insert(espacio.end(),nuevo);
            break;
        }
        if(limit < part[i+1].part_start){
            int size2 = 0;
            int pos2=part[i+1].part_start;

            size2 = pos2-limit;
            Espacio nuevo;
            nuevo.pos=limit;
            nuevo.size=size2;
            espacio.insert(espacio.end(),nuevo);
        }

    }
    return espacio;
}



vector<Espacio> Funciones::ascendete(vector<Espacio> espacios){
    int n=espacios.size();
    Espacio aux;
    for (int i = 0; i < n - 1; i++) {
          for (int j = 0; j < n - 1; j++) {
              if (espacios[j].size > espacios[j + 1].size) {
                  aux = espacios[j];
                  espacios[j] = espacios[j + 1];
                  espacios[j + 1] = aux;
              }
          }
      }
    return espacios;
}



int  Funciones::getDskMountIndex(const char* path, MountDisk *mountDisk)
{
    for(int i=0;i<20;i++)
    {
        if(strcmp(mountDisk[i].idDisk,path) == 0)
        {
            return i;
        }
    }
    return 0;
}

MountDisk Funciones::existIdPartMount(string id,MountDisk *mountDisk){
    for(int i=0;i<20;i++)
    {
        for(int j=0;j<20;j++)
        {
            if(strcmp(mountDisk[i].mountPart[j].idMount,id.c_str()) == 0)
            {

                return mountDisk[i];
            }
        }
    }
    MountDisk mounP;
    return mounP;
}


string Funciones::getPathByID(const char*id, string *partName, MountDisk *mountDisk)
{
    for(int i=0;i<20;i++)
    {
        for(int j=0;j<20;j++)
        {
            if(strcmp(mountDisk[i].mountPart[j].idMount,id) == 0)
            {
                *partName = mountDisk[i].mountPart[j].idPart;
                return mountDisk[i].idDisk;
            }
        }
    }
    return "";
}

int Funciones::numEstructuras(int size_t){
    double_t num = (size_t-sizeof(Superbloque))/(4.0+sizeof(Journaling)+sizeof(Inodo)+3*sizeof(Barchivo));
    return floor(num);
}


