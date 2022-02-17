#include "syscall.h"

int 
main()
{
    //In ra bang ma ascii
    int i,j;
    PrintString("\t\tAscii Table\n");
    for(i = 32; i <= 50; i++)
    {
        for(j = 0; j < 5; j++)
        {
            PrintNum(i + j * 19);
            PrintString(" - ");
            PrintChar(i + j);
            PrintString("\t");
        }
    }
    PrintString("\n");
    Halt();
    return 0;
}