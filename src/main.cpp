#include "dynamixelControl.h"
#include <unistd.h>
#define max 8
struct leg_joints{
  float hip[max]       =
  // {0.78192,1.0782,1.1312,1.1745,0.94197,0.94197,0.88734,0.84245,0.80706,0.78192};//TESTED
  {0.78192,1.0916,1.1549,1.0721,1.0721,0.94191,0.85104,0.78192};//TESTED P8
  // {0.96327,1.0446,1.1549,0.97514,0.97514,0.9196,0.98644,0.96327};

  float knee[max]      =
  // {0.41314,0,0,0,0.42066,0.42066,0.44303,0.44909,0.43946,0.41314};//TESTED
  {0.41314,0,0,0.20077,0.20077,0.32828,0.38911,0.41314};//TESTED P8
  // {0,0,0,0.45108,0.45108,0.3822,0.11332,0};

  float back_hip[max]  =
  // {0.94197,0.88734,0.84245,0.80706,0.78192,0.78192,1.0782,1.1312,1.1745,0.94197};//TESTED
  {1.0721,0.94191,0.85104,0.78192,0.78192,1.0916,1.1549,1.0721};//TESTED P8
  // {0.97514,0.9196,0.98644,0.96327,0.96327,1.0446,1.1549,0.97514};

  float back_knee[max] =
  // {0.42066,0.44303,0.44909,0.43946,0.41314,0.41314,0,0,0,0.42066};//TESTED
  {0.20077,0.32828,0.38911,0.41314,0.41314,0,0,0.20077};//TESTED P8
  // {0.45108,0.3822,0.11332,0,0,0,0,0.45108};

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
  bool teach=false;
  uint8_t arr[10];
  uint8_t count;
  char val;
  int32_t _val[8];
  struct leg_joints legs;
  float current_pos[10];

  bool added,initialized ,returned;
  DynamixelWorkbench dxl;
  dynamixel_joint *joints[8];

  dxl.begin("/dev/USB2Dynamixel",115200);

  if (dxl.scan(arr, &count, 30) != true)
  {
  printf("Motors not detected!\n");
  return 1;
  }

  printf("\n\n------INIT MOTORS------\n");
  for(int i=0;i<count;i++){
  joints[i] = new dynamixel_joint(arr[i],&dxl,250,100);
  }


  printf("\n\n------TORQUE ENABLE------\n");
  for(int i=0;i<count;i++){
  joints[i]->torque(true);
  }




float hip_angle=0.0,knee_angle=0.0;

_val[0] = dxl.convertRadian2Value(arr[0],-hip_angle);
// _val[1] = dxl.convertRadian2Value(arr[1],hip_angle);
// _val[2] = dxl.convertRadian2Value(arr[2],-hip_angle);
// _val[3] = dxl.convertRadian2Value(arr[3],hip_angle);
// _val[4] = dxl.convertRadian2Value(arr[4],-knee_angle);
// _val[5] = dxl.convertRadian2Value(arr[5],-knee_angle);
// _val[6] = dxl.convertRadian2Value(arr[6],knee_angle);
// _val[7] = dxl.convertRadian2Value(arr[7],knee_angle);

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


 _val[0] = dxl.convertRadian2Value(arr[0],-legs.hip[(i%max)]);
 _val[1] = dxl.convertRadian2Value(arr[1],legs.back_hip[(i%max)]);
 _val[2] = dxl.convertRadian2Value(arr[2],-legs.back_hip[(i%max)]);
 _val[3] = dxl.convertRadian2Value(arr[3],legs.hip[(i%max)]);
 _val[4] = dxl.convertRadian2Value(arr[4],-legs.knee[(i%max)]);
 _val[5] = dxl.convertRadian2Value(arr[5],-legs.back_knee[(i%max)]);
 _val[6] = dxl.convertRadian2Value(arr[6],legs.back_knee[(i%max)]);
 _val[7] = dxl.convertRadian2Value(arr[7],legs.knee[(i%max)]);

  i+=1;

returned=update_goal(&dxl, arr, count, _val,false);

printf("Updated?:%s\n", returned ? "True" : "False");

while(joints[0]->moving()||joints[1]->moving()||joints[2]->moving()||joints[3]->moving());
read_pos(&dxl, arr, count, current_pos);
}
}
