// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------
void IncreasePC(){
	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}
	return;
}

/* 
Input: - User space address (int) 
 - Limit of buffer (int) 
Output:- Buffer (char*) 
Purpose: Copy buffer from User memory space to System memory space 
*/ 
char* User2System(int virtAddr,int limit) 
{ 
	int i;// index 
	int oneChar; 
	char* kernelBuf = NULL; 
	
	kernelBuf = new char[limit +1];//need for terminal string 
	if (kernelBuf == NULL) 
		return kernelBuf; 
	memset(kernelBuf,0,limit+1); 
	
	//printf("\n Filename u2s:"); 
	for (i = 0 ; i < limit ;i++) 
	{ 
		kernel->machine->ReadMem(virtAddr+i,1,&oneChar); 
		kernelBuf[i] = (char)oneChar; 
		//printf("%c",kernelBuf[i]); 
		if (oneChar == 0) 
			break; 
	} 
	return kernelBuf; 
}

/* 
Input: - User space address (int) 
 - Limit of buffer (int) 
 - Buffer (char[]) 
Output:- Number of bytes copied (int) 
Purpose: Copy buffer from System memory space to User memory space 
*/ 
int System2User(int virtAddr,int len,char* buffer) 
{ 
	if (len < 0) 	return -1; 
	if (len == 0)	return len; 
	int i = 0; 
	int oneChar = 0 ; 
	do{ 
		oneChar= (int) buffer[i]; 
		kernel->machine->WriteMem(virtAddr+i,1,oneChar); 
		i ++;
	}while(i < len && oneChar != 0); 
	return i; 
} 

void
ExceptionHandler(ExceptionType which)
{
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which) {
    case SyscallException:
      switch(type) {
      	case SC_Halt:
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

			SysHalt();

			ASSERTNOTREACHED();
			break;

      	case SC_Add:
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
	
	/* Process SysAdd Systemcall*/
			int result;
			result = SysAdd(/* int op1 */(int)kernel->machine->ReadRegister(4),
			/* int op2 */(int)kernel->machine->ReadRegister(5));

			DEBUG(dbgSys, "Add returning with " << result << "\n");
	/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);
	
			IncreasePC();

			return;

			ASSERTNOTREACHED();

			break;
		case SC_Sub:
			DEBUG(dbgSys, "Sub " << kernel->machine->ReadRegister(4) << " - " << kernel->machine->ReadRegister(5) << "\n");
	
			/* Process SysSub Systemcall*/
			int res;
			res = SysSub(/* int op1 */(int)kernel->machine->ReadRegister(4),
			/* int op2 */(int)kernel->machine->ReadRegister(5));

			DEBUG(dbgSys, "Sub returning with " << res << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)res);
	
			IncreasePC();

			return;
	
			ASSERTNOTREACHED();

			break;
		case SC_ReadNum:
		{
			DEBUG(dbgSys, "Read a number " << kernel->machine->ReadRegister(4) << "\n");
	
			/* Process SysReadNum Systemcall*/
			int number;
			number = SysReadNum();

			DEBUG(dbgSys, "The entered number is " << number << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)number);
	
			IncreasePC();

			return;
	
			ASSERTNOTREACHED();

			break;
		}
		case SC_PrintNum:
		{
			DEBUG(dbgSys, "Print a number " << "\n");
	
			/* Process SysPrintNum Systemcall*/
			SysPrintNum(/* int number */(int)kernel->machine->ReadRegister(4));
	
			IncreasePC();

			return;
	
			ASSERTNOTREACHED();

			break;
		}
		case SC_ReadChar:
		{
			int res=SysReadChar();
			kernel->machine->WriteRegister(2, (int)res);
								
			IncreasePC();
			
			return;
	
			ASSERTNOTREACHED();

			break;
		}
		case SC_PrintChar:	
		{	
			SysPrintChar((char)kernel->machine->ReadRegister(4));
			
			IncreasePC();
			
			return;
	
			ASSERTNOTREACHED();


			break;
		}
		case SC_ReadString:
		{
			int virtAddr;
			int length;
			char* buffer;
		
			virtAddr = kernel->machine->ReadRegister(4);
			length = kernel->machine->ReadRegister(5);
			buffer = User2System(virtAddr, length);

			SysReadString(buffer, length);
			System2User(virtAddr, length, buffer);
			delete buffer;

			IncreasePC();

			return;
	
			ASSERTNOTREACHED();

			break;
		}
		case SC_PrintString:
		{
			int virtAddr1;
			char* buffer1;
			virtAddr1 = kernel->machine->ReadRegister(4);
			buffer1 = User2System(virtAddr1, 255);
			SysPrintString(buffer1);
			delete[] buffer1;
			IncreasePC();

			return;
	
			ASSERTNOTREACHED();

			break;
		}
		case SC_RandomNum:
		{
			DEBUG(dbgSys, "Create a random number " << "\n");
	
			/* Process SysRandomNum Systemcall*/
			int num;
			num = SysRandomNum();
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)num);

			IncreasePC();

			return;
	
			ASSERTNOTREACHED();

			break;
		}
		//The createfile system call will use the
