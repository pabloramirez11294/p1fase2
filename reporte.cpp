#include "reporte.h"

void Reporte::RepMbr(const char* path,const char* pathRep)
{
    char lDot[1000000];memset(lDot,0,sizeof lDot);

    struct MBR mbr = funciones.leerMBR(path);
    if(mbr.mbr_tamano==0){
        cout<<"ERROR!,reporte mbr, el disco no existe. "<<path<<endl;
        return;
    }
    time_t result = mbr.mbr_fecha_creacion;
    char sublDot[1000];
    memset(sublDot,0,sizeof(sublDot));
    strcpy(lDot,"digraph d{\nnode [shape=plaintext]\nstruct0 [label=<<TABLE>");
    strcat(lDot,"\n<TR><TD>Nombre</TD><TD>Valor</TD></TR>");
    strcat(lDot,"\n<TR><TD>mbr_tamano</TD><TD>");
    sprintf(sublDot,"%d",mbr.mbr_tamano);strcat(lDot,sublDot);
    strcat(lDot,"</TD></TR>");
    strcat(lDot,"\n<TR><TD>mbr_fecha_creacion</TD><TD>");
    strcat(lDot,asctime(gmtime(&result)));strcat(lDot,"</TD></TR>");
    strcat(lDot,"\n<TR><TD>mbr_disk_signature</TD><TD>");
    sprintf(sublDot,"%d",mbr.mbr_disk_signature);strcat(lDot,sublDot);
    strcat(lDot,"</TD></TR>");

    if(mbr.mbr_partition1.part_size > 0){
        char Num[10];memset(Num,0,sizeof(Num));
        strcpy(Num,"1");
        Partition *paritition = &mbr.mbr_partition1;
        strcat(lDot,"\n<TR><TD>part_status_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%c",paritition->part_status);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_type_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%c",paritition->part_type);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_fit_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%c",paritition->part_fit);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_start_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%d",paritition->part_start);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_size_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%d",paritition->part_size);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_name_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        strcat(lDot,paritition->part_name);
        strcat(lDot,"</TD></TR>");
    }
    if(mbr.mbr_partition2.part_size > 0){
        char Num[10];memset(Num,0,sizeof(Num));
        strcpy(Num,"2");
        Partition *paritition = &mbr.mbr_partition2;
        strcat(lDot,"\n<TR><TD>part_status_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%c",paritition->part_status);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_type_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%c",paritition->part_type);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_fit_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%c",paritition->part_fit);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_start_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%d",paritition->part_start);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_size_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%d",paritition->part_size);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_name_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        strcat(lDot,paritition->part_name);
        strcat(lDot,"</TD></TR>");
    }
    if(mbr.mbr_partition3.part_size > 0){
        char Num[10];memset(Num,0,sizeof(Num));
        strcpy(Num,"3");
        Partition *paritition = &mbr.mbr_partition3;
        strcat(lDot,"\n<TR><TD>part_status_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%c",paritition->part_status);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_type_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%c",paritition->part_type);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_fit_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%c",paritition->part_fit);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_start_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%d",paritition->part_start);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_size_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%d",paritition->part_size);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_name_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        strcat(lDot,paritition->part_name);
        strcat(lDot,"</TD></TR>");
    }
    if(mbr.mbr_partition4.part_size > 0){
        char Num[10];memset(Num,0,sizeof(Num));
        strcpy(Num,"4");
        Partition *paritition = &mbr.mbr_partition4;
        strcat(lDot,"\n<TR><TD>part_status_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%c",paritition->part_status);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_type_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%c",paritition->part_type);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_fit_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%c",paritition->part_fit);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_start_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%d",paritition->part_start);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_size_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        sprintf(sublDot,"%d",paritition->part_size);
        strcat(lDot,sublDot);
        strcat(lDot,"</TD></TR>");
        strcat(lDot,"\n<TR><TD>part_name_");
        strcat(lDot,Num);
        strcat(lDot,"</TD><TD>");
        strcat(lDot,paritition->part_name);
        strcat(lDot,"</TD></TR>");
    }
    strcat(lDot,"\n</TABLE>>];");



    strcat(lDot,"}");
    GenerarIMGDOT(lDot,"mbr",pathRep);

}

