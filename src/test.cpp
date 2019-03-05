#include "dynamixelControl.h"
#include <iostream>
using namespace std;
int main(){
uint8_t arr[10];
uint8_t count;
uint8_t id;
char c;
DynamixelWorkbench dxl;
dynamixel_joint *joints[8];

dxl.begin("/dev/USB2Dynamixel",115200);


while(1){
if (dxl.scan(arr, &count, 30) != true)
{
  printf("Motors not found!\n");
  return 1;
}


printf("\n\n------INIT MOTORS-------\n");
for(int i=0;i<count;i++){
  joints[i] = new dynamixel_joint(arr[i],&dxl,200,50);
}


printf("\n\n------TORQUE ENABLE------\n");
for(int i=0;i<count;i++){
  joints[i]->torque(true);
}
printf("Run again?(Y/N):");
scanf("%c",&c);
switch(c){
  case 'y':
  case 'Y': break;
  case 'n':
  case 'N': return 0;
}
}
return 0;
}
