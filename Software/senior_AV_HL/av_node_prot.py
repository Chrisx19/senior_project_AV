#!/usr/bin/env python3
import rospy
from math import pi
from rpi_node.msg import CmdVelUgv
from std_msgs.msg import Float32
from geometry_msgs.msg import Twist
from sensor_msgs.msg import Joy

class Vechicle(object):
    def __init__(self):
        self.quad_pps = 2700 #max is somewhere 26xx
        self.max_corner_turn = 600
    

        self.cmd_pub = rospy.Publisher("/cmd_vel_AV", Twist, queue_size = 10)
        self.speed_pub = rospy.Publisher("/speed_av", Float32, queue_size = 10)
        self.joy_sub = rospy.Subscriber("/joy", Joy, self.joy_cb, queue_size = 10)

    def joy_cb(self, joy_msg):
        # vel = CmdVelUgv()
        vel = Twist()
        
        speed_pulse  = joy_msg.axes[1] * self.quad_pps
        ackerman     = joy_msg.axes[3] * self.max_corner_turn
        en_button    = joy_msg.buttons[5]

        speed = self.qpps_conversion(speed_pulse)
        vel.linear.x = int(speed_pulse)
        vel.angular.z = int(ackerman)

        self.speed_pub.publish(speed)

        if(en_button>0): 
            self.cmd_pub.publish(vel)
        else:
            vel.linear.x = 0

    # def pwm_mode(self, duty):
    #     self.quad_pps = 1500

    def qpps_conversion(self, qpps):
        wheel_radius = 0.076
        ppr_motor = 537.7 
        wheel_circumference = 2 * pi * wheel_radius

        meter_per_sec = wheel_circumference * (1 / ppr_motor) * qpps

        return meter_per_sec


if __name__ == "__main__":
    rospy.init_node("ugv_joy", log_level=rospy.INFO)
    rospy.loginfo("Starting AV Node")
    joy = Vechicle()
    rospy.spin()