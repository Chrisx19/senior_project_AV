#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Bool.h>
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
int16_t corner = 0;
int16_t turn = 0;

bool manual_controller = true;

int right_sensor, middle_sensor, left_sensor;


// ros::NodeHandle  nh;
ros::NodeHandle_<ArduinoHardware, 5, 5, 512, 512> nh;

SoftwareSerial serial(10, 11);     //serial (rx, tx) can use pin 6 for tx
RoboClaw roboclaw(&serial, 10000); //10000
//==========================================================================Global================================================================================
void AV_vel_cb(const geometry_msgs::Twist& AV_vel_msg);
void AV_manual_en_cb(const std_msgs::Bool& AV_vel_manual_msg);
void corner_drive(int turn, int drive);
void motor_stop(void);
int sensor1(int delay_sonar);
void publishSpeed(int time);

ros::Subscriber<geometry_msgs::Twist> AV_vel_sub("cmd_vel_AV", &AV_vel_cb);
ros::Subscriber<std_msgs::Bool> AV_manual_EN_sub("manual_en", &AV_manual_en_cb);

av_senior::Arduino_telemetry distance_data;
ros::Publisher distance_pub("distance_ll", &distance_data);
//==========================================================================Setup and Loop================================================================================
void setup()
{
  roboclaw.begin(115200);
  // nh.getHardware()->setBaud(38400);
  motor_stop();
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);

  nh.initNode();
  nh.advertise(distance_pub);
  nh.subscribe(AV_vel_sub);
  nh.subscribe(AV_manual_EN_sub);
}

void loop()
{
  right_sensor = analogRead(analog_in_right);
  middle_sensor = sensor1(500);
  left_sensor = analogRead(analog_in_left);
 
  if (manual_controller == false)
  {
    drive_duty_cycle = 500;

    if (middle_sensor < 40)
    {
      drive_duty_cycle = -500;
      // Object in front and to the right => Turn Left
      if(right_sensor > 150 && middle_sensor < 30)
      {
        turn = -250; // Turn half left
      }
      // Object in front and to the left => Turn Right
      else if(middle_sensor < 30 && left_sensor > 150)
      {
        turn = 250; // Turn half right
      }
      // Object is close to all sensors => Reverse or go backwards
      else if(right_sensor > 150 && middle_sensor < 20 && left_sensor > 150)
      {
        turn = 0; // Set Turn angle to 0 or face straight
        drive_duty_cycle = -250; // Set reverse speed to 1/4 of max speed
      }
      // Object very close to right side => Turn Hard Left 
    }
    else if (right_sensor > 100)
    {
      turn = -490;
    }
    else if (left_sensor > 100)
    {
      turn = 490;
    }
    corner = turn + 780;
    corner_drive(corner, drive_duty_cycle);
  }

  // publishSpeed(10);

  nh.spinOnce();
  // delay(1);
}

//==========================================================================Functions================================================================================

void publishSpeed(int time)
{
  distance_data.Right_Distance = right_sensor;
  distance_data.Mid_Distance   = middle_sensor;
  distance_data.Left_Distance  = left_sensor;
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


void AV_manual_en_cb(const std_msgs::Bool& AV_vel_manual_msg)
{
  manual_controller = AV_vel_manual_msg.data;
}

void AV_vel_cb(const geometry_msgs::Twist& AV_vel_msg)  //callback function from subscribe

{
  drive_duty_cycle = AV_vel_msg.linear.x;           //val: -2800 <-> 2800
  turn = AV_vel_msg.angular.z;
  corner = turn + 780;          //middle: 780

    if (manual_controller)
    {
      corner_drive(corner, drive_duty_cycle);
    }

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