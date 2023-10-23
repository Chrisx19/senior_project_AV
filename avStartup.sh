#!/bin/bash

# Source your ROS setup file
#source /opt/ros/noetic/setup.bash  # Replace <ROS-DISTRO> with your ROS distribution

# Navigate to your ROS workspace if needed
# cd /path/to/your/ros/workspace

# Run your ROS launch file
bash -c ". /home/jetson/rover_ws/devel/setup.bash"
bash -c ". /opt/ros/noetic/setup.bash"

bash -i -c "roslaunch av_senior av.launch"

