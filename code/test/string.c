/* sub.c
 *	Simple program to test whether the systemcall interface works.
 *	
 *	Just do a sub syscall that subs two values and returns the result.
 *
 */

#include "syscall.h"
char a[255];
int main()
{
    ReadString(a,255);
    PrintString(a);
    Halt();
}
