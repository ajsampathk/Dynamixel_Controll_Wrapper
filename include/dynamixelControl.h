#ifndef _DXL_CNTRL
#define _DXL_CNTRL

#include "dynamixel_workbench.h"


class dynamixel_joint{
private:
  DynamixelWorkbench *dxl_wb;
  uint8_t id;

public:
  dynamixel_joint(uint8_t id,DynamixelWorkbench *wb,int32_t profile_velocity,int32_t profile_acc);
  ~dynamixel_joint();
   bool ping();
   bool Move(float rad);
   bool syncMove(float rad,uint8_t *id,uint8_t num);
   float read();
   bool torque(bool enable);
   int32_t moving();	
   bool addBulkWriteParam(uint8_t id, const char item_name, int32_t data); 
};
#include "dynamixelControl.cxx"
#endif
