#include "comando.h"

void Comando::Mkdisk(string path, int sizeB, char unit, string fit)
{
    char fit_aux=0;
    if(fit=="bf"){
        fit_aux='B';
    }else if(fit=="wf"){
        fit_aux='W';
    }else{
        fit_aux='F';
    }
    switch(unit)
    {
    case'K':
    case'k':
        sizeB*=1024;
        break;
    case'M':
    case'm':
    case 0:
        sizeB*=1024*1024;
        break;
    default:
        printf("ERROR! Unidad de medida\n");
        return;
    }

    MBR mbr=funciones.getNewMBR();
    mbr.mbr_tamano = sizeB;
    mbr.mbr_disk_signature =funciones.numAleatorio();
    mbr.mbr_fecha_creacion = time(NULL);
    mbr.disk_fit=fit_aux;

    //remover el archivo anterior
    remove(path.c_str());

    FILE *ptr_file;
    ptr_file = fopen(path.c_str(),"wb");
    if(!ptr_file)
    {
        funciones.createDirs(path.c_str());
        ptr_file = fopen(path.c_str(),"wb");
        if(!ptr_file)
        {
            cout<<"ERROR mkdisk, Abrir disco: "<<path<<endl;
            return;
        }
    }
    int fin=(sizeB/1024);
    char buffer[1024];
    for(int i=0;i<1024;i++)
    {
        buffer[i]='\0';
    }
    int j=0;
    while(j!=fin){
        fwrite(&buffer,1024 , 1, ptr_file);
        j++;
    }
    fclose(ptr_file);

    if(funciones.escribirMBR(path,mbr)){
        cout<<"MKDISK,  Disco creado: "<<path<<endl;
    }else{
        cout<<"ERROR! MKDISK, no se pudo crear el disco "<<path<<endl;
    }
}

void  Comando::Rmdisk(string path)
{
    char yes[1]="";
    cout<<"[Y/N]: ";
    cin>>yes;
    if((strcmp(yes,"Y") != 0) &&
        (strcmp(yes,"y") != 0))
        return;

    if(remove(path.c_str())==-1)
    {
        cout<<"ERROR!,rmdisk, No existe Disco: "<<path<<endl;
        return;
    }
    cout<<"RMDISk: Disco borrado: "<<path<<endl;
}


void Comando::Fdisk(string path, string name, int sizeB, char unit, char type, char fit){
    MBR mbr = funciones.leerMBR(path.c_str());
    if(mbr.mbr_tamano==0){
        cout<<"ERROR!,fdisk, el disco no existe. "<<path<<endl;
        return;
    }
    int numPart = funciones.existPartition(mbr,name);

    //verificar parametros
    if(numPart!=0){
        cout<<"ERROR!,fdisk name, ya existe una particion con el mismo nombre: "<<name<<endl;
        return;
    }
    switch(unit)
    {
    case'b':
        break;
    case'k':
    case 0:
        sizeB*=1024;
        break;
    case'm':
        sizeB*=1024*1024;
        break;
    default:
        printf("ERROR!,fdisk unit, Unidad de medida no reconocida.\n");
        return;
    }
    switch(fit)
    {
    case'b':
        fit = 'B';
        break;
    case 'f':
        fit = 'F';
        break;
    case 'w':
    case 0:
        fit = 'W';
        break;
    default:
        printf("ERROR!, fdisk fit, ajuste no reconocido.\n");
        return;
    }

    switch(type)
    {
    case 'P':
    case'p':
    case 0:
        type = 'P';
        break;
    case 'E':
    case 'e':
        type = 'E';
        break;
    case 'L':
    case 'l':

        break;
    default:
        printf( "ERROR!, fdisk type, Tipo no existente.\n");
        return;
    }

    //comprobaciones
    if(type=='L'){
        printf( "particion logica\n");
        return;
    }else if(type=='E' && funciones.existExtendida(mbr)){
            cout<<"ERROR!,fdisk , ya existe una particion extendida: "<<name<<endl;
            return;
    }
    Partition *actual = funciones.partitionDisponible(&mbr);
    if(actual==NULL){
        cout<<"ERROR!,fdisk , ya estan utilizadas las 4 particiones primarias."<<name<<endl;
        return;
    }
    vector<Espacio> espacios = funciones.obtenerEspacios(mbr);
    int start=0;
    if(mbr.disk_fit=='F'){
        for(unsigned int i=0;i<espacios.size();i++){
            if(sizeB<=espacios[i].size){
                start=espacios[i].pos;
                break;
            }
        }
    }else if(mbr.disk_fit=='B'){
        espacios=funciones.ascendete(espacios);
        for(unsigned int i=0;i<espacios.size();i++){
            if(sizeB<=espacios[i].size){
                start=espacios[i].pos;
                break;
            }
        }
    }else if(mbr.disk_fit=='W'){
        espacios=funciones.ascendete(espacios);
        for(int i=espacios.size();i>0;i--){
            if(sizeB<=espacios[i-1].size){
                start=espacios[i-1].pos;
                break;
            }
        }
    }
    if(start==0){
        cout<<"ERROR!,fdisk , no hay espacio para crear la particion: "<<name<<endl;
        return;
    }


    actual->part_fit=fit;
    strcpy(actual->part_name,name.c_str());
    actual->part_size=sizeB;
    actual->part_start=start;
    actual->part_status='1';
    actual->part_type=type;


    if(funciones.escribirMBR(path,mbr)){
        cout<<"FDISK, se creo la particion: "<<name<<endl;
    }else{
        cout<<"ERROR! FDISK, no se crear la particion: "<<name<<endl;
    }

}


