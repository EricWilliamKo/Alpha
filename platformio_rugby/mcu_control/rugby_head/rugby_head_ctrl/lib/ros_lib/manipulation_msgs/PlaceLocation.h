#ifndef _ROS_manipulation_msgs_PlaceLocation_h
#define _ROS_manipulation_msgs_PlaceLocation_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "sensor_msgs/JointState.h"
#include "geometry_msgs/PoseStamped.h"
#include "manipulation_msgs/GripperTranslation.h"

namespace manipulation_msgs
{

  class PlaceLocation : public ros::Msg
  {
    public:
      const char* id;
      sensor_msgs::JointState post_place_posture;
      geometry_msgs::PoseStamped place_pose;
      manipulation_msgs::GripperTranslation approach;
      manipulation_msgs::GripperTranslation retreat;
      uint32_t allowed_touch_objects_length;
      char* st_allowed_touch_objects;
      char* * allowed_touch_objects;

    PlaceLocation():
      id(""),
      post_place_posture(),
      place_pose(),
      approach(),
      retreat(),
      allowed_touch_objects_length(0), allowed_touch_objects(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_id = strlen(this->id);
      memcpy(outbuffer + offset, &length_id, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->id, length_id);
      offset += length_id;
      offset += this->post_place_posture.serialize(outbuffer + offset);
      offset += this->place_pose.serialize(outbuffer + offset);
      offset += this->approach.serialize(outbuffer + offset);
      offset += this->retreat.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->allowed_touch_objects_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->allowed_touch_objects_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->allowed_touch_objects_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->allowed_touch_objects_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->allowed_touch_objects_length);
      for( uint32_t i = 0; i < allowed_touch_objects_length; i++){
      uint32_t length_allowed_touch_objectsi = strlen(this->allowed_touch_objects[i]);
      memcpy(outbuffer + offset, &length_allowed_touch_objectsi, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->allowed_touch_objects[i], length_allowed_touch_objectsi);
      offset += length_allowed_touch_objectsi;
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_id;
      memcpy(&length_id, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_id; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_id-1]=0;
      this->id = (char *)(inbuffer + offset-1);
      offset += length_id;
      offset += this->post_place_posture.deserialize(inbuffer + offset);
      offset += this->place_pose.deserialize(inbuffer + offset);
      offset += this->approach.deserialize(inbuffer + offset);
      offset += this->retreat.deserialize(inbuffer + offset);
      uint32_t allowed_touch_objects_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      allowed_touch_objects_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      allowed_touch_objects_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      allowed_touch_objects_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->allowed_touch_objects_length);
      if(allowed_touch_objects_lengthT > allowed_touch_objects_length)
        this->allowed_touch_objects = (char**)realloc(this->allowed_touch_objects, allowed_touch_objects_lengthT * sizeof(char*));
      allowed_touch_objects_length = allowed_touch_objects_lengthT;
      for( uint32_t i = 0; i < allowed_touch_objects_length; i++){
      uint32_t length_st_allowed_touch_objects;
      memcpy(&length_st_allowed_touch_objects, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_allowed_touch_objects; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_allowed_touch_objects-1]=0;
      this->st_allowed_touch_objects = (char *)(inbuffer + offset-1);
      offset += length_st_allowed_touch_objects;
        memcpy( &(this->allowed_touch_objects[i]), &(this->st_allowed_touch_objects), sizeof(char*));
      }
     return offset;
    }

    const char * getType(){ return "manipulation_msgs/PlaceLocation"; };
    const char * getMD5(){ return "0139dab9852add0e64233c5fb3b8a25a"; };

  };

}
#endif