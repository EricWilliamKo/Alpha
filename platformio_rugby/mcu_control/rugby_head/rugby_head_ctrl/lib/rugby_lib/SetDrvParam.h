#ifndef _ROS_SERVICE_SetDrvParam_h
#define _ROS_SERVICE_SetDrvParam_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace rugby
{

static const char SETDRVPARAM[] = "rugby/SetDrvParam";

  class SetDrvParamRequest : public ros::Msg
  {
    public:
      float Kp;
      float Ki;
      float Kd;

    SetDrvParamRequest():
      Kp(0),
      Ki(0),
      Kd(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += serializeAvrFloat64(outbuffer + offset, this->Kp);
      offset += serializeAvrFloat64(outbuffer + offset, this->Ki);
      offset += serializeAvrFloat64(outbuffer + offset, this->Kd);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->Kp));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->Ki));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->Kd));
     return offset;
    }

    const char * getType(){ return SETDRVPARAM; };
    const char * getMD5(){ return "aaaa16f23b4716dd0f817d0839cdbee8"; };

  };

  class SetDrvParamResponse : public ros::Msg
  {
    public:
      float Kp;
      float Ki;
      float Kd;

    SetDrvParamResponse():
      Kp(0),
      Ki(0),
      Kd(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += serializeAvrFloat64(outbuffer + offset, this->Kp);
      offset += serializeAvrFloat64(outbuffer + offset, this->Ki);
      offset += serializeAvrFloat64(outbuffer + offset, this->Kd);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->Kp));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->Ki));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->Kd));
     return offset;
    }

    const char * getType(){ return SETDRVPARAM; };
    const char * getMD5(){ return "aaaa16f23b4716dd0f817d0839cdbee8"; };

  };

  class SetDrvParam {
    public:
    typedef SetDrvParamRequest Request;
    typedef SetDrvParamResponse Response;
  };

}
#endif