void Comando::FdiskDelete(string path, string name, string deleteType){
    MBR mbr = this->funciones.leerMBR(path.c_str());
    if(mbr.mbr_tamano==0){
        cout<<"ERROR!, fdisk delete, no existe el disco: "<<path<<endl;
        return;
    }
    Partition *part = NULL;
    int numPart = funciones.existPartition(mbr,name);
    if(numPart==1){
        part = &mbr.mbr_partition1;
    }else if(numPart==2){
        part = &mbr.mbr_partition2;
    }else if(numPart==3){
        part = &mbr.mbr_partition3;
    }else if(numPart==4){
        part = &mbr.mbr_partition4;
    }else {
        cout<<"ERROR!, fdisk delete, no existe la particion: "<<name<<endl;
        return;
    }

    if(deleteType.compare("fast")==0){
        part->part_status='0';
    }else{
        Partition aux =this->funciones.getNewPartition();
        part->part_fit=aux.part_fit;
        memset(part->part_name,0,sizeof(part->part_name));
        part->part_size=aux.part_size;
        part->part_start=aux.part_start;
        part->part_status=aux.part_status;
        part->part_type=aux.part_type;

    }
    if(funciones.escribirMBR(path,mbr)){
        cout<<"FDISK, Se elimino la particion: "<<name<<endl;
    }else{
        cout<<"ERROR! FDISK, delete, no se puedo aumentar eliminar la particion: "<<name<<endl;
    }

}

void Comando::FdiskAdd(string name, string path, char unit, int add){
     //verificar parametros
    MBR mbr = this->funciones.leerMBR(path.c_str());
    if(mbr.mbr_tamano==0){
        cout<<"ERROR!, fdisk add, no existe el disco: "<<path<<endl;
        return;
    }
    Partition *part = NULL;
    int numPart = funciones.existPartition(mbr,name);
    if(numPart==1){
        part = &mbr.mbr_partition1;
    }else if(numPart==2){
        part = &mbr.mbr_partition2;
    }else if(numPart==3){
        part = &mbr.mbr_partition3;
    }else if(numPart==4){
        part = &mbr.mbr_partition4;
    }else {
        cout<<"ERROR!, fdisk add, no existe la particion: "<<name<<endl;
        return;
    }
    switch(unit)
    {
    case'b':
        break;
    case'k':
    case 0:
        add*=1024;
        break;
    case'm':
        add*=1024*1024;
        break;
    default:
        printf("ERROR!,fdisk unit, Unidad de medida no reconocida.\n");
        return;
    }


    if(add > 0){
        vector<Espacio> espacios = funciones.obtenerEspacios(mbr);
        int limite = part->part_start + part->part_size;
        bool error=true;
        for(unsigned int i = 0; i<espacios.size();i++){
            if(limite == espacios[i].pos){
                int disponible=espacios[i].pos+espacios[i].size;
                int total = limite + add;
                if(total<=disponible){
                    part->part_size=part->part_size+add;
                    error=false;
                    break;
                }else{
                    cout<<"ERROR!,fdisk add, no hay espacio suficiente para aumentar la particion: "<<name<<endl;
                    return;
                }
            }
        }
        if(error){
            cout<<"ERROR!,fdisk add, no hay espacio disponible para aumentar la particion: "<<name<<endl;
            return;
        }
    }else{
        int reduce = part->part_size + add;
        if(reduce<=0){
            cout<<"ERROR!,fdisk add negativo o cero, la particion: "<<name<<endl;
            return;
        }
        part->part_size = reduce;
    }

    if(funciones.escribirMBR(path,mbr)){
        cout<<"FDISK, add, se aumento el tamaño de la particion: "<<name<<endl;
    }else{
        cout<<"ERROR! FDISK, add, no se puedo aumentar el tamaño de la particion: "<<name<<endl;
    }

}


