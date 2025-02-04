#ifndef SUPERBLOQUE_H
#define SUPERBLOQUE_H
#include<string>
using namespace std;
struct Superbloque
{
    int s_inodes_count;
    int s_blocks_count;
    int s_free_blocks_count;
    int s_free_inodes_count;
    time_t s_mtime;
    time_t s_umtime;
    int s_mnt_count;
    int s_magic;
    int s_inode_size;
    int s_block_size;
    int s_firts_ino;
    int s_first_blo;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;

};
struct Journaling
{
    char cadena[200];
};
struct Inodo
{
    int i_uid;
    int i_gid;
    int i_size;
    time_t i_atime;
    time_t i_ctime;
    time_t i_mtime;
    int i_block[15];
    char i_type;
    int i_perm;
};
struct Bitmap{
    char* bits;
};
struct Log_usuario
{
    string user="";
    string pass="";
    int log=0;
    int uid;
    int gid;
    int partStart;
    string path;
    string partName;
    Superbloque superbloque;
};
struct Usuario{
    int id;
    string tipo;
    string grupo;
    string usr;
    string pwd;
};
#endif // SUPERBLOQUE_H
