#ifndef _ROS_rocon_app_manager_msgs_Rapp_h
#define _ROS_rocon_app_manager_msgs_Rapp_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "rocon_std_msgs/Icon.h"
#include "rocon_std_msgs/KeyValue.h"

namespace rocon_app_manager_msgs
{

  class Rapp : public ros::Msg
  {
    public:
      const char* name;
      const char* display_name;
      const char* description;
      const char* compatibility;
      const char* status;
      uint32_t implementations_length;
      char* st_implementations;
      char* * implementations;
      const char* preferred;
      rocon_std_msgs::Icon icon;
      uint32_t public_interface_length;
      rocon_std_msgs::KeyValue st_public_interface;
      rocon_std_msgs::KeyValue * public_interface;
      uint32_t public_parameters_length;
      rocon_std_msgs::KeyValue st_public_parameters;
      rocon_std_msgs::KeyValue * public_parameters;

    Rapp():
      name(""),
      display_name(""),
      description(""),
      compatibility(""),
      status(""),
      implementations_length(0), implementations(NULL),
      preferred(""),
      icon(),
      public_interface_length(0), public_interface(NULL),
      public_parameters_length(0), public_parameters(NULL)
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
      uint32_t length_display_name = strlen(this->display_name);
      memcpy(outbuffer + offset, &length_display_name, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->display_name, length_display_name);
      offset += length_display_name;
      uint32_t length_description = strlen(this->description);
      memcpy(outbuffer + offset, &length_description, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->description, length_description);
      offset += length_description;
      uint32_t length_compatibility = strlen(this->compatibility);
      memcpy(outbuffer + offset, &length_compatibility, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->compatibility, length_compatibility);
      offset += length_compatibility;
      uint32_t length_status = strlen(this->status);
      memcpy(outbuffer + offset, &length_status, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->status, length_status);
      offset += length_status;
      *(outbuffer + offset + 0) = (this->implementations_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->implementations_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->implementations_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->implementations_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->implementations_length);
      for( uint32_t i = 0; i < implementations_length; i++){
      uint32_t length_implementationsi = strlen(this->implementations[i]);
      memcpy(outbuffer + offset, &length_implementationsi, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->implementations[i], length_implementationsi);
      offset += length_implementationsi;
      }
      uint32_t length_preferred = strlen(this->preferred);
      memcpy(outbuffer + offset, &length_preferred, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->preferred, length_preferred);
      offset += length_preferred;
      offset += this->icon.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->public_interface_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->public_interface_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->public_interface_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->public_interface_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->public_interface_length);
      for( uint32_t i = 0; i < public_interface_length; i++){
      offset += this->public_interface[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset + 0) = (this->public_parameters_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->public_parameters_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->public_parameters_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->public_parameters_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->public_parameters_length);
      for( uint32_t i = 0; i < public_parameters_length; i++){
      offset += this->public_parameters[i].serialize(outbuffer + offset);
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
      uint32_t length_display_name;
      memcpy(&length_display_name, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_display_name; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_display_name-1]=0;
      this->display_name = (char *)(inbuffer + offset-1);
      offset += length_display_name;
      uint32_t length_description;
      memcpy(&length_description, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_description; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_description-1]=0;
      this->description = (char *)(inbuffer + offset-1);
      offset += length_description;
      uint32_t length_compatibility;
      memcpy(&length_compatibility, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_compatibility; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_compatibility-1]=0;
      this->compatibility = (char *)(inbuffer + offset-1);
      offset += length_compatibility;
      uint32_t length_status;
      memcpy(&length_status, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_status; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_status-1]=0;
      this->status = (char *)(inbuffer + offset-1);
      offset += length_status;
      uint32_t implementations_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      implementations_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      implementations_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      implementations_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->implementations_length);
      if(implementations_lengthT > implementations_length)
        this->implementations = (char**)realloc(this->implementations, implementations_lengthT * sizeof(char*));
      implementations_length = implementations_lengthT;
      for( uint32_t i = 0; i < implementations_length; i++){
      uint32_t length_st_implementations;
      memcpy(&length_st_implementations, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_implementations; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_implementations-1]=0;
      this->st_implementations = (char *)(inbuffer + offset-1);
      offset += length_st_implementations;
        memcpy( &(this->implementations[i]), &(this->st_implementations), sizeof(char*));
      }
      uint32_t length_preferred;
      memcpy(&length_preferred, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_preferred; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_preferred-1]=0;
      this->preferred = (char *)(inbuffer + offset-1);
      offset += length_preferred;
      offset += this->icon.deserialize(inbuffer + offset);
      uint32_t public_interface_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      public_interface_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      public_interface_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      public_interface_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->public_interface_length);
      if(public_interface_lengthT > public_interface_length)
        this->public_interface = (rocon_std_msgs::KeyValue*)realloc(this->public_interface, public_interface_lengthT * sizeof(rocon_std_msgs::KeyValue));
      public_interface_length = public_interface_lengthT;
      for( uint32_t i = 0; i < public_interface_length; i++){
      offset += this->st_public_interface.deserialize(inbuffer + offset);
        memcpy( &(this->public_interface[i]), &(this->st_public_interface), sizeof(rocon_std_msgs::KeyValue));
      }
      uint32_t public_parameters_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      public_parameters_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      public_parameters_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      public_parameters_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->public_parameters_length);
      if(public_parameters_lengthT > public_parameters_length)
        this->public_parameters = (rocon_std_msgs::KeyValue*)realloc(this->public_parameters, public_parameters_lengthT * sizeof(rocon_std_msgs::KeyValue));
      public_parameters_length = public_parameters_lengthT;
      for( uint32_t i = 0; i < public_parameters_length; i++){
      offset += this->st_public_parameters.deserialize(inbuffer + offset);
        memcpy( &(this->public_parameters[i]), &(this->st_public_parameters), sizeof(rocon_std_msgs::KeyValue));
      }
     return offset;
    }

    const char * getType(){ return "rocon_app_manager_msgs/Rapp"; };
    const char * getMD5(){ return "0214a3ee298e9d3ae08c6ab15b8e910d"; };

  };

}
#endif