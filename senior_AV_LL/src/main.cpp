#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int32.h>
#include <SoftwareSerial.h>
#include "RoboClaw.h"
#include <math.h>

#define address1 0x80
#define address2 0x81
#define address3 0x82     //corner_motor

ros::NodeHandle  nh;

SoftwareSerial serial(10, 11);     //serial (rx, tx)
RoboClaw roboclaw(&serial, 10000); //10000

int drive_duty_cycle = 0;
bool enable = false;

void AV_vel_cb(const geometry_msgs::Twist& AV_vel_msg);
void motor_stop(void);

ros::Subscriber<geometry_msgs::Twist> AV_vel_sub("cmd_vel_AV", &AV_vel_cb);

std_msgs::Int32 velocity;
ros::Publisher vel_cmd("vel_cmd", &velocity);

void setup()
{
  roboclaw.begin(115200);

  nh.initNode();
  nh.advertise(vel_cmd);
  nh.subscribe(AV_vel_sub);
//  roboclaw.ForwardM1(address3, 0);
//  roboclaw.ForwardM2(address3, 0);

}

void loop()
{
  velocity.data = drive_duty_cycle;
  vel_cmd.publish( &velocity );
  enable == false;
  motor_stop();
  nh.spinOnce();
//  delay(1);
}

void motor_stop(void)
{
  if (enable == false)
  {
    roboclaw.ForwardM1(address1, 0);
    roboclaw.ForwardM2(address1, 0);
    roboclaw.ForwardM1(address2, 0);
    roboclaw.ForwardM2(address2, 0);
    roboclaw.ForwardM1(address3, 0);
    roboclaw.ForwardM2(address3, 0);
  }

}

void AV_vel_cb(const geometry_msgs::Twist& AV_vel_msg)  //callback function from subscribe
{
  float const accel_rate = 1.5;
  float const accel_max = pow(2,15)-1;
  int corner_accel = accel_max * accel_rate;
  
  drive_duty_cycle = AV_vel_msg.linear.x;//val: -63 <-> 63
  int turn = AV_vel_msg.angular.z + 780;
  enable = true;
  
  if (enable)
  {
    if (turn < 780)
    {
      roboclaw.SpeedAccelDeccelPositionM1(address3,corner_accel,1000,corner_accel,turn,1);    //Right_Corner    //  roboclaw.SpeedAccelDeccelPositionM1(address3,26213,1000,26213,6,1);    //Right_Turn
      roboclaw.SpeedAccelDeccelPositionM2(address3,corner_accel,1000,corner_accel,turn,1);                      //  roboclaw.SpeedAccelDeccelPositionM2(address3,26213,1000,26213,7,1);
    }
    else if (turn > 780)
    {
      roboclaw.SpeedAccelDeccelPositionM1(address3,corner_accel,1000,corner_accel,turn,1);   //Left_Corner       //  roboclaw.SpeedAccelDeccelPositionM1(address3,26213,1000,26213,1559,1);    //Left_Turn 
      roboclaw.SpeedAccelDeccelPositionM2(address3,corner_accel,1000,corner_accel,turn,1);                       //  roboclaw.SpeedAccelDeccelPositionM2(address3,26213,1000,26213,1564,1);
    }
    else
    {
      roboclaw.SpeedAccelDeccelPositionM1(address3,corner_accel,1000,corner_accel,777,1);  //Middle_Corner
      roboclaw.SpeedAccelDeccelPositionM2(address3,corner_accel,1000,corner_accel,750,1);
    }
  
    if (drive_duty_cycle > 0)
    {
      roboclaw.ForwardM1(address1, drive_duty_cycle);
      roboclaw.ForwardM2(address1, drive_duty_cycle);
      roboclaw.ForwardM1(address2, drive_duty_cycle);
      roboclaw.ForwardM2(address2, drive_duty_cycle);
    }
    else
    {
      roboclaw.BackwardM1(address1, drive_duty_cycle * -1);
      roboclaw.BackwardM2(address1, drive_duty_cycle * -1);
      roboclaw.BackwardM1(address2, drive_duty_cycle * -1);
      roboclaw.BackwardM2(address2, drive_duty_cycle * -1);
    }
  }


}