#include "syscall.h"
int arr[100];
int
main()
{
  int n,i,opt,j;
  PrintString("Enter the number of elements: ");
  n= ReadNum();
  if(n >100||n<=0){
    PrintString("Invalid number!");
    Halt();
    return 0;
  }
  //Enter elements
  for(i = 0; i < n; i++){
    arr[i] = ReadNum();
  }
  PrintString("Do you want to sort in increasing or decreasing order?\n");
  PrintString("Enter 1 to sort in increasing order.\n");
  PrintString("Enter 2 to sort in decreasing order.\n");
  //int opt;
  opt = ReadNum();
  //sort in increasing order
  if(opt == 1){
      for(i =0; i<n; i++){
        for( j=n - 1; j > i; j--){
          if(arr[i] > arr[j]){
            int tmp =arr[i];
            arr[i] =arr[j];
            arr[j] = tmp;
          }
        }
      }
  }
  //in decreasing order
  else if(opt == 2){
      for(i =0; i<n; i++){
        for( j=n - 1; j > i; j--){
          if(arr[i] < arr[j]){
            int tmp =arr[i];
            arr[i] =arr[j];
            arr[j] = tmp;
          }
        }
      }
  }else{
    PrintString("Wrong Option!\n");
  }
  PrintString("The array after being ordered is: ");
  for(i = 0; i< n; i++){
    PrintNum(arr[i]);
    PrintChar(' ');
  }
  Halt();
  /* not reached */
  return 0;
}