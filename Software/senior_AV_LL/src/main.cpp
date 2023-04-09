#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <SoftwareSerial.h>
#include "RoboClaw.h"
#include <math.h>

#define address1 0x80
#define address2 0x81
#define address3 0x82     //corner_motor

#define wheel_radius 0.076
#define ppr_motor 537.7
#define circumference (2 * PI * wheel_radius)

ros::NodeHandle  nh;

SoftwareSerial serial(10, 11);     //serial (rx, tx)
RoboClaw roboclaw(&serial, 10000); //10000
//==========================================================================Global================================================================================
void AV_vel_cb(const geometry_msgs::Twist& AV_vel_msg);
void right_left(int turn);
void motor_stop(void);
int avg_encoder_val(int addy, int addy_2);

int g_av_encoder = 0;
float g_m_s = 0;

ros::Subscriber<geometry_msgs::Twist> AV_vel_sub("cmd_vel_AV", &AV_vel_cb);

std_msgs::Int32 enc;
ros::Publisher enc_pub("qpps", &enc);

std_msgs::Float32 speed;
ros::Publisher speed_pub("speed_av", &speed);
//==========================================================================Setup and Loop================================================================================
void setup()
{
  roboclaw.begin(115200);
  nh.getHardware()->setBaud(115200);

  nh.initNode();
  nh.advertise(enc_pub);
  nh.advertise(speed_pub);
  nh.subscribe(AV_vel_sub);
  motor_stop();
}

void loop()
{
  enc.data = g_av_encoder;
  enc_pub.publish( &enc);

  speed.data = g_m_s;
  speed_pub.publish( &speed);
  nh.spinOnce();
  // delay(1);
}
//==========================================================================Functions================================================================================
void AV_vel_cb(const geometry_msgs::Twist& AV_vel_msg)  //callback function from subscribe
{
  int drive_duty_cycle = AV_vel_msg.linear.x;           //val: -63 <-> 63
  int corner = AV_vel_msg.angular.z + 780;

  g_av_encoder = avg_encoder_val(address1, address2);
  // g_av_encoder = roboclaw.ReadSpeedM2(address1);
  g_m_s = circumference * (1 / ppr_motor) * g_av_encoder;

  roboclaw.SpeedM1M2(address1, drive_duty_cycle, drive_duty_cycle);
  roboclaw.SpeedM1M2(address2, drive_duty_cycle, drive_duty_cycle);
  right_left(corner);
}//end fo AV_vel_cb

int avg_encoder_val(int addy, int addy_2)
{
  int encoder1_M1 = roboclaw.ReadSpeedM1(addy);
  int encoder2_M1 = roboclaw.ReadSpeedM2(addy);
  int encoder1_M2 = roboclaw.ReadSpeedM1(addy_2);
  int encoder2_M2 = roboclaw.ReadSpeedM2(addy_2);

  return (encoder1_M1 + encoder2_M1 + encoder1_M2 + encoder2_M2) / 4;
}//end of avg_encoder_val

void right_left(int turn)
{
  float accel_rate = 1.5;
  float accel_max = pow(2,15)-1;
  int corner_accel = accel_max * accel_rate;

  if (turn < 780)
  {
    roboclaw.SpeedAccelDeccelPositionM1(address3, corner_accel, 1000, corner_accel, turn, 1);    //Right_Corner    //  roboclaw.SpeedAccelDeccelPositionM1(address3,26213,1000,26213,6,1);    //Right_Turn
    roboclaw.SpeedAccelDeccelPositionM2(address3, corner_accel, 1000, corner_accel, turn, 1);                      //  roboclaw.SpeedAccelDeccelPositionM2(address3,26213,1000,26213,7,1);
  }
  else if (turn > 780)
  {
    roboclaw.SpeedAccelDeccelPositionM1(address3, corner_accel, 1000, corner_accel, turn, 1);   //Left_Corner       //  roboclaw.SpeedAccelDeccelPositionM1(address3,26213,1000,26213,1559,1);    //Left_Turn 
    roboclaw.SpeedAccelDeccelPositionM2(address3, corner_accel, 1000, corner_accel, turn, 1);                       //  roboclaw.SpeedAccelDeccelPositionM2(address3,26213,1000,26213,1564,1);
  }
  else
  {
    roboclaw.SpeedAccelDeccelPositionM1(address3, corner_accel, 1000, corner_accel, 777, 1);  //Middle_Corner
    roboclaw.SpeedAccelDeccelPositionM2(address3, corner_accel, 1000, corner_accel, 750, 1);
  }
}//end of right_left

void motor_stop(void)
{
  uint8_t rc[3] = {address1, address2, address3};
  for(int i = 0; i < 3; i++)
  {
    roboclaw.SpeedM1M2(rc[i], 0, 0);
  }
}//end of motor_stop