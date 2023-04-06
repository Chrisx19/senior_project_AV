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
bool g_motor_enable = false;


void AV_vel_cb(const geometry_msgs::Twist& AV_vel_msg);
void forward_backward(int val);
void right_left(int turn);
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
  
  motor_stop();
}

void loop()
{
  vel_cmd.publish( &velocity );
  nh.spinOnce();
//  delay(1);
}

void motor_stop(void)
{
  uint8_t rc[3] = {address1, address2, address3};
  for(int i = 0; i < 3; i++)
  {
    roboclaw.ForwardM1(rc[i], 0);
  }
}

void AV_vel_cb(const geometry_msgs::Twist& AV_vel_msg)  //callback function from subscribe
{
  drive_duty_cycle = AV_vel_msg.linear.x;           //val: -63 <-> 63
  int corner = AV_vel_msg.angular.z + 780;

  forward_backward(drive_duty_cycle);
  right_left(corner);
}

void forward_backward(int val)
{
  if(val > 0)
  {
    roboclaw.ForwardM1(address1, val);
    roboclaw.ForwardM2(address1, val);
    roboclaw.ForwardM1(address2, val);
    roboclaw.ForwardM2(address2, val);
  }
  else
  {
    roboclaw.BackwardM1(address1, val * -1);
    roboclaw.BackwardM2(address1, val * -1);
    roboclaw.BackwardM1(address2, val * -1);
    roboclaw.BackwardM2(address2, val * -1);
  }
}

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
}