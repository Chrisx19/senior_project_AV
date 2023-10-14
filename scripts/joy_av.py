#!/usr/bin/env python3
import rospy
from math import pi
from std_msgs.msg import Float32, Bool
from geometry_msgs.msg import Twist
from sensor_msgs.msg import Joy

class Vehicle(object):
    def __init__(self):
        self.quad_pps = 1800 #max is somewhere 26xx
        self.max_corner_turn = 500
        self.wheel_diameter = 0.135
    
        self.vel = Twist()
        self.drive  = 0
        self.corner = 0

        self.person = 0

        self.cmd_pub = rospy.Publisher("/cmd_vel_AV", Twist, queue_size = 10)
        self.speed_pub = rospy.Publisher("/speed_av", Float32, queue_size = 10)
        self.joy_sub = rospy.Subscriber("/joy", Joy, self.joy_cb, queue_size = 10)
        self.person_sub = rospy.Subscriber("/person", Float32, self.person_cb, queue_size = 10)
        self.manual_pub = rospy.Publisher("/manual_en", Bool, queue_size = 10)

    def person_cb(self, percent_msg):
        self.person = percent_msg.data
        
    def joy_cb(self, joy_msg):
        speed_pulse  = joy_msg.axes[3] * self.quad_pps
        ackerman     = joy_msg.axes[0] * self.max_corner_turn * -1
        en_button    = joy_msg.buttons[7]
        manual_enable= joy_msg.buttons[0]
        manual_disable = joy_msg.buttons[1]
        fast_duty_button = joy_msg.buttons[6]

        speed = self.qpps_conversion(speed_pulse)
        self.drive = int(speed_pulse)
        self.corner = int(ackerman)

        if(en_button > 0): 
            self.drive = int(speed_pulse * 2/3)


            if (self.person > 60):
                self.drive = 0
            else:
                if (manual_enable > 0):
                    manual = True
                    self.manual_pub.publish(manual)
                elif(manual_disable > 0):
                    manual = False
                    self.manual_pub.publish(manual)

                if (fast_duty_button > 0):
                    self.drive = int(speed_pulse)
                else:
                    pass

            self.vel.linear.x = self.drive
            self.vel.angular.z = self.corner
            self.cmd_pub.publish(self.vel)

        else:
            self.drive = 0
            self.vel.linear.x = self.drive
            self.cmd_pub.publish(self.vel)

        self.speed_pub.publish(speed)

    def qpps_conversion(self, qpps):
        wheel_radius = self.wheel_diameter / 2
        ppr_motor = 537.7 
        wheel_circumference = 2 * pi * wheel_radius

        meter_per_sec = wheel_circumference * (1 / ppr_motor) * qpps

        return meter_per_sec

if __name__ == "__main__":
    rospy.init_node("av_node", log_level=rospy.INFO)
    rospy.loginfo("Starting AV Node")
    av = Vehicle()
    rospy.spin()
