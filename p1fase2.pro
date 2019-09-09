TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    acciones.cpp \
    comando.cpp \
    funciones.cpp \
    reporte.cpp

HEADERS += \
    acciones.h \
    comando.h \
    librerias.h \
    mbr.h \
    partition.h \
    funciones.h \
    espacio.h \
    montaje.h \
    reporte.h \
    superbloque.h \
    bloques.h
