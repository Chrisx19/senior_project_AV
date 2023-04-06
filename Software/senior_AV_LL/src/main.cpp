#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/UInt32.h>
#include <SoftwareSerial.h>
#include "RoboClaw.h"
#include <math.h>

#define address1 0x80
#define address2 0x81
#define address3 0x82     //corner_motor

ros::NodeHandle  nh;

SoftwareSerial serial(10, 11);     //serial (rx, tx)
RoboClaw roboclaw(&serial, 10000); //10000
//==========================================================================Global================================================================================
void AV_vel_cb(const geometry_msgs::Twist& AV_vel_msg);
void forward_backward(int val);
void right_left(int turn);
void motor_stop(void);
uint32_t avg_encoder_val(int addy, int addy_2);

int drive_duty_cycle = 0;
int av_encoder = avg_encoder_val(address1, address2);

ros::Subscriber<geometry_msgs::Twist> AV_vel_sub("cmd_vel_AV", &AV_vel_cb);

std_msgs::UInt32 enc;
ros::Publisher enc_pub("av_encoder", &enc);
//==========================================================================Steup and Loop================================================================================
void setup()
{
  roboclaw.begin(115200);

  nh.initNode();
  nh.advertise(enc_pub);
  nh.subscribe(AV_vel_sub);
  
  motor_stop();
}

void loop()
{
  enc.data = av_encoder;
  enc_pub.publish( &enc);
  nh.spinOnce();
//  delay(1);
}
//==========================================================================Functions================================================================================
void AV_vel_cb(const geometry_msgs::Twist& AV_vel_msg)  //callback function from subscribe
{
  drive_duty_cycle = AV_vel_msg.linear.x;           //val: -63 <-> 63
  int corner = AV_vel_msg.angular.z + 780;

  forward_backward(drive_duty_cycle);
  right_left(corner);
}//end fo AV_vel_cb

uint32_t avg_encoder_val(int addy, int addy_2)
{
  uint32_t encoder1_M1 = roboclaw.ReadEncM1(addy);
  uint32_t encoder2_M1 = roboclaw.ReadEncM2(addy);
  uint32_t encoder1_M2 = roboclaw.ReadEncM1(addy_2);
  uint32_t encoder2_M2 = roboclaw.ReadEncM2(addy_2);

  return (encoder1_M1 + encoder2_M1 + encoder1_M2 + encoder2_M2) / 4;
}//end of avg_encoder_val

void forward_backward(int drive)
{
  if(drive > 0)
  {
    roboclaw.ForwardM1(address1, drive);
    roboclaw.ForwardM2(address1, drive);
    roboclaw.ForwardM1(address2, drive);
    roboclaw.ForwardM2(address2, drive);
  }
  else
  {
    roboclaw.BackwardM1(address1, drive * -1);
    roboclaw.BackwardM2(address1, drive * -1);
    roboclaw.BackwardM1(address2, drive * -1);
    roboclaw.BackwardM2(address2, drive * -1);
  }
}//end of forward_backward

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
    roboclaw.ForwardM1(rc[i], 0);
  }
}//end of motor_stop