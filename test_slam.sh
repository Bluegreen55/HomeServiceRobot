#!/bin/sh

#xterm -e " source /opt/kinetic/setup.bash; roscore " &

#sleep 5

xterm -e " roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=/home/workspace/catkin_ws/src/my_robot/worlds/jenna2.world" &

sleep 5

xterm -e " roslaunch turtlebot_gazebo gmapping_demo.launch " &

sleep 5

xterm -e " roslaunch turtlebot_rviz_launchers view_navigation.launch " &

sleep 5

xterm -hold -e " roslaunch turtlebot_teleop keyboard_teleop.launch" 
#xterm -e " rosrun teleop_twist_keyboard teleop_twist_keyboard.py" 
 
#sleep 5
