/* add.c
 *	Simple program to test whether the systemcall interface works.
 *	
 *	Just do a add syscall that adds two values and returns the result.
 *
 */

#include "syscall.h"
#define SIZE 100
int arr[SIZE];
int
main()
{
  int opt;
  opt = ReadNum();
  //sort in increasing order
  if(opt == 1){
      
  }
  Halt();
  /* not reached */
}
