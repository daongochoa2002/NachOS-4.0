#include "syscall.h"

int main()
{
    //In ra bang ma ascii
    PrintString("\t\tAscii Table\n")
    for (int i = 32; i <= 50; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            PrintNum(i + j * 19);
            PrintString(" - ");
            PrintChar(i + j);
            PrintString("\t");
        }
        PrintString("\n");
    }

    return 0;
}