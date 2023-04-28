#!/usr/bin/env python3
import rospy
<<<<<<< HEAD
from math import pi,radians, sin, cos, atan2, sqrt
from std_msgs.msg import Float32
from geometry_msgs.msg import Twist
from sensor_msgs.msg import Joy
from av_senior.msg import Arduino_telemetry

class Vechicle(object):
    def __init__(self):
        self.quad_pps = 1800 #max is somewhere 26xx
        self.max_corner_turn = 500
        self.wheel_diameter = 0.135
=======
from math import pi
from rpi_node.msg import CmdVelUgv
from std_msgs.msg import Float32
from geometry_msgs.msg import Twist
from sensor_msgs.msg import Joy

class Vechicle(object):
    def __init__(self):
        self.quad_pps = 2700 #max is somewhere 26xx
        self.max_corner_turn = 0
>>>>>>> 1ece3bd2fa72e01d24d2201eb7f579aea1bcd7ff
    

        self.cmd_pub = rospy.Publisher("/cmd_vel_AV", Twist, queue_size = 10)
        self.speed_pub = rospy.Publisher("/speed_av", Float32, queue_size = 10)
        self.joy_sub = rospy.Subscriber("/joy", Joy, self.joy_cb, queue_size = 10)
<<<<<<< HEAD
        # self.distance_sub = rospy.Subscriber("/distance_ll", Arduino_telemetry, self.distance_cb, queue_size = 10)

    # def distance_cb(self, disntace_msg):
    



=======
>>>>>>> 1ece3bd2fa72e01d24d2201eb7f579aea1bcd7ff

    def joy_cb(self, joy_msg):
        # vel = CmdVelUgv()
        vel = Twist()
        
<<<<<<< HEAD
        speed_pulse  = joy_msg.axes[3] * self.quad_pps
        ackerman     = joy_msg.axes[0] * self.max_corner_turn * -1
        en_button    = joy_msg.buttons[7]
        fast_duty_button = joy_msg.buttons[6]
=======
        speed_pulse       = joy_msg.axes[1] * self.quad_pps
        ackerman     = joy_msg.axes[3] * self.max_corner_turn
        en_button = joy_msg.buttons[5]
>>>>>>> 1ece3bd2fa72e01d24d2201eb7f579aea1bcd7ff

        speed = self.qpps_conversion(speed_pulse)
        vel.linear.x = int(speed_pulse)
        vel.angular.z = int(ackerman)
<<<<<<< HEAD
        
        if(en_button > 0): 
            vel.linear.x = int(speed_pulse * 2/3)
            if (en_button > 0 and fast_duty_button > 0):
                vel.linear.x = int(speed_pulse)
            self.cmd_pub.publish(vel)
        else:
            vel.linear.x = 0
            self.cmd_pub.publish(vel)
        self.speed_pub.publish(speed)

    def qpps_conversion(self, qpps):
        wheel_radius = self.wheel_diameter / 2
=======

        self.speed_pub.publish(speed)

        if(en_button>0): 
            self.cmd_pub.publish(vel)

    def qpps_conversion(self, qpps):
        wheel_radius = 0.076
>>>>>>> 1ece3bd2fa72e01d24d2201eb7f579aea1bcd7ff
        ppr_motor = 537.7 
        wheel_circumference = 2 * pi * wheel_radius

        meter_per_sec = wheel_circumference * (1 / ppr_motor) * qpps

        return meter_per_sec

<<<<<<< HEAD
if __name__ == "__main__":
    rospy.init_node("av_node", log_level=rospy.INFO)
    rospy.loginfo("Starting AV Node")
    av = Vechicle()
    rospy.spin()
=======

if __name__ == "__main__":
    rospy.init_node("ugv_joy", log_level=rospy.INFO)
    rospy.loginfo("Starting AV Node")
    joy = Vechicle()
    rospy.spin()
>>>>>>> 1ece3bd2fa72e01d24d2201eb7f579aea1bcd7ff
