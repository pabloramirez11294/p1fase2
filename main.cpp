#include"acciones.h"

using namespace std;



int main()
{
    Acciones accion;
    do{
        __fpurge(stdin);
        printf("Comando:~$ " );
        char txt[1000];
        memset(txt,0,sizeof txt);

        fgets(txt, sizeof txt, stdin);
        accion.leerComando(txt);
    }while(1);

    return 0;
}