void Comando::Mount(string path, string name){
    MBR mbr = this->funciones.leerMBR(path.c_str());
    if(mbr.mbr_tamano==0){
        cout<<"ERROR!, mount, no existe el disco: "<<path<<endl;
        return;
    }
    int numPart = funciones.existPartition(mbr,name);
    if(numPart==0){
        cout<<"ERROR!, mount, no existe la particion: "<<name<<endl;
        return;
    }

    for(int i=0;i<20;i++)
    {
        if(strcmp(mountDisk[i].idDisk,path.c_str()) == 0)
        {
            SEARCHMOUNTID:
            for(int j=0;j<20;j++)
            {   if(strcmp(mountDisk[i].mountPart[j].idPart,name.c_str())==0)
                {
                    printf( "ERROR!, esta particion ya fue montada, id = " );
                    printf("%s\n" ,mountDisk[i].mountPart[j].idMount);
                    return;
                }
                if(strcmp(mountDisk[i].mountPart[j].idPart,"") == 0)
                {
                    strcat(mountDisk[i].mountPart[j].idPart,name.c_str());
                    strcat(mountDisk[i].mountPart[j].idMount,"vd");
                    mountDisk[i].mountPart[j].idMount[2] = static_cast<char>((97+i));
                    char integer_string[32];
                    int integer = j+1;
                    sprintf(integer_string, "%d", integer);
                    strcat(mountDisk[i].mountPart[j].idMount, integer_string);
                    printf( "id = ");
                    printf("%s\n" ,mountDisk[i].mountPart[j].idMount);
                    mountDisk[i].countParts++;
                    return;
                }
            }
        }
        int x = funciones.getDskMountIndex(path.c_str(),mountDisk);
        if(strcmp(mountDisk[x].idDisk,path.c_str()) == 0)
        {
            i = x;
            goto SEARCHMOUNTID;
        }
        else if(strcmp(mountDisk[i].idDisk,"") == 0)
        {
            strcat(mountDisk[i].idDisk,path.c_str());
            strcat(mountDisk[i].mountPart[0].idPart,name.c_str());
            strcat(mountDisk[i].mountPart[0].idMount,"vd");
            mountDisk[i].mountPart[0].idMount[2] = static_cast<char>(97+i);
            strcat(mountDisk[i].mountPart[0].idMount,"1");
            printf( "id = ");
            printf("%s\n" ,mountDisk[i].mountPart[0].idMount);
            mountDisk[i].countParts = 1;
            return;
        }
    }
}

void Comando::UnMountPartition(const char* id)
{
    for(int i=0;i<20;i++)
    {
        for(int j=0;j<20;j++)
        {
            if(strcmp(mountDisk[i].mountPart[j].idMount,id) == 0)
            {
                memset(mountDisk[i].mountPart[j].idMount,0
                       , sizeof(mountDisk[i].mountPart[j].idMount));
                memset(mountDisk[i].mountPart[j].idPart,0
                       , sizeof(mountDisk[i].mountPart[j].idPart));
                mountDisk[i].countParts--;
                if(mountDisk[i].countParts <= 0)
                {
                    memset(mountDisk[i].idDisk,0
                           , sizeof(mountDisk[i].idDisk));
                    mountDisk[i].countParts = 0;
                }
                return;
            }
        }
    }
    cout<<"ERROR!,unmount, No existe particion con este id: "<<id<<endl;
}

void Comando::Rep(string id, string path, string name){
    MountDisk mounD = funciones.existIdPartMount(id,mountDisk);
    if(strcmp(mounD.idDisk,"") == 0){
        cout<<"ERROR!, reporte, no esta montada la particion: "<<id<<endl;
        return;
    }
    if(name.compare("mbr")==0){
        reporte.RepMbr(mounD.idDisk,path.c_str());
        cout<<"Se creo el reporte mbr, de la particion: "<<id<<endl;
    }else if(name.compare("disk")==0){
        reporte.RepDisk(mounD.idDisk,path.c_str());
        cout<<"Se creo el reporte disk, de la particion: "<<id<<endl;
    }else if(name.compare("bm_inode")==0){
        reporte.RepBm(mounD.idDisk,path.c_str(),id,mountDisk,true);
        cout<<"Se creo el reporte bm_inode, de la particion: "<<id<<endl;
    }else if(name.compare("bm_block")==0){
        reporte.RepBm(mounD.idDisk,path.c_str(),id,mountDisk,false);
        cout<<"Se creo el reporte bm_block, de la particion: "<<id<<endl;
    }else if(name.compare("block")==0){
        reporte.RepBlock(mounD.idDisk,path.c_str(),id,mountDisk);
        cout<<"Se creo el reporte block, de la particion: "<<id<<endl;
    }else if(name.compare("inode")==0){
        reporte.RepInode(mounD.idDisk,path.c_str(),id,mountDisk);
        cout<<"Se creo el reporte inode, de la particion: "<<id<<endl;
    }else{
        cout<<"ERROR!, reporte, no existe ese  reporte."<<endl;
    }
}

