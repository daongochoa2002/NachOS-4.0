/* ExceNum.c
 *	Simple program to test whether the systemcall interface works.
 *	
 *	Just do a ReadNum syscall that read a integer from the console and returns the result,
 *  a PrintNum syscall that print that integer on the screen,
 *  and a RandomNum syscall that create a random number.
 *
 */

#include "syscall.h"
int
main()
{
  int result;
  //enter a number
  result = ReadNum();
  PrintNum(result);
  PrintChar('\n');
  //create a random number
  result = RandomNum();
  PrintNum(result);

  Halt();
  /* not reached */
}
