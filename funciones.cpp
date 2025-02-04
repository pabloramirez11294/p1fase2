#include "funciones.h"

Funciones::Funciones(){
    numAl=0;
}
string Funciones::convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}
StringVector Funciones::Explode(const std::string & str, char separator )
{
   StringVector  result;
   size_t pos1 = 0;
   size_t pos2 = 0;
   while ( pos2 != str.npos )
   {
      pos2 = str.find(separator, pos1);
      if ( pos2 != str.npos )
      {
         if ( pos2 > pos1 )
            result.push_back( str.substr(pos1, pos2-pos1) );
         pos1 = pos2+1;
      }
   }
   result.push_back( str.substr(pos1, str.size()-pos1) );
   return result;
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
void Funciones::continuar()
{
    __fpurge(stdin);
    printf("\t Presione una tecla para continuar:");
    getchar();
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
   // this->raid(path.c_str());
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

void Funciones::setBitMapsInicio(int n,int posI_inodo,int posI_bloque,string path){
    FILE *ptr_file;
    ptr_file = fopen(path.c_str(),"rb+");
    if(!ptr_file)
    {
        return;
    }
    char bitmap_inodos[n];
    memset(bitmap_inodos,'0',sizeof bitmap_inodos);
    char bitmap_bloques[3*n];
    memset(bitmap_bloques,'0',sizeof(bitmap_bloques));

    fseek(ptr_file,posI_inodo,SEEK_SET);
    fwrite(&bitmap_inodos,sizeof(bitmap_inodos),1,ptr_file);

    fseek(ptr_file,posI_bloque,SEEK_SET);
    fwrite(&bitmap_bloques,sizeof(bitmap_bloques),1,ptr_file);


    fclose(ptr_file);
}


void Funciones::getBitmap(int n,const char* location, int posInicial,char* bitmap)
{
    char bitmapLoc[n]; memset(bitmapLoc,0,sizeof bitmapLoc);
    FILE *ptr_file;
    ptr_file = fopen(location,"rb");
    if(!ptr_file)
    {
        strcpy(bitmap,bitmapLoc);
        return;
    }
    if(fseek(ptr_file,posInicial,SEEK_SET)!=0)
    {
        strcpy(bitmap,bitmapLoc);
        return;
    }
    fread(&bitmapLoc,n,1,ptr_file);
    rewind(ptr_file);
    fclose(ptr_file);
    strcpy(bitmap,bitmapLoc);
    return;
}

Superbloque Funciones::getSuperbloque(string path,string partName){
    Superbloque superBloque;
    struct MBR mbr = leerMBR(path.c_str());
    if(mbr.mbr_tamano==0)
        return superBloque;
    Partition *partition = get_partition_name(&mbr,partName);
    if(partition==NULL)
        return superBloque;


    FILE *ptr_file;
    ptr_file = fopen(path.c_str(),"rb");
    if(!ptr_file)
    {
        return superBloque;
    }
    if(fseek(ptr_file,partition->part_start,SEEK_SET)!=0)
    {
        return superBloque;
    }
    fread(&superBloque,sizeof(Superbloque),1,ptr_file);
    rewind(ptr_file);
    fclose(ptr_file);
    return superBloque;
}

void Funciones::setSuperbloque(string path,Superbloque superbloque,int pos){
    FILE *ptr_file;
    ptr_file = fopen(path.c_str(),"rb+");
    if(!ptr_file)
    {
        return;
    }
    fseek(ptr_file,pos,SEEK_SET);
    fwrite(&superbloque,sizeof(superbloque),1,ptr_file);
    fclose(ptr_file);
}

bool Funciones::formatearTabla_inodos_bloques(string path,Superbloque superbloque,int journStart){
    FILE *ptr_file;
    ptr_file = fopen(path.c_str(),"rb+");
    if(!ptr_file)
    {
        return false;
    }
    char val='\0';
    int count;
    int posIni = superbloque.s_inode_start;
    count = posIni;
    int posIniBloque =superbloque.s_block_start ;
    int posFin = posIniBloque + (sizeof(Barchivo)*superbloque.s_blocks_count);

    fseek(ptr_file,posIni,SEEK_SET);

    while(true){
        if(count>=posIniBloque)
            break;
        fwrite(&val,sizeof(val),sizeof(Inodo),ptr_file);
        count+=sizeof(Inodo);
    }

    while(true){
        if(count>posFin)
            break;
        fwrite(&val,sizeof(val),sizeof(Barchivo),ptr_file);
        count+=sizeof(Barchivo);
    }
    //formatear journaling

    int finJour=journStart+superbloque.s_inodes_count;
    count=journStart+sizeof(Superbloque);
    fseek(ptr_file,journStart+sizeof(Superbloque),SEEK_SET);
    while(true){
        if(count>finJour)
            break;
        fwrite(&val,sizeof(val),sizeof(Journaling),ptr_file);
        count+=sizeof(Journaling);
    }

    //colocar cantidad de journaling

    int cantidad=0;
    fseek(ptr_file,journStart+sizeof(Superbloque),SEEK_SET);
    fwrite(&cantidad,sizeof(int),1,ptr_file);

    fclose(ptr_file);
    return true;
}

bool Funciones::ingresarInodo(string path, Inodo inodo, int pos, int num){
    FILE *ptr_file;
    ptr_file = fopen(path.c_str(),"rb+");
    if(!ptr_file)
    {
        return false;
    }
    pos+= num*sizeof(Inodo);
    fseek(ptr_file,pos,SEEK_SET);
    fwrite(&inodo,sizeof(inodo),1,ptr_file);
    fclose(ptr_file);

    return true;
}

bool Funciones::ingresar_bitmap(string path, int pos, int num){
    FILE *ptr_file;
    ptr_file = fopen(path.c_str(),"rb+");
    if(!ptr_file)
    {
        return false;
    }
    char val='1';
    pos+=num;
    fseek(ptr_file,pos,SEEK_SET);
    fwrite(&val,sizeof(val),1,ptr_file);
    fclose(ptr_file);
    return true;
}


bool Funciones::ingresarBloqueCarpeta(string path, Bcarpeta carpeta, int pos, int num){
    FILE *ptr_file;
    ptr_file = fopen(path.c_str(),"rb+");
    if(!ptr_file)
    {
        return false;
    }
    pos+= num*sizeof(Bcarpeta);
    fseek(ptr_file,pos,SEEK_SET);
    fwrite(&carpeta,sizeof(carpeta),1,ptr_file);
    fclose(ptr_file);
    return true;
}

bool Funciones::ingresarBloqueArchivo(string path, Barchivo carpeta, int pos, int num){
    FILE *ptr_file;
    ptr_file = fopen(path.c_str(),"rb+");
    if(!ptr_file)
    {
        return false;
    }
    pos+= num*sizeof(Barchivo);
    fseek(ptr_file,pos,SEEK_SET);
    fwrite(&carpeta,sizeof(carpeta),1,ptr_file);
    fclose(ptr_file);
    return true;
}

Bcarpeta Funciones::getBcarpeta(string path,int pos, int num){
    Bcarpeta carpeta;
    FILE *ptr_file;
    ptr_file = fopen(path.c_str(),"rb");
    if(!ptr_file)
    {
        return carpeta;
    }
    pos+= num*sizeof(Bcarpeta);

    if(fseek(ptr_file,pos,SEEK_SET)!=0)
    {
        return carpeta;
    }
    fread(&carpeta,sizeof(Bcarpeta),1,ptr_file);
    fclose(ptr_file);
    return carpeta;
}
Barchivo Funciones::getBarchivo(string path,int pos, int num){
    Barchivo carpeta;
    FILE *ptr_file;
    ptr_file = fopen(path.c_str(),"rb+");
    if(!ptr_file)
    {
        return carpeta;
    }
    pos+= num*sizeof(Barchivo);
    fseek(ptr_file,pos,SEEK_SET);
    fread(&carpeta,sizeof(Barchivo),1,ptr_file);
    fclose(ptr_file);
    return carpeta;
}
Bapuntador Funciones::getBapuntador(string path,int pos, int num){
    Bapuntador carpeta;
    FILE *ptr_file;
    ptr_file = fopen(path.c_str(),"rb+");
    if(!ptr_file)
    {
        return carpeta;
    }
    pos+= num*sizeof(Bapuntador);
    fseek(ptr_file,pos,SEEK_SET);
    fread(&carpeta,sizeof(Bapuntador),1,ptr_file);
    fclose(ptr_file);
    return carpeta;
}
Inodo Funciones::getInode(string path,int pos, int num){
    Inodo inodo;
    FILE *ptr_file;
    ptr_file = fopen(path.c_str(),"rb+");
    if(!ptr_file)
    {
        return inodo;
    }
    pos+= num*sizeof(Inodo);
    fseek(ptr_file,pos,SEEK_SET);
    fread(&inodo,sizeof(Inodo),1,ptr_file);
    fclose(ptr_file);
    return inodo;
}

bool Funciones::setBloque(Inodo inodo,string txt,Superbloque superbloque,string path,int num_inodo){
    int aux=-1;
    for(int i=0;i<15;i++){
        if(inodo.i_block[i]==-1){
            if(i==0)
                break;
            aux=i-1;
            break;
        }
    }
    Barchivo archivo;

        archivo=getBarchivo(path,superbloque.s_block_start,inodo.i_block[aux]);


    int total=(inodo.i_size-64*aux)+txt.size();
    if(total<=64){
        strcat(archivo.b_content,txt.c_str());
        if(!(ingresarBloqueArchivo(path,archivo,superbloque.s_block_start,inodo.i_block[aux]) )){
            return false;
        }
    }else{
        int b_libre=bloqueLibre(path,superbloque);
        if(b_libre==-1){
            cout<<"Error, no hay bloque libres."<<endl;
            return false;
        }
        inodo.i_block[aux+1]=b_libre;
        Barchivo archivo2;
        int residuo=total-64;
        int tamano=txt.size()-residuo;        
        char ch2[residuo];memset(ch2,0,sizeof ch2+1);
        txt.copy(ch2,residuo,tamano);

        strcpy(archivo2.b_content,ch2);
        if(!(ingresarBloqueArchivo(path,archivo2,superbloque.s_block_start,inodo.i_block[aux+1]) && ingresar_bitmap(path,superbloque.s_bm_block_start,inodo.i_block[aux+1]))){
            return false;
        }
        if(tamano!=0){
            char ch[tamano];memset(ch,0,sizeof ch+1);
            txt.copy(ch,tamano,0);
            strcat(archivo.b_content,ch);
            if(!(ingresarBloqueArchivo(path,archivo,superbloque.s_block_start,inodo.i_block[aux]) )){
                return false;
            }
        }



    }
    inodo.i_size+=txt.size();
    if(!(ingresarInodo(path,inodo,superbloque.s_inode_start,num_inodo))){
        cout<<"ERROR!, setBloque, no se pudo ingresar el inodo: "<<endl;
        return false;
    }
    return true;
}


int Funciones::bloqueLibre(string path, Superbloque superbloque){
    int n=superbloque.s_blocks_count;
    char bitmap[n]; memset(bitmap,0,sizeof bitmap);
    getBitmap(n,path.c_str(),superbloque.s_bm_block_start,bitmap);
    for(int i=0;i<n;i++){
        if(bitmap[i]=='0'){
            return i;
        }
    }
    return -1;
}
int Funciones::inodoLibre(string path, Superbloque superbloque){
    int n=superbloque.s_inodes_count;
    char bitmap[n]; memset(bitmap,0,sizeof bitmap);
    getBitmap(n,path.c_str(),superbloque.s_bm_inode_start,bitmap);
    for(int i=0;i<n;i++){
        if(bitmap[i]=='0'){
            return i;
        }
    }
    return -1;
}

string Funciones::leerArchivo(string path, Superbloque superbloque,string ruta){
    int num_i=buscarInodoRaiz(path,superbloque,ruta);
    if(num_i==-1)
        return "";
    Inodo inodo = getInode(path,superbloque.s_inode_start,num_i);
    string txt="";
    for(int i=0;i<15;i++){
        int numB=inodo.i_block[i];
        if(numB==-1){
            break;
        }
        Barchivo archivo=getBarchivo(path,superbloque.s_block_start,numB);
        txt.append(archivo.b_content,0,64);
    }
    return txt;
}

int Funciones::buscarInodoRaiz(string path, Superbloque superbloque,string ruta){
    Inodo raiz = getInode(path,superbloque.s_inode_start,0);
    StringVector l1=Explode(ruta,'/');
    string nombre = l1.at(l1.size()-1);
    for(int i=0;i<15;i++){
        if(raiz.i_block[i]!=-1){
            Bcarpeta carpeta = getBcarpeta(path,superbloque.s_block_start,raiz.i_block[i]);
            for(int y=0;y<4;y++){                
                Content cont = carpeta.b_content[y];
                if(cont.b_inodo==-1)
                    continue;
                if(nombre.compare(cont.name)==0){
                    return cont.b_inodo;
                }
            }
        }
    }
    return -1;
}
int Funciones::buscarInodoCarpeta(string path,string nombre, Superbloque superbloque,Inodo raiz){

    for(int i=0;i<15;i++){
        if(raiz.i_block[i]!=-1){
            Bcarpeta capeta = getBcarpeta(path,superbloque.s_block_start,raiz.i_block[i]);
            for(int y=0;y<4;y++){
                Content cont = capeta.b_content[y];
                if(nombre.compare(cont.name)==0){
                    return cont.b_inodo;
                }
            }
        }
    }
    return -1;
}

bool Funciones::Fmkdir(Inodo inodo,string dir, string path,Superbloque superbloque){
    StringVector l1=Explode(dir,'/');
    string nombre = l1.at(l1.size()-1);
    l1.erase(l1.end());
    Inodo inodoAux = this->getInode(path,superbloque.s_inode_start,0);
    int numInodo=0;
    for(unsigned int i=0;i<l1.size();i++){
        string nombreAux=l1.at(i);
        numInodo=buscarInodoCarpeta(path,nombreAux,superbloque,inodoAux);
        if(numInodo==-1){
            return false;
        }
        inodoAux= this->getInode(path,superbloque.s_inode_start,numInodo);
        if(inodoAux.i_type==1){
            return false;
        }
    }
    if(numInodo==-1){
        return false;
    }
    //ingresar numinodo e inodo
    int numInodoNuevo=this->inodoLibre(path,superbloque);
    if(numInodoNuevo==-1){
        return false;
    }

    for(int i=0;i<15;i++){
        if(inodoAux.i_block[i]!=-1){
            Bcarpeta carpeta=this->getBcarpeta(path,superbloque.s_block_start,inodoAux.i_block[i]);
            for(int y=0;y<4;y++){
                Content *cont = &carpeta.b_content[y];
                if(cont->b_inodo==-1){
                    cont->b_inodo=numInodoNuevo;
                    strcpy(cont->name,nombre.c_str());
                    this->ingresarBloqueCarpeta(path,carpeta,superbloque.s_block_start,inodoAux.i_block[i]);
                    goto forext;
                }
            }

        }else{
            Bcarpeta carpeta;
            for(int i=0;i<4;i++){
                carpeta.b_content[i].b_inodo=-1;
                strcpy(carpeta.b_content[i].name,"-");
            }
            int numCarpeta=this->bloqueLibre(path,superbloque);
            carpeta.b_content[0].b_inodo=numInodoNuevo;
            strcpy(carpeta.b_content[0].name,nombre.c_str());
            if(!(ingresarBloqueCarpeta(path,carpeta,superbloque.s_block_start,numCarpeta) && ingresar_bitmap(path,superbloque.s_bm_block_start,numCarpeta))){
                return false;
            }
            inodoAux.i_block[i]=numCarpeta;
            break;
        }
        if(i==14){
            return false;
        }
    }
    forext:
    if(!(ingresarInodo(path,inodoAux,superbloque.s_inode_start,numInodo))){
        return false;
    }

    int numCar=bloqueLibre(path,superbloque);
    Bcarpeta c_raiz;
    strcpy(c_raiz.b_content[0].name,".");
    c_raiz.b_content[0].b_inodo=numInodoNuevo;

    strcpy(c_raiz.b_content[1].name,"..");
    c_raiz.b_content[1].b_inodo=numInodo;

    strcpy(c_raiz.b_content[2].name,"-");
    c_raiz.b_content[2].b_inodo=-1;

    strcpy(c_raiz.b_content[3].name,"-");
    c_raiz.b_content[3].b_inodo=-1;


    if(!(ingresarBloqueCarpeta(path,c_raiz,superbloque.s_block_start,numCar) && ingresar_bitmap(path,superbloque.s_bm_block_start,numCar))){
        return false;
    }
    inodo.i_block[0]=numCar;
    if(!(ingresarInodo(path,inodo,superbloque.s_inode_start,numInodoNuevo) && ingresar_bitmap(path,superbloque.s_bm_inode_start,numInodoNuevo))){
        return false;
    }
    return true;

}


string Funciones::leerArchivoFisico(string path){
    string cadena="";
    string txt="";
    ifstream datos(path);
    if(datos.fail()){
        cout<<"archivo no existe"<<endl;
        return "";
    }else{
        while(!datos.eof()){
            getline(datos,cadena);
            txt+=cadena+"\n";
        }
    }
    datos.close();
    txt.erase(txt.end()-1);
    txt.erase(txt.end()-1);
    return txt;
}


bool Funciones::Fmkfile(Inodo inodo, string dir, string path, Superbloque superbloque){
    StringVector l1=Explode(dir,'/');
    string nombre = l1.at(l1.size()-1);
    l1.erase(l1.end());
    Inodo inodoAux = this->getInode(path,superbloque.s_inode_start,0);
    int numInodo=0;
    for(unsigned int i=0;i<l1.size();i++){
        string nombreAux=l1.at(i);
        numInodo=buscarInodoCarpeta(path,nombreAux,superbloque,inodoAux);
        if(numInodo==-1){
            return false;
        }
        inodoAux= this->getInode(path,superbloque.s_inode_start,numInodo);
        if(inodoAux.i_type==1){
            return false;
        }
    }
    if(numInodo==-1){
        return false;
    }

    //ingresar numinodo e inodo
    int numInodoNuevo=this->inodoLibre(path,superbloque);
    if(numInodoNuevo==-1){
        return false;
    }

    for(int i=0;i<15;i++){
        if(inodoAux.i_block[i]!=-1){
            Bcarpeta carpeta=this->getBcarpeta(path,superbloque.s_block_start,inodoAux.i_block[i]);
            for(int y=0;y<4;y++){
                Content *cont = &carpeta.b_content[y];
                if(cont->b_inodo==-1){
                    cont->b_inodo=numInodoNuevo;
                    strcpy(cont->name,nombre.c_str());
                    this->ingresarBloqueCarpeta(path,carpeta,superbloque.s_block_start,inodoAux.i_block[i]);
                    goto forext;
                }
            }

        }else{
            Bcarpeta carpeta;
            for(int i=0;i<4;i++){
                carpeta.b_content[i].b_inodo=-1;
                strcpy(carpeta.b_content[i].name,"-");
            }
            int numCarpeta=this->bloqueLibre(path,superbloque);
            carpeta.b_content[0].b_inodo=numInodoNuevo;
            strcpy(carpeta.b_content[0].name,nombre.c_str());
            if(!(ingresarBloqueCarpeta(path,carpeta,superbloque.s_block_start,numCarpeta) && ingresar_bitmap(path,superbloque.s_bm_block_start,numCarpeta))){
                return false;
            }
            inodoAux.i_block[i]=numCarpeta;
            break;
        }
        if(i==14){
            return false;
        }
    }
    forext:
    //actualizar inodoPadre
    if(!(ingresarInodo(path,inodoAux,superbloque.s_inode_start,numInodo))){
        return false;
    }

    //ingresar inodoArchivo
    if(!(ingresarInodo(path,inodo,superbloque.s_inode_start,numInodoNuevo) && ingresar_bitmap(path,superbloque.s_bm_inode_start,numInodoNuevo))){
        return false;
    }
    return true;
}


bool Funciones::Fcat(string dir, string path, Superbloque superbloque,string *text){
    StringVector l1=Explode(dir,'/');
    string nombre = l1.at(l1.size()-1);
    l1.erase(l1.end());
    Inodo inodoAux = this->getInode(path,superbloque.s_inode_start,0);
    int numInodo=0;
    for(unsigned int i=0;i<l1.size();i++){
        string nombreAux=l1.at(i);
        numInodo=buscarInodoCarpeta(path,nombreAux,superbloque,inodoAux);
        if(numInodo==-1){
            return false;
        }
        inodoAux= this->getInode(path,superbloque.s_inode_start,numInodo);
        if(inodoAux.i_type==1){
            return false;
        }
    }
    if(numInodo==-1){
        return false;
    }
    //buscar en el inodo padre
    int numInodoArchivo=-1;
    for(int i=0;i<15;i++){
        if(inodoAux.i_block[i]!=-1){
            Bcarpeta carpeta=this->getBcarpeta(path,superbloque.s_block_start,inodoAux.i_block[i]);
            for(int y=0;y<4;y++){
                Content *cont = &carpeta.b_content[y];
                if(cont->b_inodo!=-1 && nombre.compare(cont->name)==0){
                    numInodoArchivo=cont->b_inodo;
                    goto forext;
                }
            }

        }
        if(i==14){
            return false;
        }
    }
    forext:
    if(numInodoArchivo==-1)
        return false;
    *text=this->leerArchivoCat(path,superbloque,numInodoArchivo);

    return true;
}

string Funciones::leerArchivoCat(string path, Superbloque superbloque, int numInodo){
    Inodo inodo = getInode(path,superbloque.s_inode_start,numInodo);
    string txt="";
    for(int i=0;i<15;i++){
        int numB=inodo.i_block[i];
        if(numB==-1){
            break;
        }
        Barchivo archivo=getBarchivo(path,superbloque.s_block_start,numB);
        txt.append(archivo.b_content,0,64);
    }
    return txt;
}


void Funciones::Fjournaling(string path, string log,int pos){
    Journaling jour;
    int cantidad=0;
    strcpy(jour.cadena,log.c_str());
    FILE *ptr_file;
    ptr_file = fopen(path.c_str(),"rb+");
    if(!ptr_file)
    {
        return;
    }
    fseek(ptr_file,pos+sizeof(Superbloque),SEEK_SET);
    fread(&cantidad,sizeof (int),1,ptr_file);

    fseek(ptr_file,sizeof(Journaling)*cantidad,SEEK_CUR);
    fwrite(&jour,sizeof(Journaling),1,ptr_file);

    cantidad++;
    fseek(ptr_file,pos+sizeof(Superbloque),SEEK_SET);
    fwrite(&cantidad,sizeof(int),1,ptr_file);
    fclose(ptr_file);
}

string Funciones::Frecovery(string path, int pos){
    Journaling jour;
    int cantidad=0;
    FILE *ptr_file;
    ptr_file = fopen(path.c_str(),"rb+");
    if(!ptr_file)
    {
        return "";
    }
    fseek(ptr_file,pos+sizeof(Superbloque),SEEK_SET);
    fread(&cantidad,sizeof (int),1,ptr_file);

    string texto="";
    int contador=0;
    while(!feof(ptr_file)){
        if(contador==cantidad)
            break;
        fread(&jour, sizeof(jour), 1, ptr_file);
        texto.append(jour.cadena);
        texto.append("\n");
        contador++;
    }
    fclose(ptr_file);
    return texto;
}

bool Funciones::Floss(string path,Superbloque superbloque){
    FILE *ptr_file;
    ptr_file = fopen(path.c_str(),"rb+");
    if(!ptr_file)
    {
        return false;
    }
    char val='\0';
    int count;
    int posIni = superbloque.s_inode_start;
    count = posIni;
    int posIniBloque =superbloque.s_block_start ;
    int posFin = posIniBloque + (sizeof(Barchivo)*superbloque.s_blocks_count);

    fseek(ptr_file,posIni,SEEK_SET);

    while(true){
        if(count>=posIniBloque)
            break;
        fwrite(&val,sizeof(val),sizeof(Inodo),ptr_file);
        count+=sizeof(Inodo);
    }

    while(true){
        if(count>posFin)
            break;
        fwrite(&val,sizeof(val),sizeof(Barchivo),ptr_file);
        count+=sizeof(Barchivo);
    }

    fclose(ptr_file);
    return true;
}
