#!/bin/bash

echo "==== 📦 빌드 시작합니다... ===="

# 1. LiDAR 워크스페이스 빌드
echo "➡️  ros2_4leg_robot 빌드 중..."
cd ~/Lider_sound_move-main/ros2_4leg_robot/src
colcon build
source install/setup.bash

# 2. ArUco 태그 워크스페이스 빌드
echo "➡️  quad25_ws-main 빌드 중..."
cd ~/Lider_sound_move-main/quad25_ws-main
colcon build
source install/local_setup.bash

# 3. Main control workspace 빌드
echo "➡️  robot 빌드 중..."
cd ~/Lider_sound_move-main/robot
colcon build --symlink-install --packages-select my_test_pkg_py
source install/setup.bash

# 4. Sound (CMake 방식) 빌드
echo "➡️  Sound(C++) 빌드 중..."
cd ~/Lider_sound_move-main/Sound/final
mkdir -p build
cd build
cmake ..
make

echo "✅ 빌드 완료!"

