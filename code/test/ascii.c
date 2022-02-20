#include "syscall.h"

int 
main()
{
    //In ra bang ma ascii
    int i,j;
    PrintString("\t\tAscii Table\n");
    for(i = 33; i <= 51; i++)
    {
        for(j = 0; j < 5; j++)
        {
            PrintNum(i + j * 19);
            PrintString(" - ");
            PrintChar(i + j*19) ;
            PrintString("\t");
        }
        PrintString("\n");
    }
    PrintString("\n");
    Halt();
    return 0;
}