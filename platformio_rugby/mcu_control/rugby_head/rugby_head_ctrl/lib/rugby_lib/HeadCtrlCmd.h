#ifndef _ROS_rugby_HeadCtrlCmd_h
#define _ROS_rugby_HeadCtrlCmd_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace rugby
{

  class HeadCtrlCmd : public ros::Msg
  {
    public:
      float PanCmdPos;
      float TiltCmdPos;

    HeadCtrlCmd():
      PanCmdPos(0),
      TiltCmdPos(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_PanCmdPos;
      u_PanCmdPos.real = this->PanCmdPos;
      *(outbuffer + offset + 0) = (u_PanCmdPos.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_PanCmdPos.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_PanCmdPos.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_PanCmdPos.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->PanCmdPos);
      union {
        float real;
        uint32_t base;
      } u_TiltCmdPos;
      u_TiltCmdPos.real = this->TiltCmdPos;
      *(outbuffer + offset + 0) = (u_TiltCmdPos.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_TiltCmdPos.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_TiltCmdPos.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_TiltCmdPos.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->TiltCmdPos);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_PanCmdPos;
      u_PanCmdPos.base = 0;
      u_PanCmdPos.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_PanCmdPos.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_PanCmdPos.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_PanCmdPos.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->PanCmdPos = u_PanCmdPos.real;
      offset += sizeof(this->PanCmdPos);
      union {
        float real;
        uint32_t base;
      } u_TiltCmdPos;
      u_TiltCmdPos.base = 0;
      u_TiltCmdPos.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_TiltCmdPos.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_TiltCmdPos.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_TiltCmdPos.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->TiltCmdPos = u_TiltCmdPos.real;
      offset += sizeof(this->TiltCmdPos);
     return offset;
    }

    const char * getType(){ return "rugby/HeadCtrlCmd"; };
    const char * getMD5(){ return "005ed080bf9b4f19585e20d3d005da54"; };

  };

}
#endif