void Comando::mkfs(string id, string type){
    string partName;
    string path = funciones.getPathByID(id.c_str(),&partName,mountDisk);
    if(path.compare("")==0){
        cout<<"ERROR!, mkfs, no encuentra el disco asociado al id: "<<id<<endl;
        return;
    }
    MBR mbr = funciones.leerMBR(path.c_str());
    if(mbr.mbr_tamano==0){
        cout<<"ERROR!, mkfs, no encontro el disco: "<<path<<endl;
        return;
    }
    Partition* part=funciones.get_partition_name(&mbr,partName);
    if(part==NULL){
        cout<<"ERROR!, mkfs, no encontro la particion solicitada: "<<partName<<endl;
        return;
    }
    int n = funciones.numEstructuras(part->part_size);
    Superbloque superbloque;
    superbloque.s_inodes_count=n;
    superbloque.s_blocks_count=3*n;
    superbloque.s_free_blocks_count=(3*n)-1;
    superbloque.s_free_inodes_count=n-1;
    superbloque.s_mtime=time(NULL);
    superbloque.s_umtime=0;
    superbloque.s_mnt_count=1;
    superbloque.s_magic=0xEF53;
    superbloque.s_inode_size=sizeof(Inodo);
    superbloque.s_block_size=sizeof(Barchivo);

    superbloque.s_bm_inode_start=part->part_start+sizeof(Superbloque)+sizeof(Journaling)*n;
    superbloque.s_bm_block_start=superbloque.s_bm_inode_start + n;
    superbloque.s_inode_start=superbloque.s_bm_block_start + n*3;
    superbloque.s_block_start=  superbloque.s_inode_start + sizeof(Inodo)*n;

    superbloque.s_firts_ino=superbloque.s_inode_start;
    superbloque.s_first_blo=superbloque.s_block_start;
    funciones.setSuperbloque(path,superbloque,part->part_start);
    funciones.setBitMapsInicio(n,superbloque.s_bm_inode_start,superbloque.s_bm_block_start,path);


    //formateo fast o full
    if(type.compare("full")==0){
        if(!funciones.formatearTabla_inodos_bloques(path,superbloque,part->part_start)){
            cout<<"ERROR!,MKFS full no se pudo formatear la tabla indodos o tablo bloque, particion: "<<partName<<endl;
        }
    }


    //crear inodo y carpeta raiz

    Inodo raiz;
    raiz.i_uid=1;
    raiz.i_gid=1;
    raiz.i_size=0;
    raiz.i_atime=0;
    raiz.i_ctime=time(NULL);
    raiz.i_mtime=0;
    memset(raiz.i_block,-1,sizeof raiz.i_block);
    raiz.i_block[0]=0;
    raiz.i_type=0;
    raiz.i_perm=770;

    if(!(funciones.ingresarInodo(path,raiz,superbloque.s_inode_start,0) && funciones.ingresar_bitmap(path,superbloque.s_bm_inode_start,0))){
        cout<<"ERROR!,MKFS no se pudo ingresar el inodo: raiz"<<endl;
        return;
    }

    Bcarpeta c_raiz;
    strcpy(c_raiz.b_content[0].name,".");
    c_raiz.b_content[0].b_inodo=0;

    strcpy(c_raiz.b_content[1].name,"..");
    c_raiz.b_content[1].b_inodo=0;

    strcpy(c_raiz.b_content[2].name,"users.txt");
    c_raiz.b_content[2].b_inodo=1;

    strcpy(c_raiz.b_content[3].name,"-");
    c_raiz.b_content[3].b_inodo=-1;

    if(!(funciones.ingresarBloqueCarpeta(path,c_raiz,superbloque.s_block_start,0) && funciones.ingresar_bitmap(path,superbloque.s_bm_block_start,0))){
        cout<<"ERROR!,MKFS no se pudo ingresar el bloque carpeta: raiz"<<endl;
        return;
    }

    //crear inodo y bloque users.txt
    Inodo i_users;
    i_users.i_uid=1;
    i_users.i_gid=1;
    i_users.i_size=27;
    i_users.i_atime=0;
    i_users.i_ctime=time(NULL);
    i_users.i_mtime=0;
    memset(i_users.i_block,-1,sizeof i_users.i_block);
    i_users.i_block[0]=1;
    i_users.i_type=1;
    i_users.i_perm=770;

    if(!(funciones.ingresarInodo(path,i_users,superbloque.s_inode_start,1) && funciones.ingresar_bitmap(path,superbloque.s_bm_inode_start,1))){
        cout<<"ERROR!,MKFS no se pudo ingresar el inodo: users.txt"<<endl;
        return;
    }
    Barchivo b_users;
    strcpy(b_users.b_content,"1,G,Root\n1,U,Root,root,123\n");
    if(!(funciones.ingresarBloqueArchivo(path,b_users,superbloque.s_block_start,1) && funciones.ingresar_bitmap(path,superbloque.s_bm_block_start,1))){
        cout<<"ERROR!,MKFS no se pudo ingresar el bloque archivo: users.txt"<<endl;
        return;
    }





    cout<<"MKFS full exitoso, particion: "<<partName<<endl;

    //Journaling****
    string txt_type="";
    if(type.compare("")!=0)
        txt_type=" -type="+type;
    string log="mkfs -id="+id+txt_type;
    funciones.Fjournaling(path,log,part->part_start);
    //****

}

