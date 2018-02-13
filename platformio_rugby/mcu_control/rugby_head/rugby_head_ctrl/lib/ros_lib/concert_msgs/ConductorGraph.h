#ifndef _ROS_concert_msgs_ConductorGraph_h
#define _ROS_concert_msgs_ConductorGraph_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "concert_msgs/ConcertClient.h"

namespace concert_msgs
{

  class ConductorGraph : public ros::Msg
  {
    public:
      uint32_t pending_length;
      concert_msgs::ConcertClient st_pending;
      concert_msgs::ConcertClient * pending;
      uint32_t bad_length;
      concert_msgs::ConcertClient st_bad;
      concert_msgs::ConcertClient * bad;
      uint32_t blocking_length;
      concert_msgs::ConcertClient st_blocking;
      concert_msgs::ConcertClient * blocking;
      uint32_t busy_length;
      concert_msgs::ConcertClient st_busy;
      concert_msgs::ConcertClient * busy;
      uint32_t uninvited_length;
      concert_msgs::ConcertClient st_uninvited;
      concert_msgs::ConcertClient * uninvited;
      uint32_t joining_length;
      concert_msgs::ConcertClient st_joining;
      concert_msgs::ConcertClient * joining;
      uint32_t available_length;
      concert_msgs::ConcertClient st_available;
      concert_msgs::ConcertClient * available;
      uint32_t missing_length;
      concert_msgs::ConcertClient st_missing;
      concert_msgs::ConcertClient * missing;
      uint32_t gone_length;
      concert_msgs::ConcertClient st_gone;
      concert_msgs::ConcertClient * gone;

