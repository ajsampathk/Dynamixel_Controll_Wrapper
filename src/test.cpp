#include "dynamixelControl.h"
#include <unistd.h>
struct leg_joints{
  float hip[20]       = {0.7,0.9,0.6,0.5,0.4,0.3,0.2,0.1,0.1,0.3,0.4,0.5,0.6,0.7};
  float knee[20]      = {0.7,0.8,0.6,0.3,0.2,0.2,0.2,0.1,0.1,0.2,0.3,0.4,0.5,0.7};
  float back_hip[20]  = {0.7,0.7,0.8,0.9,0.9,0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1};
  float back_knee[20] = {0.7,0.7,0.8,0.7,0.6,0.5,0.4,0.5,0.6,0.7,0.8,0.7,0.7,0.7};
};



int main(){
uint8_t arr[10];
uint8_t count;
float val;
int32_t _val[8];
struct leg_joints legs;

bool added,initialized ,returned;
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
  joints[i] = new dynamixel_joint(arr[i],&dxl,50,10);
}
printf("\n\n------TORQUE ENABLE------\n");
for(int i=0;i<count;i++){
  joints[i]->torque(true);
}


printf("\n\n--------syncMove---------\n");
bool read[8];
dxl.initBulkRead();
for(int i=0;i<8;i++)
read[i]=dxl.addBulkReadParam(arr[i], "Present_Position");
dxl.setBulkRead();
printf("\n\n-------POSITION----------\n");
float _read;
for(int i=0;i<count;i++){
  _read = dxl.convertValue2Radian(arr[i],dxl.bulkRead(arr[i],"Present_Position"));
  printf("[ID:%d]:%f\n",arr[i],_read);
}//initial value
//dxl.initBulkRead();
//for(int i=0;i<8;i++)
//read[i]=dxl.addBulkReadParam(arr[i], "Present_Position");
float angle=0.6;
_val[0] = dxl.convertRadian2Value(arr[0],-angle);
_val[1] = dxl.convertRadian2Value(arr[1],angle);
_val[2] = dxl.convertRadian2Value(arr[2],-angle);
_val[3] = dxl.convertRadian2Value(arr[3],angle);
_val[4] = dxl.convertRadian2Value(arr[4],-angle);
_val[5] = dxl.convertRadian2Value(arr[5],-angle);
_val[6] = dxl.convertRadian2Value(arr[6],angle);
_val[7] = dxl.convertRadian2Value(arr[7],angle);
dxl.initBulkWrite();
added=dxl.addBulkWriteParam(arr[0] , "Goal_Position",  _val[0]);
printf("added[ID:%d]:%s\n",arr[0], added ? "True" : "False");
added=dxl.addBulkWriteParam(arr[1] , "Goal_Position",  _val[1]);
printf("added[ID:%d]:%s\n",arr[1], added ? "True" : "False");
added=dxl.addBulkWriteParam(arr[2] , "Goal_Position", _val[2]);
printf("added[ID:%d]:%s\n",arr[2], added ? "True" : "False");
added=dxl.addBulkWriteParam(arr[3] , "Goal_Position",  _val[3]);
printf("added[ID:%d]:%s\n",arr[3], added ? "True" : "False");

added=dxl.addBulkWriteParam(arr[4] , "Goal_Position",  _val[4]);
printf("added[ID:%d]:%s\n",arr[4], added ? "True" : "False");
added=dxl.addBulkWriteParam(arr[5] , "Goal_Position", _val[5]);
printf("added[ID:%d]:%s\n",arr[5], added ? "True" : "False");
added=dxl.addBulkWriteParam(arr[6] , "Goal_Position",  _val[6]);
printf("added[ID:%d]:%s\n",arr[6], added ? "True" : "False");
added=dxl.addBulkWriteParam(arr[7] , "Goal_Position",  _val[7]);
printf("added[ID:%d]:%s\n",arr[7], added ? "True" : "False");

returned=dxl.bulkWrite();
printf("intial position attained:%s\n", returned ? "True" : "False");
printf("Value:");
scanf("%f",&val);

int i=0;
while(1){


//initialized =


dxl.initBulkRead();
for(int i=0;i<8;i++)
read[i]=dxl.addBulkReadParam(arr[i], "Present_Position");

 _val[0] = dxl.convertRadian2Value(arr[0],-legs.hip[i%14]);
 _val[1] = dxl.convertRadian2Value(arr[1],legs.back_hip[i%14]);
 _val[2] = dxl.convertRadian2Value(arr[2],-legs.back_hip[i%14]);
 _val[3] = dxl.convertRadian2Value(arr[3],legs.hip[i%14]);
 _val[4] = dxl.convertRadian2Value(arr[4],-legs.knee[i%14]);
 _val[5] = dxl.convertRadian2Value(arr[5],-legs.back_knee[i%14]);
 _val[6] = dxl.convertRadian2Value(arr[6],legs.back_knee[i%14]);
 _val[7] = dxl.convertRadian2Value(arr[7],legs.knee[i%14]);

  i++;

dxl.initBulkWrite();
added=dxl.addBulkWriteParam(arr[0] , "Goal_Position",  _val[0]);
printf("added[ID:%d]:%s\n",arr[0], added ? "True" : "False");
added=dxl.addBulkWriteParam(arr[1] , "Goal_Position",  _val[1]);
printf("added[ID:%d]:%s\n",arr[1], added ? "True" : "False");
added=dxl.addBulkWriteParam(arr[2] , "Goal_Position", _val[2]);
printf("added[ID:%d]:%s\n",arr[2], added ? "True" : "False");
added=dxl.addBulkWriteParam(arr[3] , "Goal_Position",  _val[3]);
printf("added[ID:%d]:%s\n",arr[3], added ? "True" : "False");

added=dxl.addBulkWriteParam(arr[4] , "Goal_Position",  _val[4]);
printf("added[ID:%d]:%s\n",arr[4], added ? "True" : "False");
added=dxl.addBulkWriteParam(arr[5] , "Goal_Position", _val[5]);
printf("added[ID:%d]:%s\n",arr[5], added ? "True" : "False");
added=dxl.addBulkWriteParam(arr[6] , "Goal_Position",  _val[6]);
printf("added[ID:%d]:%s\n",arr[6], added ? "True" : "False");
added=dxl.addBulkWriteParam(arr[7] , "Goal_Position",  _val[7]);
printf("added[ID:%d]:%s\n",arr[7], added ? "True" : "False");

returned=dxl.bulkWrite();
printf("closed:%s\n", returned ? "True" : "False");

while(joints[0]->moving()||joints[1]->moving()||joints[2]->moving()||joints[3]->moving());

dxl.setBulkRead();
float _read;
for(int i=0;i<count;i++){
  _read = dxl.convertValue2Radian(arr[i],dxl.bulkRead(arr[i],"Present_Position"));
  printf("[ID:%d]:%f\n",arr[i],_read);
}

}
}
