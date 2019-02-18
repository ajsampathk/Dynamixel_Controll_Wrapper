#include "dynamixelControl.h"

dynamixel_joint::dynamixel_joint(uint8_t _id,DynamixelWorkbench *wb,int32_t profile_velocity,int32_t profile_acc){
  id = _id;
  dxl_wb=wb;
  bool vel,acc;
  acc=dxl_wb->itemWrite(id, "Profile_Acceleration", profile_acc);
  vel=dxl_wb->itemWrite(id, "Profile_Velocity", profile_velocity);
  printf("[INFO ID:%d ] Profile Velocity:%d State:%s  ",id,profile_velocity, vel ? "True" : "False");
  printf("Profile Acc:%d State:%s\n--------------------------\n",profile_acc, vel ? "True" : "False");
}

dynamixel_joint::~dynamixel_joint(){
  uint8_t x =dxl_wb->itemWrite(id,"Torque_Enable",0);
  printf("Turning off Torque[ID:%d]:%s\n",id, x ? "True" : "False");
}

bool dynamixel_joint::torque(bool enable){
  uint8_t x =dxl_wb->itemWrite(id,"Torque_Enable",enable ? 1:0);
  printf("[INFO ID:%d ] Torque State:%s\n",id, x ? "On" : "Off");
  printf("---------------------------------------------------\n");
  return true;
}

bool dynamixel_joint::ping(){
  return dxl_wb->ping(id);
}

bool dynamixel_joint::syncMove(float rad,uint8_t *id,uint8_t num){
  int32_t goal = dxl_wb->convertRadian2Value(id[0],rad);
  dxl_wb->syncWrite(id, num,"Goal_Position", &goal);
}

bool dynamixel_joint::Move(float rad){
  int32_t goal = 0;
  goal = dxl_wb->convertRadian2Value(id,rad);
  // printf("[INFO] ID:%d-Goal Position:%d\n",id,goal);
  return dxl_wb->goalPosition(id,goal);
}

float dynamixel_joint::read(){
  return dxl_wb->convertValue2Radian(id, dxl_wb->itemRead(id,"Present_Position"));
}

int32_t dynamixel_joint::moving(){
  return dxl_wb->itemRead(id,"Moving");
}