void Comando::login(string id, string usr, string pwd){
    if(this->log_usuario.log==1){
        cout<<"ERROR!, login, primero cerrar sesión."<<endl;
        return;
    }
    string partName;
    string path = funciones.getPathByID(id.c_str(),&partName,mountDisk);
    if(path.compare("")==0){
        cout<<"ERROR!, login, no encuentra el disco asociado al id: "<<id<<endl;
        return;
    }
    MBR mbr = funciones.leerMBR(path.c_str());
    if(mbr.mbr_tamano==0){
        cout<<"ERROR!, login, no encontro el disco: "<<path<<endl;
        return;
    }
    Partition* part=funciones.get_partition_name(&mbr,partName);
    if(part==NULL){
        cout<<"ERROR!, login, no encontro la particion solicitada: "<<partName<<endl;
        return;
    }
    Superbloque superbloque = funciones.getSuperbloque(path,partName);
    if(superbloque.s_magic!=0xEF53){
        cout<<"ERROR!, login, no encontro el superbloque de la particion: "<<partName<<endl;
        return;
    }
    //listar los usuarios
    Barchivo a_users=funciones.getBarchivo(path,superbloque.s_block_start,1);
    string text = a_users.b_content;
    StringVector l1=funciones.Explode(text,'\n');
    string vacio=l1.at(l1.size()-1);
    if(vacio.compare("")==0){
            l1.erase(l1.end());
    }

    vector<Usuario> lista_u;
    for(unsigned int i=0;i<l1.size();i++){
        StringVector l2=funciones.Explode(l1[i],',');
        Usuario usuario;
        if(l2.size()==5){
            usuario.id=stoi(l2[0]);
            usuario.tipo=l2[1];
            usuario.grupo=l2[2];
            usuario.usr=l2[3];
            usuario.pwd=l2[4];
        }else {
            usuario.id=stoi(l2[0]);
            usuario.tipo=l2[1];
            usuario.grupo=l2[2];
        }
        lista_u.insert(lista_u.end(),usuario);
    }


    //comprobar usuario y contraseña
    Usuario us;
    bool encontro_u=0;
    bool encontro_p=0;
    for(unsigned int i=0;i<lista_u.size();i++){
        us=lista_u[i];
        if(us.tipo.compare("U")==0 && us.usr.compare(usr)==0){
            encontro_u=1;
            if(us.pwd.compare(pwd)==0){
                encontro_p=1;
            }
            break;
        }
    }
    if(!encontro_u){
        cout<<"ERROR!, no encontro usuario: "<<usr<<endl;
        return;
    }else if(encontro_u && !encontro_p) {
        cout<<"ERROR!, contraseña invalida usuario: "<<usr<<endl;
        return;
    }

    //entontrar gid
    int numGrupo=0;
    Usuario gr;
    for(unsigned int i=0;i<lista_u.size();i++){
        gr=lista_u[i];
        if(gr.tipo.compare("G")==0 && gr.grupo.compare(us.grupo)==0){
            numGrupo=gr.id;
            break;
        }
    }

    this->log_usuario.log=1;
    this->log_usuario.user=usr;
    this->log_usuario.pass=pwd;
    this->log_usuario.partName=partName;
    this->log_usuario.path=path;
    this->log_usuario.superbloque=superbloque;
    this->log_usuario.uid=us.id;
    this->log_usuario.gid=numGrupo;
    this->log_usuario.partStart=part->part_start;
    cout<<"Login exitoso!, usuario: "<<usr<<endl;


    //Journaling****
    string txt_id="";
    string txt_usr="";
    string txt_pwd="";
    if(id.compare("")!=0)
        txt_id=" -id="+id;
    if(usr.compare("")!=0)
        txt_usr=" -usr="+usr;
    if(pwd.compare("")!=0)
        txt_pwd=" -pwd="+pwd;

    string log="login "+txt_id+txt_usr+txt_pwd;
    funciones.Fjournaling(this->log_usuario.path,log,this->log_usuario.partStart);
    //****

}

void Comando::logout(){
    if(this->log_usuario.log==0){
        cout<<"ERROR!, logout, primero iniciar sesión."<<endl;
        return;
    }
    Superbloque superb;
    this->log_usuario.log=0;
    this->log_usuario.partName="";
    this->log_usuario.pass="";
    this->log_usuario.path="";
    this->log_usuario.superbloque=superb;
    this->log_usuario.user="";
    this->log_usuario.uid=0;
    this->log_usuario.gid=0;
    this->log_usuario.partStart=-1;
    cout<<"logout, se cerro sesión."<<endl;

    //Journaling****


    string log="logout";
    funciones.Fjournaling(this->log_usuario.path,log,this->log_usuario.partStart);
    //****

}

void Comando::mkgrp(string name){
    if(this->log_usuario.log==0){
        cout<<"ERROR!, mkgrp, primero iniciar sesión."<<endl;
        return;
    }
    if(!this->log_usuario.user.compare("root")==0){
        cout<<"ERROR!, mkgrp, no tiene permiso de utilizar el comando, usr: "<<this->log_usuario.user<<endl;
        return;
    }
    //Journaling****
    string log="mkgrp -name="+name;
    funciones.Fjournaling(this->log_usuario.path,log,this->log_usuario.partStart);
    //****
    //listar los usuarios
    string text = funciones.leerArchivo(this->log_usuario.path,this->log_usuario.superbloque,"/users.txt");
    if(text.compare("")==0){
        cout<<"ERROR!, mkgrp,no pudo leer el archivo users.txt"<<endl;
        return;
    }

    //string text="1,G,Root\n1,U,root,root,123\n2,G,usuarios\n3,G,otros\n";
    StringVector l1=funciones.Explode(text,'\n');
    string vacio=l1.at(l1.size()-1);
    if(vacio.compare("")==0){
            l1.erase(l1.end());
    }

    vector<Usuario> lista_g;
    for(unsigned int i=0;i<l1.size();i++){
        StringVector l2=funciones.Explode(l1[i],',');
        if(l2.size()==3){
            Usuario *usuario=new Usuario;
            usuario->id=stoi(l2[0]);
            usuario->tipo=l2[1];
            usuario->grupo=l2[2];
            lista_g.push_back(*usuario);
        }

    }

    //comprobaciones grupo
    Usuario *usu=new Usuario;
    for(unsigned int i=0;i<lista_g.size();i++){
        *usu=lista_g[i];
        if(usu->id!=0 && usu->grupo.compare(name)==0){
            cout<<"ERROR!, mkgrp, grupo ya existente: "<<name<<endl;
            return;
        }
    }


    int tam=lista_g.size()+1;
    string nuevo_g=to_string(tam)+",G,"+name+"\n";
    Inodo inodo= funciones.getInode(this->log_usuario.path,this->log_usuario.superbloque.s_inode_start,1);
    if(!funciones.setBloque(inodo,nuevo_g,this->log_usuario.superbloque,this->log_usuario.path,1)){
        cout<<"ERROR!,mkgrp no se pudo ingresar el bloque archivo: users.txt"<<endl;
        return;
    }

    cout<<"mkgrp exitoso!, se creo el grupo: "<<name<<endl;



}


