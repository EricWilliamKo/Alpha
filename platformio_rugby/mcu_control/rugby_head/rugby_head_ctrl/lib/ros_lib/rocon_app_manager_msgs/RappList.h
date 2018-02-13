#ifndef _ROS_rocon_app_manager_msgs_RappList_h
#define _ROS_rocon_app_manager_msgs_RappList_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "rocon_app_manager_msgs/Rapp.h"

namespace rocon_app_manager_msgs
{

  class RappList : public ros::Msg
  {
    public:
      uint32_t available_rapps_length;
      rocon_app_manager_msgs::Rapp st_available_rapps;
      rocon_app_manager_msgs::Rapp * available_rapps;
      uint32_t running_rapps_length;
      rocon_app_manager_msgs::Rapp st_running_rapps;
      rocon_app_manager_msgs::Rapp * running_rapps;

    RappList():
      available_rapps_length(0), available_rapps(NULL),
      running_rapps_length(0), running_rapps(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->available_rapps_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->available_rapps_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->available_rapps_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->available_rapps_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->available_rapps_length);
      for( uint32_t i = 0; i < available_rapps_length; i++){
      offset += this->available_rapps[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset + 0) = (this->running_rapps_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->running_rapps_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->running_rapps_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->running_rapps_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->running_rapps_length);
      for( uint32_t i = 0; i < running_rapps_length; i++){
      offset += this->running_rapps[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t available_rapps_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      available_rapps_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      available_rapps_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      available_rapps_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->available_rapps_length);
      if(available_rapps_lengthT > available_rapps_length)
        this->available_rapps = (rocon_app_manager_msgs::Rapp*)realloc(this->available_rapps, available_rapps_lengthT * sizeof(rocon_app_manager_msgs::Rapp));
      available_rapps_length = available_rapps_lengthT;
      for( uint32_t i = 0; i < available_rapps_length; i++){
      offset += this->st_available_rapps.deserialize(inbuffer + offset);
        memcpy( &(this->available_rapps[i]), &(this->st_available_rapps), sizeof(rocon_app_manager_msgs::Rapp));
      }
      uint32_t running_rapps_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      running_rapps_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      running_rapps_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      running_rapps_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->running_rapps_length);
      if(running_rapps_lengthT > running_rapps_length)
        this->running_rapps = (rocon_app_manager_msgs::Rapp*)realloc(this->running_rapps, running_rapps_lengthT * sizeof(rocon_app_manager_msgs::Rapp));
      running_rapps_length = running_rapps_lengthT;
      for( uint32_t i = 0; i < running_rapps_length; i++){
      offset += this->st_running_rapps.deserialize(inbuffer + offset);
        memcpy( &(this->running_rapps[i]), &(this->st_running_rapps), sizeof(rocon_app_manager_msgs::Rapp));
      }
     return offset;
    }

    const char * getType(){ return "rocon_app_manager_msgs/RappList"; };
    const char * getMD5(){ return "68cf30e80880962b92b2794f1f078c7f"; };

  };

}
#endif