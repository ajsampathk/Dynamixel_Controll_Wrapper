#include "dynamixel_workbench.h"
#include <iostream>
using namespace std;
int main(){
uint8_t arr[10];
uint8_t count;
DynamixelWorkbench dxl;
dxl.begin("/dev/ttyUSB0",57600);
if (dxl.scan(arr, &count, 200) != true)
{
  cout<<"Not found Motors, Please check scan range or baud rate";
  return 1;
}
return 0;
}
