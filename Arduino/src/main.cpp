#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Bool.h>
#include <av_senior/Arduino_telemetry.h>
#include <SoftwareSerial.h>
#include "RoboClaw.h"
#include <math.h>

#define ROBOCLAW_ADDRESS1 0x80
#define ROBOCLAW_ADDRESS2 0x81
#define ROBOCLAW_ADDRESS3 0x82     //corner_motor

#define MIDDLE_CORNER 780
#define ACCELERATION 1000

const int analog_in_right = A0;
const int analog_in_left = A1;

int16_t g_drive_duty_cycle = 0;
int16_t g_corner = 0;
int16_t g_turn= 0;

// ros::NodeHandle  nh;
ros::NodeHandle_<ArduinoHardware, 5, 5, 512, 512> nh;
SoftwareSerial serial(10, 11);     //serial (rx, tx) can use pin 6 for tx
RoboClaw roboclaw(&serial, 10000); //10000
//==========================================================================Global================================================================================
void AV_vel_cb(const geometry_msgs::Twist& AV_vel_msg);
void corner_drive(int turn, int drive);
void motor_stop(void);

ros::Subscriber<geometry_msgs::Twist> AV_vel_sub("cmd_vel_AV", &AV_vel_cb);

// std_msgs::Int32 testPub;
// ros::Publisher testAdvertise("testPub", &testPub);
//==========================================================================Setup and Loop================================================================================
void setup()
{
  roboclaw.begin(115200);
  motor_stop();

  nh.initNode();
  // nh.advertise(testAdvertise);
  nh.subscribe(AV_vel_sub);
}

void loop()
{
  nh.spinOnce();
  // delay(1);
}

//==========================================================================Functions================================================================================
void AV_vel_cb(const geometry_msgs::Twist& AV_vel_msg)  //callback function from subscribe
{
  g_drive_duty_cycle = AV_vel_msg.linear.x;           //val: -2800 <-> 2800
  g_turn = AV_vel_msg.angular.z;
  g_corner = g_turn + MIDDLE_CORNER;     
  // testPub.data = g_corner; 
  // testAdvertise.publish(&testPub);

  corner_drive(g_corner, g_drive_duty_cycle);

}//end fo AV_vel_cb

void corner_drive(int turn, int drive)
{
  float accel_rate = 1.5;
  float accel_max = pow(2,15)-1;
  int corner_accel = accel_max * accel_rate;

  if (turn < 780)
  {
    roboclaw.SpeedAccelDeccelPositionM1(ROBOCLAW_ADDRESS3, corner_accel, 1000, corner_accel, turn, 1);    //Right_Corner    //  roboclaw.SpeedAccelDeccelPositionM1(ROBOCLAW_ADDRESS3,26213,1000,26213,6,1);    //Right_Turn
    roboclaw.SpeedAccelDeccelPositionM2(ROBOCLAW_ADDRESS3, corner_accel, 1000, corner_accel, turn, 1);                      //  roboclaw.SpeedAccelDeccelPositionM2(ROBOCLAW_ADDRESS3,26213,1000,26213,7,1);
    
    roboclaw.SpeedAccelM1M2(ROBOCLAW_ADDRESS1, ACCELERATION, drive * 1/2, drive * 1/2);
    roboclaw.SpeedAccelM1M2(ROBOCLAW_ADDRESS2, ACCELERATION, drive, drive);
  
  }
  else if (turn > 780)
  {
    roboclaw.SpeedAccelDeccelPositionM1(ROBOCLAW_ADDRESS3, corner_accel, 1000, corner_accel, turn, 1);   //Left_Corner       //  roboclaw.SpeedAccelDeccelPositionM1(ROBOCLAW_ADDRESS3,26213,1000,26213,1559,1);    //Left_Turn 
    roboclaw.SpeedAccelDeccelPositionM2(ROBOCLAW_ADDRESS3, corner_accel, 1000, corner_accel, turn, 1);                       //  roboclaw.SpeedAccelDeccelPositionM2(ROBOCLAW_ADDRESS3,26213,1000,26213,1564,1);
  
    roboclaw.SpeedAccelM1M2(ROBOCLAW_ADDRESS1, ACCELERATION, drive, drive);
    roboclaw.SpeedAccelM1M2(ROBOCLAW_ADDRESS2, ACCELERATION, drive * 1/2, drive * 1/2);
  }
  else
  {
    roboclaw.SpeedAccelDeccelPositionM1(ROBOCLAW_ADDRESS3, corner_accel, 1000, corner_accel, 777, 1);  //Middle_Corner
    roboclaw.SpeedAccelDeccelPositionM2(ROBOCLAW_ADDRESS3, corner_accel, 1000, corner_accel, 750, 1);
    roboclaw.SpeedAccelM1M2(ROBOCLAW_ADDRESS1, ACCELERATION, drive, drive);
    roboclaw.SpeedAccelM1M2(ROBOCLAW_ADDRESS2, ACCELERATION, drive, drive);
  }
}//end of corner_drive

void motor_stop(void)
{
  uint8_t rc[3] = {ROBOCLAW_ADDRESS1, ROBOCLAW_ADDRESS2, ROBOCLAW_ADDRESS3};
  for(int i = 0; i < 3; i++)
  {
    roboclaw.SpeedAccelM1M2(rc[i], 2800, 0, 0);
  }
}//end of motor_stop