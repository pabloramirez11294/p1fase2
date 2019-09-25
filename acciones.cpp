#include "acciones.h"
#include"comando.h"

void Acciones::leerComando(char *entrada)
{
    int i = 0;
    if(entrada[i]=='\n' )
        return;
    //volver minusculas
    bool noMinus=false;
    string cmd="";
    while(entrada[i])
    {


        char pwd[5];
        if(i>4){
            char pwd2[5]={entrada[i-3],entrada[i-2],entrada[i-1],entrada[i],'\0'};
            strcpy(pwd,pwd2);
        }

        string pass(pwd);
        if(pass.compare("-pwd")==0 || pass.compare("-usr")==0 || (noMinus && pass.compare("-nam")==0)){
            while(entrada[i]){
                i++;
                if(entrada[i]=='-')
                    break;
            }
        }
        entrada[i] = tolower(entrada[i]);
        cmd+=entrada[i];
        if(!noMinus && (cmd.compare("mkgrp")==0 || cmd.compare("rmgrp")==0)){
            noMinus=true;
        }
        i++;
    }

    i = 0;
    int numToken=0;
    char tok[100];
    Token token[15];
    memset(token,0,sizeof token);

    while(entrada[i])
    {
        int asciichar = toascii(entrada[i]);
        //a-z y 0-9
        if((asciichar >= 65 && asciichar <= 90) ||(asciichar >= 97 && asciichar <= 122) ||
                (asciichar >= 45 && asciichar <= 57))
        {
            memset(tok,0,sizeof tok);
            i = getToken(entrada,i,tok);
            strcat(token[numToken++].value,tok);
        }
        //"
        else if(asciichar == 34)
        {
            i++;
            memset(tok,0,sizeof tok);
            i = getSpaceToken(entrada,i,tok);
            strcat(token[numToken++].value,tok);
        }
        //#
        else if(asciichar == 35)
        {

            while(entrada[i])
            {
                i++;
                asciichar = toascii(entrada[i]);
                if(asciichar==toascii('\n') || asciichar==toascii('\r') || asciichar==toascii('\0')){
                    break;
                }
            }
        }
        i++;
    }
    ejecutarComando(token);
    fflush(stdin);
    fflush(stdout);
}

int Acciones::getToken(char* cmd, int i,char* token)
{
    int j = 0;
    while(cmd[i] && cmd[i] != ' ' && cmd[i] != '=' && cmd[i] != '\n' && cmd[i] != '\r')
    {
        token[j++] = cmd[i++];
    }
    return i;
}
int Acciones::getSpaceToken(char* cmd, int i,char* token)
{
    int j = 0;
    while(cmd[i] && cmd[i] != '\"')
    {
        token[j++] = cmd[i++];
    }
    return i;
}