void Comando::rmgrp(string name){
    if(this->log_usuario.log==0){
        cout<<"ERROR!, rmgrp, primero iniciar sesión."<<endl;
        return;
    }
    if(!this->log_usuario.user.compare("root")==0){
        cout<<"ERROR!, rmgrp, no tiene permiso de utilizar el comando, usr: "<<this->log_usuario.user<<endl;
        return;
    }
    //Journaling****
    string log="rmgrp -name="+name;
    funciones.Fjournaling(this->log_usuario.path,log,this->log_usuario.partStart);
    //****
    //listar los usuarios
    string text = funciones.leerArchivo(this->log_usuario.path,this->log_usuario.superbloque,"/users.txt");
    if(text.compare("")==0){
        cout<<"ERROR!, rmgrp,no pudo leer el archivo users.txt"<<endl;
        return;
    }

    size_t pos = text.find(",G,"+name);
    if(pos==std::string::npos){
        cout<<"ERROR!, rmgrp,no encontro el grupo: "<<name<<endl;
        return;
    }
    if(text.at(pos-1)=='0'){
        cout<<"ERROR!, rmgrp,no encontro el grupo: "<<name<<endl;
        return;
    }
    text.replace(pos-1,1,"0");

    Inodo inodo= funciones.getInode(this->log_usuario.path,this->log_usuario.superbloque.s_inode_start,1);


    int res=0;
    int cont=0;
    do{
        res=text.size()-64;
        string aux=text.substr(0,64);
        text.replace(0,64,"");

        Barchivo archivo=funciones.getBarchivo(this->log_usuario.path,this->log_usuario.superbloque.s_block_start,inodo.i_block[cont]);
        strcpy(archivo.b_content,aux.c_str());
        if(!(funciones.ingresarBloqueArchivo(this->log_usuario.path,archivo,this->log_usuario.superbloque.s_block_start,inodo.i_block[cont]) )){
            cout<<"ERROR!,rmgrp no se pudo ingresar el bloque archivo: users.txt"<<endl;
            return;
        }
        cont++;
    }while(res >=0);



    cout<<"rmgrp exitoso!, se borro el grupo: "<<name<<endl;
}


void Comando::mkusr(string grp, string usr, string pwd){
    if(this->log_usuario.log==0){
        cout<<"ERROR!, mkusr, primero iniciar sesión."<<endl;
        return;
    }
    if(!this->log_usuario.user.compare("root")==0){
        cout<<"ERROR!, mkusr, no tiene permiso de utilizar el comando, usr: "<<this->log_usuario.user<<endl;
        return;
    }
    //Journaling****
    string log="mkusr -usr="+usr+" -pwd="+pwd+" -grp="+grp;
    funciones.Fjournaling(this->log_usuario.path,log,this->log_usuario.partStart);
    //****
    //listar los usuarios
    string text = funciones.leerArchivo(this->log_usuario.path,this->log_usuario.superbloque,"/users.txt");
    if(text.compare("")==0){
        cout<<"ERROR!, mkusr,no pudo leer el archivo users.txt"<<endl;
        return;
    }

    size_t pos = text.find(",G,"+grp);
    if(pos==std::string::npos){
        cout<<"ERROR!, mkusr,no encontro el grupo: "<<grp<<endl;
        return;
    }

    //string text="1,G,Root\n1,U,root,root,123\n2,G,usuarios\n3,G,otros\n";
    StringVector l1=funciones.Explode(text,'\n');
    string vacio=l1.at(l1.size()-1);
    if(vacio.compare("")==0){
            l1.erase(l1.end());
    }

    vector<Usuario> lista_g;
    for(unsigned int i=0;i<l1.size();i++){
        StringVector l2=funciones.Explode(l1[i],',');
        if(l2.size()==5){
            Usuario *usuario=new Usuario;
            usuario->id=stoi(l2[0]);
            usuario->tipo=l2[1];
            usuario->grupo=l2[2];
            usuario->usr=l2[3];
            usuario->pwd=l2[4];
            lista_g.push_back(*usuario);
        }

    }

    //comprobaciones usuario
    Usuario *usu=new Usuario;
    for(unsigned int i=0;i<lista_g.size();i++){
        *usu=lista_g[i];
        if(usu->id!=0 && usu->usr.compare(usr)==0){
            cout<<"ERROR!, mkusr, usuario ya existente: "<<usr<<endl;
            return;
        }
    }

    int tam=lista_g.size()+1;
    string nuevo_g=to_string(tam)+",U,"+grp+","+usr+","+pwd+"\n";
    Inodo inodo= funciones.getInode(this->log_usuario.path,this->log_usuario.superbloque.s_inode_start,1);
    if(!funciones.setBloque(inodo,nuevo_g,this->log_usuario.superbloque,this->log_usuario.path,1)){
        cout<<"ERROR!,mkusr no se pudo ingresar el bloque archivo: users.txt"<<endl;
        return;
    }

    cout<<"mkusr exitoso!, se creo el usuario: "<<usr<<endl;

}

