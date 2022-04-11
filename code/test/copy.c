#include "syscall.h"
int
main(){
    char a[255];
    char b[255];
    PrintString("Enter the name of file1: ");
    ReadString(a,255);
    PrintString("Enter the name of file2: ");
    ReadString(b,255);
    OpenFileID id=Open(a);
    char *buffer=new char[255];
    Read(buffer,255,id);
    Close(id);
    id=Open(b);
    Write(buffer,255,id);
    Close(id);
    delete[] buffer;
    Halt();
    return 0;
}