#include "dynamixelControl.h"
#include <iostream>
using namespace std;
int main(){
uint8_t arr[10];
uint8_t count;
uint8_t id;
float val;
DynamixelWorkbench dxl;
dynamixel_joint *joints[8];
dxl.begin("/dev/ttyUSB0",115200);
if (dxl.scan(arr, &count, 30) != true)
{
  printf("Not found Motors, Please check scan range or baud rate\n");
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
int x;
while(1){
  for(int i=0;i<count;i++){
  joints[i]->Move(0);
//cin>>id>>val;
//x=joints[id]->Move(val);
}
// printf("\n\n-------POSITION----------\n");
// for(int i=0;i<count;i++){
//   printf("[ID:%d]:%f\n",arr[i],joints[i]->read());
// }
return 0;
}
}