void Reporte::RepDisk(const char* path, const char *pathRep){


    struct MBR mbr = funciones.leerMBR(path);
    if(mbr.mbr_tamano==0){
        cout<<"ERROR!,reporte disk, el disco no existe. "<<path<<endl;
        return;
    }
    string txt;
    txt="digraph d{node [shape=plaintext] struct0 [label=";
    txt+="<<TABLE>";
    txt+="\n<TR>\n";



    vector<Partition> vec = funciones.ordenarParticiones(mbr);
    string tipo="";
    string nombre="";
    double div =0;
    int porc =0;
    int limit=0;
    double sizeGlobal=mbr.mbr_tamano;
    for(int i=0;i<vec.size();i++){
        if(vec[i].part_size==0)
            continue;

        if(limit<vec[i].part_start){
            int vacio = vec[i].part_start-limit;
            div = vacio/sizeGlobal;
            porc = div * 100;
            txt+="<td> Libre <br></br><br></br>"+to_string(porc)+"% del disco</td>\n";
        }
        tipo=vec[i].part_type;
        if(tipo.compare("M")==0){
            nombre="MBR";
        }else if(tipo.compare("P")==0){
            nombre="Primaria";
        }else if(tipo.compare("E")==0){
            nombre="Extendida";
        }
        div = vec[i].part_size/sizeGlobal;
        porc =(int)( div * 100);
        txt+="<td>"+nombre+"<br></br><br></br> "+to_string(porc)+"% del disco</td>\n";
        limit=vec[i].part_start+vec[i].part_size;

        if(i+1==vec.size() && limit<mbr.mbr_tamano){
            int vacio = mbr.mbr_tamano-limit;
            div = vacio/sizeGlobal;
            porc = div * 100;
            txt+="<td>Libre <br></br><br></br>"+to_string(porc)+"% del disco</td>\n";
        }
    }

    txt+="\n</TR>\n";



    txt+="</TABLE>>];}";
    GenerarIMGDOT(txt.c_str(),"reporte2",pathRep);

}


void Reporte::RepBm(const char* path, const char *pathRep,string id,MountDisk mountDisk[],bool esInodo)
{

    string partName;
    string pathAux=funciones.getPathByID(id.c_str(),&partName,mountDisk);
    if(pathAux.compare("")==0){
        cout<<"ERROR!, reporte bitmap, no encuentra el disco asociado al id: "<<id<<endl;
        return;
    }
    MBR mbr = funciones.leerMBR(path);
    if(mbr.mbr_tamano==0){
        cout<<"ERROR!, reporte bitmap, no encontro el disco: "<<path<<endl;
        return;
    }
    Partition* part=funciones.get_partition_name(&mbr,partName);
    if(part==NULL){
        cout<<"ERROR!, reporte bitmap, no encontro la particion solicitada: "<<partName<<endl;
        return;
    }
    int n = funciones.numEstructuras(part->part_size);

    if(!esInodo)
        n=3*n;

    char bitmap[n]; memset(bitmap,0,sizeof bitmap);

    Superbloque superbloque = funciones.getSuperbloque(path,partName);
    if(superbloque.s_magic!=0xEF53){
        cout<<"ERROR!, reporte bitmap, no encontro el superbloque de la particion: "<<partName<<endl;
        return;
    }
    if(esInodo)
        funciones.getBitmap(n,path,superbloque.s_bm_inode_start,bitmap);
    else
        funciones.getBitmap(n,path,superbloque.s_bm_block_start,bitmap);

    ofstream file;
    file.open(pathRep);
    for(int i=0;i<n;){
        for(int j=0;j<20;j++){
            if(i>=n)
                break;
            file<<bitmap[i]<<" ";
            i++;
        }
        file<<"\n";
    }



}
void Reporte::RepBlock(const char* path, const char *pathRep, string id, MountDisk mountDisk[]){
    string partName;
    string pathAux=funciones.getPathByID(id.c_str(),&partName,mountDisk);
    if(pathAux.compare("")==0){
        cout<<"ERROR!, reporte block, no encuentra el disco asociado al id: "<<id<<endl;
        return;
    }
    MBR mbr = funciones.leerMBR(path);
    if(mbr.mbr_tamano==0){
        cout<<"ERROR!, reporte block, no encontro el disco: "<<path<<endl;
        return;
    }
    Partition* part=funciones.get_partition_name(&mbr,partName);
    if(part==NULL){
        cout<<"ERROR!, reporte block, no encontro la particion solicitada: "<<partName<<endl;
        return;
    }
    Superbloque superbloque = funciones.getSuperbloque(path,partName);
    if(superbloque.s_magic!=0xEF53){
        cout<<"ERROR!, reporte bitmap, no encontro el superbloque de la particion: "<<partName<<endl;
        return;
    }
    int cont=0;
    string text="digraph G { \n rankdir=LR \nnode [shape=rectangle]\n";
    string aux="";
    string aux2="";
    int n = superbloque.s_inodes_count;
    char bitmap[n]; memset(bitmap,0,sizeof bitmap);
    funciones.getBitmap(n,path,superbloque.s_bm_inode_start,bitmap);
    for(int i=0;i<n;i++){
        if(bitmap[i]=='1'){
            aux2=aux;
            Inodo inodo = funciones.getInode(path,superbloque.s_inode_start,i);
            if(inodo.i_type==0){
                aux2+=inodoCarpeta(path,inodo,superbloque.s_block_start,&text,&cont);
            }else{
                aux2+=inodoArchivo(path,inodo,superbloque.s_block_start,&text,&cont);
            }

            aux+=aux2+" -> ";

        }
    }
    text+=aux2;
    text+="}";

    GenerarIMGDOT(text.c_str(),"reporte2",pathRep);

}
void Reporte::RepInode(const char* path, const char *pathRep, string id, MountDisk mountDisk[]){
    string partName;
    string pathAux=funciones.getPathByID(id.c_str(),&partName,mountDisk);
    if(pathAux.compare("")==0){
        cout<<"ERROR!, reporte block, no encuentra el disco asociado al id: "<<id<<endl;
        return;
    }
    MBR mbr = funciones.leerMBR(path);
    if(mbr.mbr_tamano==0){
        cout<<"ERROR!, reporte block, no encontro el disco: "<<path<<endl;
        return;
    }
    Partition* part=funciones.get_partition_name(&mbr,partName);
    if(part==NULL){
        cout<<"ERROR!, reporte block, no encontro la particion solicitada: "<<partName<<endl;
        return;
    }
    Superbloque superbloque = funciones.getSuperbloque(path,partName);
    if(superbloque.s_magic!=0xEF53){
        cout<<"ERROR!, reporte bitmap, no encontro el superbloque de la particion: "<<partName<<endl;
        return;
    }
    string text="digraph G { \n rankdir=LR \nnode [shape=rectangle]\n";
    string aux="";
    string aux2="";
    int n = superbloque.s_inodes_count;
    char bitmap[n]; memset(bitmap,0,sizeof bitmap);
    funciones.getBitmap(n,path,superbloque.s_bm_inode_start,bitmap);
    for(int i=0;i<n;i++){
        if(bitmap[i]=='1'){
            aux2=aux+"n"+to_string(i);
            aux+=aux2+" -> ";
            text+="n"+to_string(i)+" [label=\"Inodo "+to_string(i)+"&#92;n";
            Inodo inodo = funciones.getInode(path,superbloque.s_inode_start,i);
            text+="Tipo: "+to_string(inodo.i_type)+"\"] \n";
        }
    }
    text+=aux2;
    text+="}";

    GenerarIMGDOT(text.c_str(),"reporte2",pathRep);
}

