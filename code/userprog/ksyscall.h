/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"
#include "synchconsole.h"
#include "machine.h"


void SysHalt()
{
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

int SysSub(int op1, int op2)
{
  return op1 - op2;
}

int SysReadNum(){
  int num = 0;
  char temp;
  bool isSign = false, isNum = true;
  //ignore the space and endline
  for(;;){
    temp = kernel->synchConsoleIn->GetChar();
    if(temp == ' ' || temp == '\n')
      continue;
    else break;
  } 
  //check if the number is negative
  if(temp == '-') isSign=true;
  //check if the number is digit
  else if(('0'<=temp&&temp<='9')) num = temp - '0';
  else if(temp != '+') isNum = false;
  //get characters from the console
  for(;;){
    temp = kernel->synchConsoleIn->GetChar();
    if(temp == ' ' || temp == '\n')
      break;
    else if(!('0'<=temp&&temp<='9')) isNum = false;
    else num = num * 10 + temp - '0';
        
  }
  if(!isNum)  return 0;
  if(isSign) return -num;
  return num;
}

void SysPrintNum(int n){
  if(n < 0){
    kernel->synchConsoleOut->PutChar('-');
    n=-n;
  }
  //reverse number of n
  char temp[11];
  int i = 0;
  memset(temp,'\0',11);
  if(n == 0) kernel->synchConsoleOut->PutChar('0');
  //get the reversed number's value
  while(n != 0){
    temp[i++] = 48 + n % 10;
    n/=10;
  }
  //print the value of number on the console
  for(int j = i -1; j >= 0; j--)
    kernel->synchConsoleOut->PutChar(temp[j]);

}
char SysReadChar()
{
  return kernel->ConsoleGetChar();
}

void SysPrintChar(char character)
{
  kernel->ConsolePutChar(character);
}

unsigned int SysRandomNum(){
  RandomInit(time(0));
  return RandomNumber();
}
#endif /* ! __USERPROG_KSYSCALL_H__ */
