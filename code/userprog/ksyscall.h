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
#include <stdlib.h>



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
  do {
    temp = kernel->synchConsoleIn->GetChar();
  } while(temp == ' ' || temp == '\n');
  //check if the number is negative
  if(temp == '-') isSign=true;
  //check if the number is digit
  else if('0'<=temp&&temp<='9') num = temp - '0';
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
  return kernel->synchConsoleIn->GetChar();
}

void SysPrintChar(char character)
{
  kernel->synchConsoleOut->PutChar(character);
}

void SysReadString(char *buffer, int length)
{
    int index;
    char ch;
    for (index = 0; index < length; ++index)
        buffer[index] = 0;
    for (index = 0; index < length;)
    {
        do
        {
            ch = kernel->synchConsoleIn->GetChar();
        } while (ch == EOF);
        if (ch == '\001' || ch == '\n') // End the input by Enter
            break;
        buffer[index++] = ch;
    }
}

void SysPrintString(char *buffer)
{
    int len = 0;
    while (buffer[len]) // loop until meet '\0'
    {
        kernel->synchConsoleOut->PutChar(buffer[len++]);
    }
}

unsigned int SysRandomNum(){
  RandomInit(time(0));
  return RandomNumber();
}
//The create system call will use the
//Nachos Filesystem Object Instance to create a zero length file. 
int SysCreate(char *name)
{
  if (strlen(name) == 0)
  {
    DEBUG(dbgSys, "\nError! File name can't be empty");
    return -1;
  } else if (name == NULL)
  {
    DEBUG(dbgSys, "\nError! Not enough memory in system");
    return -1;
  } else
  {
    if (!kernel->fileSystem->Create(name))
    {
      DEBUG(dbgSys, "\nError creating file");
      return -1;
    } else
    {
       DEBUG(dbgSys, "\nCreated file successfully");
       return 0;
    }
  }
}
//The Open system call returns the file descriptor id
//(OpenFileID == an integer number), or –1 if the call fails. Open can fail for several reasons,
//such as trying to open a file or mailbox that does not exist or if there is not enough room in the
//file descriptor table. 
OpenFileId SysOpen(char *name)
{
  Open
  return (OpenFileId) kernel->fileSystem->Open(name,0);
}

int SysClose(OpenFileId id)
{
  return (int) kernel->fileSystem->Close(id);
}
int SysRemove(char*name){
  return (int) kernel->fileSystem->Remove(name);
}

int SysWrite(char *buffer, int size, OpenFileId id){
  if(id==1)
    return kernel->synchConsoleOut->PutString(buffer,size);
  return kernel->fileSystem->Write(buffer,size,id);
  
}

int SysRead(char *buffer, int size, OpenFileId id){
  if(id==0)
    return kernel->synchConsoleIn->GetString(buffer,size);
  return kernel->fileSystem->Read(buffer,size,id);
  
}

int SysSeek(int position, OpenFileId id){
  if (id <= 1) {
    DEBUG(dbgSys, "\nCan't seek in console");
    return -1;
  }
  return kernel->fileSystem->Seek(position, id);
}
#endif /* ! __USERPROG_KSYSCALL_H__ */
