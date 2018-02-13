#ifndef _ROS_rocon_interaction_msgs_InteractiveClient_h
#define _ROS_rocon_interaction_msgs_InteractiveClient_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "uuid_msgs/UniqueID.h"
#include "rocon_std_msgs/PlatformInfo.h"

namespace rocon_interaction_msgs
{

  class InteractiveClient : public ros::Msg
  {
    public:
      const char* name;
      uuid_msgs::UniqueID id;
      rocon_std_msgs::PlatformInfo platform_info;
      uint32_t running_interactions_length;
      char* st_running_interactions;
      char* * running_interactions;

    InteractiveClient():
      name(""),
      id(),
      platform_info(),
      running_interactions_length(0), running_interactions(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_name = strlen(this->name);
      memcpy(outbuffer + offset, &length_name, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->name, length_name);
      offset += length_name;
      offset += this->id.serialize(outbuffer + offset);
      offset += this->platform_info.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->running_interactions_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->running_interactions_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->running_interactions_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->running_interactions_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->running_interactions_length);
      for( uint32_t i = 0; i < running_interactions_length; i++){
      uint32_t length_running_interactionsi = strlen(this->running_interactions[i]);
      memcpy(outbuffer + offset, &length_running_interactionsi, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->running_interactions[i], length_running_interactionsi);
      offset += length_running_interactionsi;
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_name;
      memcpy(&length_name, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_name; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_name-1]=0;
      this->name = (char *)(inbuffer + offset-1);
      offset += length_name;
      offset += this->id.deserialize(inbuffer + offset);
      offset += this->platform_info.deserialize(inbuffer + offset);
      uint32_t running_interactions_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      running_interactions_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      running_interactions_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      running_interactions_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->running_interactions_length);
      if(running_interactions_lengthT > running_interactions_length)
        this->running_interactions = (char**)realloc(this->running_interactions, running_interactions_lengthT * sizeof(char*));
      running_interactions_length = running_interactions_lengthT;
      for( uint32_t i = 0; i < running_interactions_length; i++){
      uint32_t length_st_running_interactions;
      memcpy(&length_st_running_interactions, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_running_interactions; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_running_interactions-1]=0;
      this->st_running_interactions = (char *)(inbuffer + offset-1);
      offset += length_st_running_interactions;
        memcpy( &(this->running_interactions[i]), &(this->st_running_interactions), sizeof(char*));
      }
     return offset;
    }

    const char * getType(){ return "rocon_interaction_msgs/InteractiveClient"; };
    const char * getMD5(){ return "d8f65f317e0815a1ab6774b422c35d05"; };

  };

}
#endif