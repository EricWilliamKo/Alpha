#ifndef _ROS_lidar_follow_me_RobotStatus_h
#define _ROS_lidar_follow_me_RobotStatus_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace lidar_follow_me
{

  class RobotStatus : public ros::Msg
  {
    public:
      int8_t status;
      const char* text;
      enum { FAILED =  0 };
      enum { DETECTED =  1 };
      enum { ABORTED =  2 };
      enum { TRACKING =  3 };

    RobotStatus():
      status(0),
      text("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int8_t real;
        uint8_t base;
      } u_status;
      u_status.real = this->status;
      *(outbuffer + offset + 0) = (u_status.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->status);
      uint32_t length_text = strlen(this->text);
      memcpy(outbuffer + offset, &length_text, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->text, length_text);
      offset += length_text;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int8_t real;
        uint8_t base;
      } u_status;
      u_status.base = 0;
      u_status.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->status = u_status.real;
      offset += sizeof(this->status);
      uint32_t length_text;
      memcpy(&length_text, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_text; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_text-1]=0;
      this->text = (char *)(inbuffer + offset-1);
      offset += length_text;
     return offset;
    }

    const char * getType(){ return "lidar_follow_me/RobotStatus"; };
    const char * getMD5(){ return "62c1ce3999cf490482be865671595d64"; };

  };

}
#endif