void Acciones::ejecutarComando(Token token[]){
    int i = 0;
    string nomComando=token[i].value;

    //Parametros a enviar
    string path="";
    string id="";
    int size = 0;
    char unit=0;
    string fit="";
    string cadType="";
    string name="";
    string fs="2fs";
    int Addsize = 0;
    char type=0;
    string usr="";
    string pwd="";
    //Final Parametros

    while(strcmp(token[i+1].value,"")!=0)
    {
        i++;
        if(strcmp(nomComando.c_str(),"exec")==0){
            if(strcmp(token[i].value,"-path")==0)
            {i++;
                path = token[i].value;
                ejecutarScript(path.c_str());
            }else
            printf("ERROR!,exec, faltan parametros\n");

            return;
        }
        if(strcmp(nomComando.c_str(),"mkdisk")==0)
        {
            if(strcmp(token[i].value,"-size")==0)
            {i++;
                size = atoi(token[i].value);
                if(size<=0){
                    printf("ERROR!,mkdisk, El tamaño del disco debe ser positivo y mayor que cero.\n");
                    return;
                }
            }

            if(strcmp(token[i].value,"-unit")==0)
            {i++;
                unit = token[i].value[0];
                if(unit!='k' && unit !='m'){
                    printf("ERROR!,mkdisk unit, Las unidades deben estar en m o k\n");
                    return;
                }
            }

            if(strcmp(token[i].value,"-path")==0)
            {i++;
                path = token[i].value;
            }
            if(strcmp(token[i].value,"-fit")==0)
            {i++;
                fit = token[i].value;
                if(fit.compare("bf")!=0 && fit.compare("ff")!=0 && fit.compare("wf")!=0){
                    printf("ERROR!,mkdisk fit, no reconoce el ajuste.\n");
                    return;
                }
            }


        }
        if(strcmp(nomComando.c_str(),"rmdisk")==0)
           {
               if(strcmp(token[i].value,"-path")==0)
               {i++;
                   path = token[i].value;
                   comando.Rmdisk(path);
               }else
               printf("ERROR!,rmdisk, faltan parametros\n");

               return;
           }
        if(strcmp(nomComando.c_str(),"fdisk")==0)
        {
            if(strcmp(token[i].value,"-size")==0)
            {i++;
                size = atoi(token[i].value);
                if(size <= 0){
                    printf("ERROR!,fdisk, El tamaño de la partición debe ser positivo y mayor que cero.\n");
                    return;
                }
            }
            if(strcmp(token[i].value,"-unit")==0)
            {i++;
                unit = token[i].value[0];
                if(unit!='k' && unit !='m' && unit !='b'){
                    printf("ERROR!,fkdisk unit, Las unidades deben estar en m,k o b\n");
                    return;
                }


            }
            if(strcmp(token[i].value,"-path")==0)
            {i++;
                path = token[i].value;
            }
            if(strcmp(token[i].value,"-type")==0)
            {i++;
                type = token[i].value[0];
                if(type!='p' && type!='e' && type!='l'){
                    printf("ERROR!,fkdisk type, no se indico un tipo aceptado.\n");
                    return;
                }
            }
            if(strcmp(token[i].value,"-fit")==0)
            {i++;
                fit = token[i].value;
                if(fit.compare("bf")!=0 && fit.compare("ff")!=0 && fit.compare("wf")!=0){
                    printf("ERROR!,fkdisk fit, no reconoce el ajuste.\n");
                    return;
                }
            }
            if(strcmp(token[i].value,"-name")==0)
            {i++;
                name = token[i].value;
            }
            if(strcmp(token[i].value,"-delete")==0)
            {i++;
                cadType = token[i].value;
            }

            if(strcmp(token[i].value,"-add")==0)
            {i++;
                Addsize = atoi(token[i].value);                
            }

        }
        if(strcmp(nomComando.c_str(),"mount")==0){
            if(strcmp(token[i].value,"-path")==0)
            {i++;
                path = token[i].value;
            }
            if(strcmp(token[i].value,"-name")==0)
            {i++;
                name = token[i].value;
            }
        }
        if(strcmp(nomComando.c_str(),"umount")==0){

                if(strcmp(token[i].value,"-id")==0)
                {i++;
                    id = token[i].value;
                    comando.UnMountPartition(id.c_str());
                }else
                printf("ERROR!,unmount, faltan parametros\n");

                return;

        }
        if(strcmp(nomComando.c_str(),"rep")==0){

            if(strcmp(token[i].value,"-id")==0)
            {i++;
                id = token[i].value;
            }
            if(strcmp(token[i].value,"-path")==0)
            {i++;
                path = token[i].value;
            }
            if(strcmp(token[i].value,"-name")==0)
            {i++;
                name = token[i].value;
            }

        }

        if(strcmp(nomComando.c_str(),"mkfs")==0){
            if(strcmp(token[i].value,"-id")==0)
            {i++;
                id = token[i].value;
            }
            if(strcmp(token[i].value,"-type")==0)
            {i++;
                cadType = token[i].value;
                if(cadType.compare("fast")!=0 && cadType.compare("full")!=0){
                    printf("ERROR!,mkfs type, parametro invalido.\n");
                    return;
                }
            }
            if(strcmp(token[i].value,"-fs")==0)
            {i++;
                fs = token[i].value;
                if(fs.compare("2fs")!=0 && fs.compare("3fs")!=0){
                    printf("ERROR!,mkfs fs, parametro invalido.\n");
                    return;
                }
            }
        }if(strcmp(nomComando.c_str(),"login")==0){
            if(strcmp(token[i].value,"-usr")==0)
            {i++;
                usr = token[i].value;
            }
            if(strcmp(token[i].value,"-pwd")==0)
            {i++;
                pwd = token[i].value;
            }
            if(strcmp(token[i].value,"-id")==0)
            {i++;
                id = token[i].value;
            }
        }if(strcmp(nomComando.c_str(),"mkgrp")==0)
        {
            if(strcmp(token[i].value,"-name")==0)
            {i++;
                name = token[i].value;
                comando.mkgrp(name);
            }else
            printf("ERROR!,mkgrp, faltan parametros\n");

            return;
        }if(strcmp(nomComando.c_str(),"rmgrp")==0)
        {
            if(strcmp(token[i].value,"-name")==0)
            {i++;
                name = token[i].value;
                comando.rmgrp(name);
            }else
            printf("ERROR!,rmgrp, faltan parametros\n");

            return;
        }

    }//fin while

    if(strcmp(nomComando.c_str(),"mkdisk")==0){
        if(size!=0 && path.compare("")!=0){
            comando.Mkdisk(path,size,unit,fit);
        }else
            printf("ERROR!,mkdisk, Faltan parametros\n");

    }else if(strcmp(nomComando.c_str(),"fdisk")==0){
        //si es delete
        if(cadType.compare("")!=0){
            if(cadType.compare("fast")!=0 && cadType.compare("full")!=0)
                printf( "ERROR!,fdisk delete, valor incorrecto en delete.\n" );
            else if(path.compare("")!=0 && name.compare("")!=0)
                comando.FdiskDelete(path,name,cadType);
            else
                printf( "ERROR!,fdisk delete, Faltan parametros\n" );
            return;
        }else if(Addsize!=0){
            if(path.compare("")!=0 && name.compare("")!=0){
                comando.FdiskAdd(name,path,unit,Addsize);
            }else
                printf( "ERROR!,fdisk add, Faltan parametros\n" );
            return;
        }


        if(size!=0 && path.compare("")!=0 && name.compare("")!=0)
            comando.Fdisk(path,name,size,unit,type,fit[0]);
        else
            printf(  "ERROR!,fdisk, Faltan parametros\n" );

    }else if(strcmp(nomComando.c_str(),"mount")==0){
        if(path.compare("")!=0 && name.compare("")!=0){
            comando.Mount(path,name);
        }else
            cout<<"ERROR!,mount, faltan parametros"<<endl;
    }else if(strcmp(nomComando.c_str(),"rep")==0){
        if(path.compare("")!=0 && name.compare("")!=0 && id.compare("")!=0){
            comando.Rep(id,path,name);
            return;
        }else
            cout<<"ERROR!,reporte, faltan parametros"<<endl;
    }else if(strcmp(nomComando.c_str(),"mkfs")==0){
        if(id.compare("")!=0 && cadType.compare("")!=0){
            comando.mkfs(id,cadType);
            return;
        }else
            cout<<"ERROR!,mkfs, faltan parametros"<<endl;
    }else if(strcmp(nomComando.c_str(),"login")==0){
        if(id.compare("")!=0 && usr.compare("")!=0 && pwd.compare("")!=0){
            comando.login(id,usr,pwd);
            return;
        }else
            cout<<"ERROR!,login, faltan parametros"<<endl;
    }if(strcmp(nomComando.c_str(),"logout")==0){
        comando.logout();
        return;
    }

}


void Acciones::ejecutarScript(const char* path)
{
    char entrada[1000];memset(entrada,0,sizeof entrada);
    FILE *ptr_file;
    ptr_file = fopen(path,"r+");
    if(!ptr_file)
    {
        printf(  "ERROR! Archivo no existe\n" );
        return;
    }
    int i=0;
    while(*entrada !=EOF){
        char tok = (char)fgetc(ptr_file);
        if(tok=='\n' )
        {
            leerComando(entrada);
            memset(entrada,0,sizeof entrada);i=0;
        }else
        {
            entrada[i++] = tok;
        }
    }
    fclose(ptr_file);
    printf("\n");
}





