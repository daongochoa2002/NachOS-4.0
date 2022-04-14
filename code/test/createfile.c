#include "syscall.h"
int
main(){
    char a[255];
    PrintString("Enter the file name: ");
    ReadString(a,255);
    if(Create(a)==0){
        PrintString("Created file successfully!");
    }
    else
    PrintString("Created file unsuccessfully!");
    Halt();
    return 0;
}