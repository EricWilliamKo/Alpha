#ifndef _ROS_leg_tracker_FollowingStatus_h
#define _ROS_leg_tracker_FollowingStatus_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace leg_tracker
{

  class FollowingStatus : public ros::Msg
  {
    public:
      uint8_t status;
      const char* text;
      enum { FAILED =  0 };
      enum { DETECTED =  1 };
      enum { ABORTED =  2 };
      enum { TRACKING =  3 };

    FollowingStatus():
      status(0),
      text("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->status >> (8 * 0)) & 0xFF;
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
      this->status =  ((uint8_t) (*(inbuffer + offset)));
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

    const char * getType(){ return "leg_tracker/FollowingStatus"; };
    const char * getMD5(){ return "2e8698faea01d07cc3aa466c9381f016"; };

  };

}
#endif