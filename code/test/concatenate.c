#include "syscall.h"
int
main(){
    char a[255];
    char b[255];
    char buffer[255];
    OpenFileId id;
    PrintString("Enter the name of file1: ");
    ReadString(a,255);
    PrintString("Enter the name of file2: ");
    ReadString(b,255);
    id=Open(a);
    
    Read(buffer,3,id);
    Close(id);
    id=Open(b);
    Seek(-1,id);
    Write(buffer,3,id);
    Close(id);
    Halt();
    return 0;
}