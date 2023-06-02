#!/usr/bin/env python3
import rospy
import time
from math import atan2, sqrt, radians, pi, sin, cos
from std_msgs.msg import Float32
from sensor_msgs.msg import NavSatFix
from geometry_msgs.msg import Twist

class Rpi(object):
    def __init__(self):

        self.wheel_diameter = 0.135
        self.velocity = 0.4

        self.current_lat = 0
        self.current_long = 0
        self.goal_lat = 34.0574836
        self.goal_long = -117.8221183

        self.cmd_vel_pub = rospy.Publisher("/cmd_vel_AV", Twist, queue_size = 1)
        self.lat_pub = rospy.Publisher("/pub_lat", Float32, queue_size = 1)
        self.gps_sub     = rospy.Subscriber("/fix", NavSatFix, self.gps_cb)

    def gps_cb(self, gps_msg):
        self.current_lat = gps_msg.latitude
        self.current_long = gps_msg.longitude
        
        R = 6378.137
        dLat = self.goal_lat * pi / 180 - self.current_lat * pi / 180
        dLong = self.goal_long * pi / 180 - self.current_long * pi / 180
        a = sin(dLat / 2) * sin(dLat / 2) + cos(self.current_lat * pi /180) * cos(self.goal_lat * pi / 180) * sin(dLong / 2) * sin(dLong / 2)
        c = 2 * atan2(sqrt(a), sqrt(1 - a))
        distance = (R * c) * 1000 # in meters conversion

        # distance = sqrt( pow(self.goal_lat - self.current_lat, 2) + pow(self.goal_long - self.current_long, 2))

        goal_time = distance / self.velocity

        print(distance)
        
        print(goal_time)
        print('')
        print('')

    def degree_to_meter(self, lat1, lon1, lat2, lon2):
    # Radius of earth in KM
        R = 6378.137
        dLat = radians(lat2 - lat1)
        dLon = radians(lon2 - lon1)
        a = sin(dLat/2) * sin(dLat/2) + \
            cos(radians(lat1)) * cos(radians(lat2)) * \
            sin(dLon/2) * sin(dLon/2)
        c = 2 * atan2(sqrt(a), sqrt(1-a))
        d = R * c
        # meters
        return d * 1000

    def qpps_conversion(self, qpps):
        wheel_radius = self.wheel_diameter / 2
        ppr_motor = 537.7 
        wheel_circumference = 2 * pi * wheel_radius

        meter_per_sec = wheel_circumference * (1 / ppr_motor) * qpps

        return meter_per_sec

if __name__ == "__main__":
    rospy.init_node('Auto_mode', anonymous = False)
    rospy.loginfo("Starting Jetson node...")
    Auto_mode = Rpi()
    rospy.spin()
