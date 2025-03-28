#!/bin/bash
echo "==== 🚀 실행 시작합니다... ===="

# 각 실행을 새로운 터미널에서
gnome-terminal -- bash -c "cd ~/Lider_sound_move-main/ros2_4leg_robot; source install/setup.bash; ros2 launch rplidar_ros view_rplidar.launch.py; exec bash"
gnome-terminal -- bash -c "cd ~/Lider_sound_move-main/ros2_4leg_robot/src; source install/setup.bash; python3 ros2_robot_ridar.py; exec bash"
gnome-terminal -- bash -c "cd ~/Lider_sound_move-main/quad25_ws-main; source install/local_setup.bash; ros2 launch tag_follow full.launch.py; exec bash"
gnome-terminal -- bash -c "cd ~/Lider_sound_move-main; python3 main.py; exec bash"

echo "🎉 전체 시스템이 실행 중입니다!"
