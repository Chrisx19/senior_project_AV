#ifndef _ROS_av_senior_Arduino_telemetry_h
#define _ROS_av_senior_Arduino_telemetry_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace av_senior
{

  class Arduino_telemetry : public ros::Msg
  {
    public:
      typedef int16_t _Right_Distance_type;
      _Right_Distance_type Right_Distance;
      typedef int16_t _Mid_Distance_type;
      _Mid_Distance_type Mid_Distance;
      typedef int16_t _Left_Distance_type;
      _Left_Distance_type Left_Distance;

    Arduino_telemetry():
      Right_Distance(0),
      Mid_Distance(0),
      Left_Distance(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_Right_Distance;
      u_Right_Distance.real = this->Right_Distance;
      *(outbuffer + offset + 0) = (u_Right_Distance.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_Right_Distance.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->Right_Distance);
      union {
        int16_t real;
        uint16_t base;
      } u_Mid_Distance;
      u_Mid_Distance.real = this->Mid_Distance;
      *(outbuffer + offset + 0) = (u_Mid_Distance.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_Mid_Distance.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->Mid_Distance);
      union {
        int16_t real;
        uint16_t base;
      } u_Left_Distance;
      u_Left_Distance.real = this->Left_Distance;
      *(outbuffer + offset + 0) = (u_Left_Distance.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_Left_Distance.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->Left_Distance);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_Right_Distance;
      u_Right_Distance.base = 0;
      u_Right_Distance.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_Right_Distance.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->Right_Distance = u_Right_Distance.real;
      offset += sizeof(this->Right_Distance);
      union {
        int16_t real;
        uint16_t base;
      } u_Mid_Distance;
      u_Mid_Distance.base = 0;
      u_Mid_Distance.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_Mid_Distance.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->Mid_Distance = u_Mid_Distance.real;
      offset += sizeof(this->Mid_Distance);
      union {
        int16_t real;
        uint16_t base;
      } u_Left_Distance;
      u_Left_Distance.base = 0;
      u_Left_Distance.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_Left_Distance.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->Left_Distance = u_Left_Distance.real;
      offset += sizeof(this->Left_Distance);
     return offset;
    }

    virtual const char * getType() override { return "av_senior/Arduino_telemetry"; };
    virtual const char * getMD5() override { return "e6abe48c358f1e845717a29449a49bcb"; };

  };

}
#endif
