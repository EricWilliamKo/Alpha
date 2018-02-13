#ifndef _ROS_joint_qualification_controllers_JointPositionData_h
#define _ROS_joint_qualification_controllers_JointPositionData_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace joint_qualification_controllers
{

  class JointPositionData : public ros::Msg
  {
    public:
      uint32_t time_length;
      float st_time;
      float * time;
      uint32_t position_length;
      float st_position;
      float * position;
      uint32_t velocity_length;
      float st_velocity;
      float * velocity;
      uint32_t effort_length;
      float st_effort;
      float * effort;

    JointPositionData():
      time_length(0), time(NULL),
      position_length(0), position(NULL),
      velocity_length(0), velocity(NULL),
      effort_length(0), effort(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->time_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->time_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->time_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->time_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->time_length);
      for( uint32_t i = 0; i < time_length; i++){
      union {
        float real;
        uint32_t base;
      } u_timei;
      u_timei.real = this->time[i];
      *(outbuffer + offset + 0) = (u_timei.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_timei.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_timei.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_timei.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->time[i]);
      }
      *(outbuffer + offset + 0) = (this->position_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->position_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->position_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->position_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->position_length);
      for( uint32_t i = 0; i < position_length; i++){
      union {
        float real;
        uint32_t base;
      } u_positioni;
      u_positioni.real = this->position[i];
      *(outbuffer + offset + 0) = (u_positioni.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_positioni.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_positioni.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_positioni.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->position[i]);
      }
      *(outbuffer + offset + 0) = (this->velocity_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->velocity_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->velocity_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->velocity_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->velocity_length);
      for( uint32_t i = 0; i < velocity_length; i++){
      union {
        float real;
        uint32_t base;
      } u_velocityi;
      u_velocityi.real = this->velocity[i];
      *(outbuffer + offset + 0) = (u_velocityi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_velocityi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_velocityi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_velocityi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->velocity[i]);
      }
      *(outbuffer + offset + 0) = (this->effort_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->effort_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->effort_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->effort_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->effort_length);
      for( uint32_t i = 0; i < effort_length; i++){
      union {
        float real;
        uint32_t base;
      } u_efforti;
      u_efforti.real = this->effort[i];
      *(outbuffer + offset + 0) = (u_efforti.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_efforti.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_efforti.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_efforti.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->effort[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t time_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      time_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      time_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      time_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->time_length);
      if(time_lengthT > time_length)
        this->time = (float*)realloc(this->time, time_lengthT * sizeof(float));
      time_length = time_lengthT;
      for( uint32_t i = 0; i < time_length; i++){
      union {
        float real;
        uint32_t base;
      } u_st_time;
      u_st_time.base = 0;
      u_st_time.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_st_time.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_st_time.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_st_time.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->st_time = u_st_time.real;
      offset += sizeof(this->st_time);
        memcpy( &(this->time[i]), &(this->st_time), sizeof(float));
      }
      uint32_t position_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      position_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      position_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      position_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->position_length);
      if(position_lengthT > position_length)
        this->position = (float*)realloc(this->position, position_lengthT * sizeof(float));
      position_length = position_lengthT;
      for( uint32_t i = 0; i < position_length; i++){
      union {
        float real;
        uint32_t base;
      } u_st_position;
      u_st_position.base = 0;
      u_st_position.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_st_position.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_st_position.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_st_position.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->st_position = u_st_position.real;
      offset += sizeof(this->st_position);
        memcpy( &(this->position[i]), &(this->st_position), sizeof(float));
      }
      uint32_t velocity_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      velocity_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      velocity_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      velocity_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->velocity_length);
      if(velocity_lengthT > velocity_length)
        this->velocity = (float*)realloc(this->velocity, velocity_lengthT * sizeof(float));
      velocity_length = velocity_lengthT;
      for( uint32_t i = 0; i < velocity_length; i++){
      union {
        float real;
        uint32_t base;
      } u_st_velocity;
      u_st_velocity.base = 0;
      u_st_velocity.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_st_velocity.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_st_velocity.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_st_velocity.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->st_velocity = u_st_velocity.real;
      offset += sizeof(this->st_velocity);
        memcpy( &(this->velocity[i]), &(this->st_velocity), sizeof(float));
      }
      uint32_t effort_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      effort_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      effort_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      effort_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->effort_length);
      if(effort_lengthT > effort_length)
        this->effort = (float*)realloc(this->effort, effort_lengthT * sizeof(float));
      effort_length = effort_lengthT;
      for( uint32_t i = 0; i < effort_length; i++){
      union {
        float real;
        uint32_t base;
      } u_st_effort;
      u_st_effort.base = 0;
      u_st_effort.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_st_effort.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_st_effort.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_st_effort.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->st_effort = u_st_effort.real;
      offset += sizeof(this->st_effort);
        memcpy( &(this->effort[i]), &(this->st_effort), sizeof(float));
      }
     return offset;
    }

    const char * getType(){ return "joint_qualification_controllers/JointPositionData"; };
    const char * getMD5(){ return "20b7d551f5d22afac44b59b748c57bdb"; };

  };

}
#endif