//Nachos Filesystem Object Instance to create a zero length file. Remember, the filename exists in
//user space. This means the buffer that the user space pointer points to must be translated from
//user memory space to system memory space. The createfile system call returns 0 for successful
//completion, -1 for an error. 
		case SC_Create:
		{
			int virtAddr2 = kernel->machine->ReadRegister(4);
			//int length = kernel->machine->ReadRegister(5);
			char * fileName = User2System(virtAddr2, 255);

			kernel->machine->WriteRegister(2, SysCreate(fileName));
			
			delete[] fileName;
			IncreasePC();

			return;
	
			ASSERTNOTREACHED();

			break;
		}

		case SC_Open:
		{
			int virtAddr3 = kernel->machine->ReadRegister(4);
			//int length = kernel->machine->ReadRegister(5);
			char * fileName = User2System(virtAddr3, 255);

			kernel->machine->WriteRegister(2, SysOpen(fileName));

			delete[] fileName;
			IncreasePC();

			return;
	
			ASSERTNOTREACHED();

			break;
		}
		case SC_Close:
		{
			int id = kernel->machine->ReadRegister(4);
			kernel->machine->WriteRegister(2, SysClose(id));

			IncreasePC();

			return;
	
			ASSERTNOTREACHED();

			break;
		}
		case SC_Remove:
		{
			int virtAddr = kernel->machine->ReadRegister(4);
			//int length=kernel->machine->ReadRegister(5);
			char * fileName=User2System(virtAddr,255);
			int ans = SysRemove(fileName);
			kernel->machine->WriteRegister(2,(int)ans);
			IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}
		case SC_Read:
		{
			int virtAddr5 = kernel->machine->ReadRegister(4);
            int size = kernel->machine->ReadRegister(5);
            int id1 = kernel->machine->ReadRegister(6);

            char *buffer = User2System(virtAddr5, size);

            int res = SysRead(buffer, size, id1);

            kernel->machine->WriteRegister(2, res);
            if (res != -1 && res != -2)
                System2User(virtAddr5, res, buffer);

            delete buffer;
			IncreasePC();

			return;
	
			ASSERTNOTREACHED();

			break;
		}
		case SC_Write:
		{
			int virtAddr6 = kernel->machine->ReadRegister(4);
            int size2 = kernel->machine->ReadRegister(5);
            int id = kernel->machine->ReadRegister(6);

            char *buffer = User2System(virtAddr6, size2);

            int answer= SysWrite(buffer, size2, id);

            kernel->machine->WriteRegister(2, (int)answer);
            if (answer != -1 && answer != -2)
                System2User(virtAddr6, answer, buffer);

            delete buffer;
			buffer=NULL;
            IncreasePC();

			return;
	
			ASSERTNOTREACHED();

			break;
		}
		case SC_Seek:
	  	{
			int pos=kernel->machine->ReadRegister(4);
			int id=kernel->machine->ReadRegister(5);
			int res=SysSeek(pos,id);
			kernel->machine->WriteRegister(2,(int) res);
			IncreasePC();

			return;
	
			ASSERTNOTREACHED();

			break;
		}
      	default:{
			cerr << "Unexpected system call " << type << "\n";
			break;
		}
	  }
      	break;
    default:
      cerr << "Unexpected user mode exception" << (int)which << "\n";
      break;
    }
    ASSERTNOTREACHED();
	
}
