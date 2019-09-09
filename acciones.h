#ifndef ACCIONES_H
#define ACCIONES_H
#include"comando.h"
class Acciones
{
public:
    Comando comando;
    struct Token{
        char value[100];
    };
    //leer y ejecutar comandos
    void leerComando(char* entrada);
    int getToken(char* cmd, int i,char* token);
    int getSpaceToken(char* cmd, int i,char* token);
    void ejecutarComando(struct Token token[]);
    void ejecutarScript(const char* path);

};

#endif // ACCIONES_H
