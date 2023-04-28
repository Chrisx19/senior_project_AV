#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <av_senior/Arduino_telemetry.h>
#include <SoftwareSerial.h>
#include "RoboClaw.h"
#include <math.h>

#define address1 0x80
#define address2 0x81
#define address3 0x82     //corner_motor

#define accel 1000

const int trig1 = 12;
const int echo1 = 13;

const int analog_in_right = A0;
const int analog_in_left = A1;

int16_t drive_duty_cycle = 0;

// ros::NodeHandle  nh;
ros::NodeHandle_<ArduinoHardware, 5, 5, 512, 512> nh;

SoftwareSerial serial(10, 11);     //serial (rx, tx) can use pin 6 for tx
RoboClaw roboclaw(&serial, 10000); //10000
//==========================================================================Global================================================================================
void AV_vel_cb(const geometry_msgs::Twist& AV_vel_msg);
void corner_drive(int turn, int drive);
void motor_stop(void);
int sensor1(int delay_sonar);
void publishSpeed(int time);

ros::Subscriber<geometry_msgs::Twist> AV_vel_sub("cmd_vel_AV", &AV_vel_cb);

av_senior::Arduino_telemetry distance_data;
ros::Publisher distance_pub("distance_ll", &distance_data);
//==========================================================================Setup and Loop================================================================================
void setup()
{
  roboclaw.begin(115200);
  // nh.getHardware()->setBaud(38400);

  nh.initNode();
  nh.advertise(distance_pub);
  nh.subscribe(AV_vel_sub);

  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  // motor_stop();
}
  int i = 0;
void loop()
{
  // int test =analogRead(analog_in_right);
  // int test2 = analogRead(analog_in_left);
  // distance_data.Right_Distance = analogRead(analog_in_right);
  // delay(50);
  // distance_data.Mid_Distance = sensor1(1000);
  // distance_data.Left_Distance = analogRead(analog_in_left);
  // delay(50);

  // distance_pub.publish( &distance_data);
  publishSpeed(100);

  nh.spinOnce();
  // delay(1);
}
//==========================================================================Functions================================================================================
void publishSpeed(int time)
{
  distance_data.Right_Distance = analogRead(analog_in_right);
  distance_data.Mid_Distance   = sensor1(1000);
  distance_data.Left_Distance  = analogRead(analog_in_left);
  delay(time);
  distance_pub.publish( &distance_data);
}


int sensor1(int delay_sonar)
{
  digitalWrite(trig1, LOW);
  delayMicroseconds(delay_sonar);
  digitalWrite(trig1, HIGH);
  delayMicroseconds(delay_sonar);
  digitalWrite(trig1, LOW);

  long time = pulseIn (echo1, HIGH);
  int cm_distance = (time / 29.1 / 2);
  return cm_distance;
}

void AV_vel_cb(const geometry_msgs::Twist& AV_vel_msg)  //callback function from subscribe
{
  drive_duty_cycle = AV_vel_msg.linear.x;           //val: -2800 <-> 2800
  int corner = AV_vel_msg.angular.z + 780;          //middle: 780

  // uint16_t const deccel = 2800;

  // if(35 >= center_sensor) 
  // { 
      // roboclaw.SpeedAccelM1M2(address1, deccel, -400, -400);
      // roboclaw.SpeedAccelM1M2(address2, deccel, -400, -400);
  // }
  // else
  // {
  // if (drive_duty_cycle == 0) 
  // {
    // motor_stop();
  // }
  // else
  // {
    corner_drive(corner, drive_duty_cycle);
  // }
  // }

}//end fo AV_vel_cb

void corner_drive(int turn, int drive)
{
  float accel_rate = 1.5;
  float accel_max = pow(2,15)-1;
  int corner_accel = accel_max * accel_rate;

  if (turn < 780)
  {
    roboclaw.SpeedAccelDeccelPositionM1(address3, corner_accel, 1000, corner_accel, turn, 1);    //Right_Corner    //  roboclaw.SpeedAccelDeccelPositionM1(address3,26213,1000,26213,6,1);    //Right_Turn
    roboclaw.SpeedAccelDeccelPositionM2(address3, corner_accel, 1000, corner_accel, turn, 1);                      //  roboclaw.SpeedAccelDeccelPositionM2(address3,26213,1000,26213,7,1);
    
    roboclaw.SpeedAccelM1M2(address1, accel, drive * 1/2, drive * 1/2);
    roboclaw.SpeedAccelM1M2(address2, accel, drive, drive);
  
  }
  else if (turn > 780)
  {
    roboclaw.SpeedAccelDeccelPositionM1(address3, corner_accel, 1000, corner_accel, turn, 1);   //Left_Corner       //  roboclaw.SpeedAccelDeccelPositionM1(address3,26213,1000,26213,1559,1);    //Left_Turn 
    roboclaw.SpeedAccelDeccelPositionM2(address3, corner_accel, 1000, corner_accel, turn, 1);                       //  roboclaw.SpeedAccelDeccelPositionM2(address3,26213,1000,26213,1564,1);
  
    roboclaw.SpeedAccelM1M2(address1, accel, drive, drive);
    roboclaw.SpeedAccelM1M2(address2, accel, drive * 1/2, drive * 1/2);
  }
  else
  {
    roboclaw.SpeedAccelDeccelPositionM1(address3, corner_accel, 1000, corner_accel, 777, 1);  //Middle_Corner
    roboclaw.SpeedAccelDeccelPositionM2(address3, corner_accel, 1000, corner_accel, 750, 1);
    roboclaw.SpeedAccelM1M2(address1, accel, drive, drive);
    roboclaw.SpeedAccelM1M2(address2, accel, drive, drive);
  }
}//end of corner_drive

void motor_stop(void)
{
  uint8_t rc[3] = {address1, address2, address3};
  for(int i = 0; i < 3; i++)
  {
    roboclaw.SpeedAccelM1M2(rc[i], 2800, 0, 0);
  }
}//end of motor_stop