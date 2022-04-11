#include "syscall.h"
int
main(){
    char a[255];
    ReadString(a,255);
    OpenFileID id=Open(a);
    char *buffer=new char[255];
    Read(buffer,255,id);
    PrintString(buffer);
    delete[] buffer;
    Close(id);
    Halt();
    return 0;
}