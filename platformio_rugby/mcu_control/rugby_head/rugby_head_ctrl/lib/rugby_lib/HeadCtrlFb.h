#ifndef _ROS_rugby_HeadCtrlFb_h
#define _ROS_rugby_HeadCtrlFb_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace rugby
{

  class HeadCtrlFb : public ros::Msg
  {
    public:
      float PanFbPos;
      float TiltFbPos;
      uint16_t PanFbCurrent;
      uint16_t TiltFbCurrent;
      int16_t PanFbDrvState;
      int16_t TiltFbDrvState;
      int16_t PanFbDrvPWM;
      int16_t TiltFbDrvPWM;

    HeadCtrlFb():
      PanFbPos(0),
      TiltFbPos(0),
      PanFbCurrent(0),
      TiltFbCurrent(0),
      PanFbDrvState(0),
      TiltFbDrvState(0),
      PanFbDrvPWM(0),
      TiltFbDrvPWM(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_PanFbPos;
      u_PanFbPos.real = this->PanFbPos;
      *(outbuffer + offset + 0) = (u_PanFbPos.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_PanFbPos.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_PanFbPos.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_PanFbPos.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->PanFbPos);
      union {
        float real;
        uint32_t base;
      } u_TiltFbPos;
      u_TiltFbPos.real = this->TiltFbPos;
      *(outbuffer + offset + 0) = (u_TiltFbPos.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_TiltFbPos.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_TiltFbPos.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_TiltFbPos.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->TiltFbPos);
      *(outbuffer + offset + 0) = (this->PanFbCurrent >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->PanFbCurrent >> (8 * 1)) & 0xFF;
      offset += sizeof(this->PanFbCurrent);
      *(outbuffer + offset + 0) = (this->TiltFbCurrent >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->TiltFbCurrent >> (8 * 1)) & 0xFF;
      offset += sizeof(this->TiltFbCurrent);
      union {
        int16_t real;
        uint16_t base;
      } u_PanFbDrvState;
      u_PanFbDrvState.real = this->PanFbDrvState;
      *(outbuffer + offset + 0) = (u_PanFbDrvState.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_PanFbDrvState.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->PanFbDrvState);
      union {
        int16_t real;
        uint16_t base;
      } u_TiltFbDrvState;
      u_TiltFbDrvState.real = this->TiltFbDrvState;
      *(outbuffer + offset + 0) = (u_TiltFbDrvState.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_TiltFbDrvState.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->TiltFbDrvState);
      union {
        int16_t real;
        uint16_t base;
      } u_PanFbDrvPWM;
      u_PanFbDrvPWM.real = this->PanFbDrvPWM;
      *(outbuffer + offset + 0) = (u_PanFbDrvPWM.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_PanFbDrvPWM.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->PanFbDrvPWM);
      union {
        int16_t real;
        uint16_t base;
      } u_TiltFbDrvPWM;
      u_TiltFbDrvPWM.real = this->TiltFbDrvPWM;
      *(outbuffer + offset + 0) = (u_TiltFbDrvPWM.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_TiltFbDrvPWM.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->TiltFbDrvPWM);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_PanFbPos;
      u_PanFbPos.base = 0;
      u_PanFbPos.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_PanFbPos.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_PanFbPos.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_PanFbPos.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->PanFbPos = u_PanFbPos.real;
      offset += sizeof(this->PanFbPos);
      union {
        float real;
        uint32_t base;
      } u_TiltFbPos;
      u_TiltFbPos.base = 0;
      u_TiltFbPos.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_TiltFbPos.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_TiltFbPos.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_TiltFbPos.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->TiltFbPos = u_TiltFbPos.real;
      offset += sizeof(this->TiltFbPos);
      this->PanFbCurrent =  ((uint16_t) (*(inbuffer + offset)));
      this->PanFbCurrent |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->PanFbCurrent);
      this->TiltFbCurrent =  ((uint16_t) (*(inbuffer + offset)));
      this->TiltFbCurrent |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->TiltFbCurrent);
      union {
        int16_t real;
        uint16_t base;
      } u_PanFbDrvState;
      u_PanFbDrvState.base = 0;
      u_PanFbDrvState.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_PanFbDrvState.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->PanFbDrvState = u_PanFbDrvState.real;
      offset += sizeof(this->PanFbDrvState);
      union {
        int16_t real;
        uint16_t base;
      } u_TiltFbDrvState;
      u_TiltFbDrvState.base = 0;
      u_TiltFbDrvState.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_TiltFbDrvState.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->TiltFbDrvState = u_TiltFbDrvState.real;
      offset += sizeof(this->TiltFbDrvState);
      union {
        int16_t real;
        uint16_t base;
      } u_PanFbDrvPWM;
      u_PanFbDrvPWM.base = 0;
      u_PanFbDrvPWM.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_PanFbDrvPWM.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->PanFbDrvPWM = u_PanFbDrvPWM.real;
      offset += sizeof(this->PanFbDrvPWM);
      union {
        int16_t real;
        uint16_t base;
      } u_TiltFbDrvPWM;
      u_TiltFbDrvPWM.base = 0;
      u_TiltFbDrvPWM.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_TiltFbDrvPWM.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->TiltFbDrvPWM = u_TiltFbDrvPWM.real;
      offset += sizeof(this->TiltFbDrvPWM);
     return offset;
    }

    const char * getType(){ return "rugby/HeadCtrlFb"; };
    const char * getMD5(){ return "f316bb4d44c9b0461cbcf7d3fced0894"; };

  };

}
#endif