    ConductorGraph():
      pending_length(0), pending(NULL),
      bad_length(0), bad(NULL),
      blocking_length(0), blocking(NULL),
      busy_length(0), busy(NULL),
      uninvited_length(0), uninvited(NULL),
      joining_length(0), joining(NULL),
      available_length(0), available(NULL),
      missing_length(0), missing(NULL),
      gone_length(0), gone(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->pending_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->pending_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->pending_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->pending_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->pending_length);
      for( uint32_t i = 0; i < pending_length; i++){
      offset += this->pending[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset + 0) = (this->bad_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->bad_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->bad_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->bad_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->bad_length);
      for( uint32_t i = 0; i < bad_length; i++){
      offset += this->bad[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset + 0) = (this->blocking_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->blocking_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->blocking_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->blocking_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->blocking_length);
      for( uint32_t i = 0; i < blocking_length; i++){
      offset += this->blocking[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset + 0) = (this->busy_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->busy_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->busy_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->busy_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->busy_length);
      for( uint32_t i = 0; i < busy_length; i++){
      offset += this->busy[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset + 0) = (this->uninvited_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->uninvited_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->uninvited_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->uninvited_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->uninvited_length);
      for( uint32_t i = 0; i < uninvited_length; i++){
      offset += this->uninvited[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset + 0) = (this->joining_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->joining_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->joining_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->joining_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->joining_length);
      for( uint32_t i = 0; i < joining_length; i++){
      offset += this->joining[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset + 0) = (this->available_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->available_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->available_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->available_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->available_length);
      for( uint32_t i = 0; i < available_length; i++){
      offset += this->available[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset + 0) = (this->missing_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->missing_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->missing_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->missing_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->missing_length);
      for( uint32_t i = 0; i < missing_length; i++){
      offset += this->missing[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset + 0) = (this->gone_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->gone_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->gone_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->gone_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->gone_length);
      for( uint32_t i = 0; i < gone_length; i++){
      offset += this->gone[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t pending_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      pending_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      pending_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      pending_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->pending_length);
      if(pending_lengthT > pending_length)
        this->pending = (concert_msgs::ConcertClient*)realloc(this->pending, pending_lengthT * sizeof(concert_msgs::ConcertClient));
      pending_length = pending_lengthT;
      for( uint32_t i = 0; i < pending_length; i++){
      offset += this->st_pending.deserialize(inbuffer + offset);
        memcpy( &(this->pending[i]), &(this->st_pending), sizeof(concert_msgs::ConcertClient));
      }
      uint32_t bad_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      bad_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      bad_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      bad_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->bad_length);
      if(bad_lengthT > bad_length)
        this->bad = (concert_msgs::ConcertClient*)realloc(this->bad, bad_lengthT * sizeof(concert_msgs::ConcertClient));
      bad_length = bad_lengthT;
      for( uint32_t i = 0; i < bad_length; i++){
      offset += this->st_bad.deserialize(inbuffer + offset);
        memcpy( &(this->bad[i]), &(this->st_bad), sizeof(concert_msgs::ConcertClient));
      }
      uint32_t blocking_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      blocking_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      blocking_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      blocking_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->blocking_length);
      if(blocking_lengthT > blocking_length)
        this->blocking = (concert_msgs::ConcertClient*)realloc(this->blocking, blocking_lengthT * sizeof(concert_msgs::ConcertClient));
      blocking_length = blocking_lengthT;
      for( uint32_t i = 0; i < blocking_length; i++){
      offset += this->st_blocking.deserialize(inbuffer + offset);
        memcpy( &(this->blocking[i]), &(this->st_blocking), sizeof(concert_msgs::ConcertClient));
      }
      uint32_t busy_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      busy_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      busy_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      busy_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->busy_length);
      if(busy_lengthT > busy_length)
        this->busy = (concert_msgs::ConcertClient*)realloc(this->busy, busy_lengthT * sizeof(concert_msgs::ConcertClient));
      busy_length = busy_lengthT;
      for( uint32_t i = 0; i < busy_length; i++){
      offset += this->st_busy.deserialize(inbuffer + offset);
        memcpy( &(this->busy[i]), &(this->st_busy), sizeof(concert_msgs::ConcertClient));
      }
      uint32_t uninvited_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      uninvited_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      uninvited_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      uninvited_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->uninvited_length);
      if(uninvited_lengthT > uninvited_length)
        this->uninvited = (concert_msgs::ConcertClient*)realloc(this->uninvited, uninvited_lengthT * sizeof(concert_msgs::ConcertClient));
      uninvited_length = uninvited_lengthT;
      for( uint32_t i = 0; i < uninvited_length; i++){
      offset += this->st_uninvited.deserialize(inbuffer + offset);
        memcpy( &(this->uninvited[i]), &(this->st_uninvited), sizeof(concert_msgs::ConcertClient));
      }
      uint32_t joining_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      joining_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      joining_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      joining_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->joining_length);
      if(joining_lengthT > joining_length)
        this->joining = (concert_msgs::ConcertClient*)realloc(this->joining, joining_lengthT * sizeof(concert_msgs::ConcertClient));
      joining_length = joining_lengthT;
      for( uint32_t i = 0; i < joining_length; i++){
      offset += this->st_joining.deserialize(inbuffer + offset);
        memcpy( &(this->joining[i]), &(this->st_joining), sizeof(concert_msgs::ConcertClient));
      }
      uint32_t available_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      available_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      available_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      available_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->available_length);
      if(available_lengthT > available_length)
        this->available = (concert_msgs::ConcertClient*)realloc(this->available, available_lengthT * sizeof(concert_msgs::ConcertClient));
      available_length = available_lengthT;
      for( uint32_t i = 0; i < available_length; i++){
      offset += this->st_available.deserialize(inbuffer + offset);
        memcpy( &(this->available[i]), &(this->st_available), sizeof(concert_msgs::ConcertClient));
      }
      uint32_t missing_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      missing_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      missing_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      missing_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->missing_length);
      if(missing_lengthT > missing_length)
        this->missing = (concert_msgs::ConcertClient*)realloc(this->missing, missing_lengthT * sizeof(concert_msgs::ConcertClient));
      missing_length = missing_lengthT;
      for( uint32_t i = 0; i < missing_length; i++){
      offset += this->st_missing.deserialize(inbuffer + offset);
        memcpy( &(this->missing[i]), &(this->st_missing), sizeof(concert_msgs::ConcertClient));
      }
      uint32_t gone_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      gone_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      gone_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      gone_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->gone_length);
      if(gone_lengthT > gone_length)
        this->gone = (concert_msgs::ConcertClient*)realloc(this->gone, gone_lengthT * sizeof(concert_msgs::ConcertClient));
      gone_length = gone_lengthT;
      for( uint32_t i = 0; i < gone_length; i++){
      offset += this->st_gone.deserialize(inbuffer + offset);
        memcpy( &(this->gone[i]), &(this->st_gone), sizeof(concert_msgs::ConcertClient));
      }
     return offset;
    }

    const char * getType(){ return "concert_msgs/ConductorGraph"; };
    const char * getMD5(){ return "3985298cb56505a707665446d34bba5e"; };

  };

}
#endif