void Reporte::GenerarIMGDOT(const char *Lenguaje, const char *Ruta_Arcivo, const char *Ruta_Img)
{
    char arch[300] = "/home/pablo/dots/";
    funciones.createDirs(Ruta_Img);
    strcat(arch,Ruta_Arcivo);
    strcat(arch,".dot");
    funciones.createDirs(arch);
    std::fstream archivo;
    archivo.open(arch,std::ios::out);
    if(archivo.is_open())
    {
        archivo << Lenguaje;
        archivo.close();
    }


    char graph[200] = "dot -Tpng ";
    strcat(graph, arch);
    strcat(graph," -o ");
    strcat(graph, Ruta_Img);
    printf("%s\n",graph);
    system(graph);

    char graphimg[200] = "xdg-open  ";
    strcat(graphimg,Ruta_Img);
    system(graphimg);
}

string Reporte::inodoCarpeta(string path,Inodo inodo,int pos,string *text,int *cont){
    string aux="";
    string aux2="";
    for(int i=0;i<15;i++){
        if(inodo.i_block[i]==-1)
            continue;
        aux2=aux+"n"+to_string(*cont);
        aux+=aux2+" -> ";
        int num= inodo.i_block[i];
        Bcarpeta carpeta= funciones.getBcarpeta(path,pos,num);
        *text+="n"+to_string(*cont)+" [label=\"Bloque Carpeta"+to_string(num)+"&#92;n";
        for(int y=0;y<4;y++){
            string nombre(carpeta.b_content[y].name);
            string num_inodo=to_string(carpeta.b_content[y].b_inodo);
            *text+=nombre+"   "+num_inodo+"&#92;n";
        }
        *text+="\"]";
        (*cont)++;

    }
    return aux2;
}

string Reporte::inodoArchivo(string path,Inodo inodo,int pos,string *text,int *cont){
    string aux="";
    string aux2="";
    for(int i=0;i<15;i++){
        if(inodo.i_block[i]==-1)
            break;
        aux2=aux+"n"+to_string(*cont);
        aux+=aux2+" -> ";
        int num= inodo.i_block[i];
        Barchivo archivo= funciones.getBarchivo(path,pos,num);
        *text+="n"+to_string(*cont)+" [label=\"Bloque Archivo"+to_string(num)+"&#92;n";
        string contenido(archivo.b_content);
        *text+=contenido+"\"]";
        (*cont)++;

    }
    return aux2;
}
