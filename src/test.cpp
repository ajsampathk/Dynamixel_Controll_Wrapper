#include "dynamixelControl.h"
#include <unistd.h>
#define max 26
struct leg_joints{
  float hip[max]       = {0.9845,0.9179,0.8635,0.8204,0.7916,	0.7817,	0.79831,	0.8641,	0.9502,	0.9298,	0.9174,	0.9131,	0.9166,	0.9273,	0.9444,	0.9669,	0.9938,	1.0238,1.0557,1.0877,1.1183,1.1455,1.1677,1.1829,1.0809,0.9854};//{0.7,0.9,0.6,0.5,0.4,0.3,0.2,0.1,0.1,0.3,0.4,0.5,0.6,0.7};
  float knee[max]      = {0.495951951425624,	0.620772796400778	,0.693658178197603,	0.722712921436723,	0.708000147157537	,0.645071106269018,	0.521376421553457,	0.282720077410796,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0.270225758969981,	0.493937235659942};//{0.7,0.8,0.6,0.3,0.2,0.2,0.2,0.1,0.1,0.2,0.3,0.4,0.5,0.7};
  float back_hip[max]  = {0.916687813018504,	0.927366428854339,	0.944431889524348,	0.966948808021099,	0.993844686372097,	1.02389891529280,	1.05572748663198,	1.08777702162261,	1.11833969127391,	1.14559790348431,	1.16770465901764,	1.18290297970996,	1.08095985823498,	0.984523826286143,	0.917974995515905,	0.863525915373826,	0.820423688530905,	0.791608705522878,	0.781788601627923,	0.798316226823173,	0.864115598913156,	0.950228531165074,	0.929842491215872,	0.917463098537095,	0.913186073114148,	0.916687813018504};//{0.7,0.7,0.8,0.9,0.9,0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1};
  float back_knee[max] = {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0.270225758969981,	0.495951951425624,	0.620772796400778,	0.693658178197603,	0.722712921436723,	0.708000147157537,	0.645071106269018,	0.521376421553457,	0.282720077410796,	0,	0,	0,	0,	0};//{0.7,0.7,0.8,0.7,0.6,0.5,0.4,0.5,0.6,0.7,0.8,0.7,0.7,0.7};
};



bool update_goal(DynamixelWorkbench *dxl,uint8_t *motors,uint8_t n,int32_t *goals,bool log=true){
  bool added;
  dxl->initBulkWrite();

  for(int i=0;i<n;i++){
    added=dxl->addBulkWriteParam(motors[i] , "Goal_Position",  goals[i]);
    if(log)printf("Update [ID:%d]:%s\n",motors[i], added ? "True" : "False");
  }
  return dxl->bulkWrite();
}

bool read_pos(DynamixelWorkbench *dxl,uint8_t *motors,uint8_t n,float *positions,bool log=true){
  bool read;
  dxl->initBulkRead();
  for(int i=0;i<n;i++){
  read=dxl->addBulkReadParam(motors[i], "Present_Position");
  if(!read){
    if(log)printf("[Error]Reading Positions\n");
    return false;
  }
  }
  dxl->setBulkRead();
  for(int i=0;i<n;i++){
    positions[i] = dxl->convertValue2Radian(motors[i],dxl->bulkRead(motors[i],"Present_Position"));
    if(log)printf("[ID:%d]-Position:%f\n",motors[i],positions[i]);
  }
  return true;
}


int main(){
  uint8_t arr[10];
  uint8_t count;
  char val;
  int32_t _val[8];
  struct leg_joints legs;
  float current_pos[10];

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
  joints[i] = new dynamixel_joint(arr[i],&dxl,100,25);
  }


  printf("\n\n------TORQUE ENABLE------\n");
  for(int i=0;i<count;i++){
  joints[i]->torque(true);
  }




float angle=0.0;
_val[0] = dxl.convertRadian2Value(arr[0],-angle);
_val[1] = dxl.convertRadian2Value(arr[1],angle);
_val[2] = dxl.convertRadian2Value(arr[2],-angle);
_val[3] = dxl.convertRadian2Value(arr[3],angle);
_val[4] = dxl.convertRadian2Value(arr[4],-angle);
_val[5] = dxl.convertRadian2Value(arr[5],-angle);
_val[6] = dxl.convertRadian2Value(arr[6],angle);
_val[7] = dxl.convertRadian2Value(arr[7],angle);

returned=update_goal(&dxl, arr, count, _val);
printf("Initialized?:%s\n", returned ? "True" : "False");
printf("\n\n-------Positions----------\n");
read_pos(&dxl, arr, count, current_pos);
printf("Continue?(Y/N):");
scanf("%c",&val);
switch(val){
  case 'y':
  case 'Y': break;
  case 'n':
  case 'N': return 0;
}

int i=0;


while(1){


 _val[0] = dxl.convertRadian2Value(arr[0],-legs.hip[i%max]);
 _val[1] = dxl.convertRadian2Value(arr[1],legs.back_hip[i%max]);
 _val[2] = dxl.convertRadian2Value(arr[2],-legs.back_hip[i%max]);
 _val[3] = dxl.convertRadian2Value(arr[3],legs.hip[i%max]);
 _val[4] = dxl.convertRadian2Value(arr[4],-legs.knee[i%max]);
 _val[5] = dxl.convertRadian2Value(arr[5],-legs.back_knee[i%max]);
 _val[6] = dxl.convertRadian2Value(arr[6],legs.back_knee[i%max]);
 _val[7] = dxl.convertRadian2Value(arr[7],legs.knee[i%max]);

  i++;

returned=update_goal(&dxl, arr, count, _val,false);

printf("Updated?:%s\n", returned ? "True" : "False");

while(joints[0]->moving()||joints[1]->moving()||joints[2]->moving()||joints[3]->moving());
read_pos(&dxl, arr, count, current_pos);
}
}
