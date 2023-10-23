#!/bin/bash

# Run your ROS launch file
bash -c ". /home/jetson/rover_ws/devel/setup.bash"
bash -c ". /opt/ros/noetic/setup.bash"

bash -i -c "roslaunch av_senior av.launch"

