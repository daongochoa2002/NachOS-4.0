#include "syscall.h"
int
main(){
    char a[255];
    PrintString("Enter the file name: ");
    ReadString(a,255);
    if(Remove(a)==1){
        PrintString("Deleted file successfully!");
    }
    else
    PrintString("Deleted file unsuccessfully!");
    Halt();
    return 0;
}