void Comando::rmusr(string name){
    if(this->log_usuario.log==0){
        cout<<"ERROR!, rmusr, primero iniciar sesión."<<endl;
        return;
    }
    if(!this->log_usuario.user.compare("root")==0){
        cout<<"ERROR!, rmusr, no tiene permiso de utilizar el comando, usr: "<<this->log_usuario.user<<endl;
        return;
    }
    //Journaling****
    string log="rmusr -name="+name;
    funciones.Fjournaling(this->log_usuario.path,log,this->log_usuario.partStart);
    //****
    //listar los usuarios
    string text = funciones.leerArchivo(this->log_usuario.path,this->log_usuario.superbloque,"/users.txt");
    if(text.compare("")==0){
        cout<<"ERROR!, rmusr,no pudo leer el archivo users.txt"<<endl;
        return;
    }


    //string text="1,G,Root\n1,U,root,root,123\n2,G,usuarios\n3,G,otros\n";
    StringVector l1=funciones.Explode(text,'\n');
    string vacio=l1.at(l1.size()-1);
    if(vacio.compare("")==0){
            l1.erase(l1.end());
    }

    vector<Usuario> lista_g;
    for(unsigned int i=0;i<l1.size();i++){
        StringVector l2=funciones.Explode(l1[i],',');
        if(l2.size()==5){
            Usuario *usuario=new Usuario;
            usuario->id=stoi(l2[0]);
            usuario->tipo=l2[1];
            usuario->grupo=l2[2];
            usuario->usr=l2[3];
            usuario->pwd=l2[4];
            lista_g.push_back(*usuario);
        }

    }

    //comprobaciones usuario
    bool noEncotro=true;
    Usuario *usu=new Usuario;
    for(unsigned int i=0;i<lista_g.size();i++){
        *usu=lista_g[i];
        if(usu->id!=0 && usu->usr.compare(name)==0){
            usu->id=0;
            noEncotro=false;
            break;
        }
    }
    if(noEncotro){
        cout<<"ERROR!, rmusr, no enonctro usuario: "<<name<<endl;
        return;
    }
    size_t pos = text.find(",U,"+usu->grupo+","+name);
    if(pos==std::string::npos){
        cout<<"ERROR!, rmusr,no encontro el usuario: "<<name<<endl;
        return;
    }
    if(text.at(pos-1)=='0'){
        cout<<"ERROR!, rmusr,no encontro el usuario: "<<name<<endl;
        return;
    }
    text.replace(pos-1,1,"0");


    //ingreso
    Inodo inodo= funciones.getInode(this->log_usuario.path,this->log_usuario.superbloque.s_inode_start,1);


    int res=0;
    int cont=0;
    do{
        res=text.size()-64;
        string aux=text.substr(0,64);
        text.replace(0,64,"");

        Barchivo archivo=funciones.getBarchivo(this->log_usuario.path,this->log_usuario.superbloque.s_block_start,inodo.i_block[cont]);
        strcpy(archivo.b_content,aux.c_str());
        if(!(funciones.ingresarBloqueArchivo(this->log_usuario.path,archivo,this->log_usuario.superbloque.s_block_start,inodo.i_block[cont]) )){
            cout<<"ERROR!,rmusr no se pudo ingresar el bloque archivo: users.txt"<<endl;
            return;
        }
        cont++;
    }while(res >=0);



    cout<<"rmusr exitoso!, se borro el usuario: "<<name<<endl;
}

void Comando::mkdir(string dir, bool p){
    if(this->log_usuario.log==0){
        cout<<"ERROR!, mkdir, primero iniciar sesión."<<endl;
        return;
    }
    //Journaling****
    string txt_p="";
    if(p)
        txt_p=" -p";

    string log="mkdir -path="+dir+txt_p;
    funciones.Fjournaling(this->log_usuario.path,log,this->log_usuario.partStart);
    //****


    Inodo inodo;
    inodo.i_uid=this->log_usuario.uid;
    inodo.i_gid=this->log_usuario.gid;
    inodo.i_size=0;
    inodo.i_atime=0;
    inodo.i_ctime=time(NULL);
    inodo.i_mtime=0;
    memset(inodo.i_block,-1,sizeof inodo.i_block);
    inodo.i_type=0;
    inodo.i_perm=664;



    if(!funciones.Fmkdir(inodo,dir,this->log_usuario.path,this->log_usuario.superbloque)){
        cout<<"ERROR!, mkdir, no se pudo crear la carpeta: "<<dir<<endl;
        return;
    }

    cout<<"mkdir exitoso!, se creo la carpeta: "<<dir<<endl;
}

