#ifndef MBR_H
#define MBR_H
#include"partition.h"
struct MBR{
    int mbr_tamano;
    time_t mbr_fecha_creacion;
    int mbr_disk_signature;
    char disk_fit;
    Partition mbr_partition1;
    Partition mbr_partition2;
    Partition mbr_partition3;
    Partition mbr_partition4;
};
#endif // MBR_H
