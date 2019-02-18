#include "dynamixelControl.h"
#include <unistd.h>
struct leg_joints{
  float hip[20]       = {0.7,0.9,0.6,0.5,0.4,0.3,0.2,0.1,0.1,0.3,0.4,0.5,0.6,0.7};
  float knee[20]      = {0.7,0.8,0.6,0.3,0.2,0.2,0.2,0.1,0.1,0.2,0.3,0.4,0.5,0.7};
  float back_hip[20]  = {0.7,0.7,0.8,0.9,0.9,0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1};
  float back_knee[20] = {0.7,0.7,0.8,0.7,0.6,0.5,0.4,0.5,0.6,0.7,0.8,0.7,0.7,0.7};
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
  float val;
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
  joints[i] = new dynamixel_joint(arr[i],&dxl,25,5);
  }


  printf("\n\n------TORQUE ENABLE------\n");
  for(int i=0;i<count;i++){
  joints[i]->torque(true);
  }

  printf("\n\n-------Positions----------\n");
  read_pos(&dxl, arr, count, current_pos);



float angle=0.6;
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
printf("Value:");
scanf("%f",&val);

int i=0;


while(1){


 _val[0] = dxl.convertRadian2Value(arr[0],-legs.hip[i%14]);
 _val[1] = dxl.convertRadian2Value(arr[1],legs.back_hip[i%14]);
 _val[2] = dxl.convertRadian2Value(arr[2],-legs.back_hip[i%14]);
 _val[3] = dxl.convertRadian2Value(arr[3],legs.hip[i%14]);
 _val[4] = dxl.convertRadian2Value(arr[4],-legs.knee[i%14]);
 _val[5] = dxl.convertRadian2Value(arr[5],-legs.back_knee[i%14]);
 _val[6] = dxl.convertRadian2Value(arr[6],legs.back_knee[i%14]);
 _val[7] = dxl.convertRadian2Value(arr[7],legs.knee[i%14]);

  i++;

returned=update_goal(&dxl, arr, count, _val,false);

printf("Updated?:%s\n", returned ? "True" : "False");

while(joints[0]->moving()||joints[1]->moving()||joints[2]->moving()||joints[3]->moving());

read_pos(&dxl, arr, count, current_pos);
}
}
