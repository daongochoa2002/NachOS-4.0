#include "syscall.h"
int
main(){
    char a[255];
    char buffer[255];
    OpenFileId id;
    PrintString("Enter the file name: ");
    ReadString(a,255);
    id=Open(a);
    Seek(0,id);
    Read(buffer,3,id);
    PrintString(buffer);
    Close(id);
    Halt();
    return 0;
}