void Comando::mkfile(string dir, bool p, string cont, int size){
    if(this->log_usuario.log==0){
        cout<<"ERROR!, mkfile, primero iniciar sesión."<<endl;
        return;
    }
    //Journaling****
    string txt_cont="";
    string txt_size="";
    string txt_p="";
    if(p)
        txt_p=" -p";
    if(cont.compare("")!=0)
        txt_cont=" -cont="+cont;
    if(size!=0)
        txt_size=" -size="+to_string(size);

    string log="mkfile -path="+dir+txt_p+txt_cont+txt_size;
    funciones.Fjournaling(this->log_usuario.path,log,this->log_usuario.partStart);
    //****

    string text="";
    if(cont.compare("")==0){
        if(size!=0){
            int contador=0;
            for(int i=0;i<size;i++){
                text+=to_string(contador);
                contador++;
                if(contador==10)
                    contador=0;
            }
        }
    }else{
        text=funciones.leerArchivoFisico(cont);
        if(text.compare("")==0){
            cout<<"ERROR!, mkfile, no pudo leer o archivo vacio: "<<cont<<endl;
            return;
        }
    }



    //crear inodo
    Inodo inodo;
    inodo.i_uid=this->log_usuario.uid;
    inodo.i_gid=this->log_usuario.gid;
    inodo.i_size=text.size();
    inodo.i_atime=0;
    inodo.i_ctime=time(NULL);
    inodo.i_mtime=0;
    memset(inodo.i_block,-1,sizeof inodo.i_block);
    inodo.i_type=1;
    inodo.i_perm=664;


    //crear bloqueArchivo


    int res=0;
    int contador=0;
    do{
        res=text.size()-64;
        string aux=text.substr(0,64);
        text.replace(0,64,"");

        Barchivo archivo;
        strcpy(archivo.b_content,aux.c_str());
        int numBarchivo=funciones.bloqueLibre(this->log_usuario.path,this->log_usuario.superbloque);
        if(!(funciones.ingresarBloqueArchivo(this->log_usuario.path,archivo,this->log_usuario.superbloque.s_block_start,numBarchivo)
             && funciones.ingresar_bitmap(this->log_usuario.path,this->log_usuario.superbloque.s_bm_block_start,numBarchivo) )){
            cout<<"ERROR!,mkfile no se pudo ingresar el bloque archivo:"<<dir<<endl;
            return;
        }
        inodo.i_block[contador]=numBarchivo;
        contador++;
    }while(res >=0);

    if(!funciones.Fmkfile(inodo,dir,this->log_usuario.path,this->log_usuario.superbloque)){
        cout<<"ERROR!, mkfile, no se pudo crear el archivo: "<<dir<<endl;
        return;
    }


    cout<<"mkfile exitoso!, se creo el archivo: "<<dir<<endl;
}


void Comando::cat(string file){
    if(this->log_usuario.log==0){
        cout<<"ERROR!, cat, primero iniciar sesión."<<endl;
        return;
    }

    string text="";
    if(!funciones.Fcat(file,this->log_usuario.path,this->log_usuario.superbloque,&text)){
        cout<<"ERROR!, cat, no pudo leer el archivo: "<<file<<endl;
        return;
    }

    cout<<"Cat Exitoso!, Archivo "<<file<<":\n"<<text<<endl;


}


StringVector Comando::recovery(string id){
    StringVector l1;
    string partName;
    string path = funciones.getPathByID(id.c_str(),&partName,mountDisk);
    if(path.compare("")==0){
        cout<<"ERROR!, recovery, no encuentra el disco asociado al id: "<<id<<endl;
        return l1;
    }
    MBR mbr = funciones.leerMBR(path.c_str());
    if(mbr.mbr_tamano==0){
        cout<<"ERROR!, recovery, no encontro el disco: "<<path<<endl;
        return l1;
    }
    Partition* part=funciones.get_partition_name(&mbr,partName);
    if(part==NULL){
        cout<<"ERROR!, recovery, no encontro la particion solicitada: "<<partName<<endl;
        return l1;
    }
    Superbloque superbloque = funciones.getSuperbloque(path,partName);
    if(superbloque.s_magic!=0xEF53){
        cout<<"ERROR!, recovery, no encontro el superbloque de la particion: "<<partName<<endl;
        return l1;
    }
    string texto=funciones.Frecovery(path,part->part_start);

    StringVector comandos = funciones.Explode(texto,'\n');
    string vacio=comandos.at(comandos.size()-1);
    if(vacio.compare("")==0)
        comandos.erase(comandos.end());

    return comandos;

}


void Comando::loss(string id){
    string partName;
    string path = funciones.getPathByID(id.c_str(),&partName,mountDisk);
    if(path.compare("")==0){
        cout<<"ERROR!, loss, no encuentra el disco asociado al id: "<<id<<endl;
        return ;
    }
    MBR mbr = funciones.leerMBR(path.c_str());
    if(mbr.mbr_tamano==0){
        cout<<"ERROR!, loss, no encontro el disco: "<<path<<endl;
        return ;
    }
    Partition* part=funciones.get_partition_name(&mbr,partName);
    if(part==NULL){
        cout<<"ERROR!, loss, no encontro la particion solicitada: "<<partName<<endl;
        return ;
    }
    Superbloque superbloque = funciones.getSuperbloque(path,partName);
    if(superbloque.s_magic!=0xEF53){
        cout<<"ERROR!, loss, no encontro el superbloque de la particion: "<<partName<<endl;
        return ;
    }

    if(!funciones.Floss(path,superbloque)){
        cout<<"ERROR!, loss, no pudo formatear la particion: "<<id<<endl;
        return ;
    }
    funciones.setBitMapsInicio(superbloque.s_inodes_count,superbloque.s_bm_inode_start,superbloque.s_bm_block_start,path);

    cout<<"loss exitoso!, se formateo correctamente, la particion: "<<id<<endl;
}
