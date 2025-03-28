
# 목차
```
1. 노드 흐름 
2. 미완성인 부분
3. 주요 코드 위치
4. 빌드 방법
5. 실행 방법
6. 에러 해결책
```

### 1. 노드 정리
![Image](https://github.com/user-attachments/assets/dc8e1e8d-d530-452b-b5ec-ad2643d34285)
![Image](https://github.com/user-attachments/assets/0386969d-4bd5-4cd6-8fb1-6ef1d616c365)



### 2. 미완성인 부분
___
***
뭔가 더 있을것같은데 중요한 부분만 적었습니다

~~1. 위의 사진에서 알 수 있듯이 가운데 노드가 빠져있음 - 이미 코드는 기상선배가 notion에 올려놓은 상태 아직 코드를 합치지 못함~~  
-> 3/22 토요일 성공  
~~2. 음원 cmakelist 2개여서 어떻게 해야하는지 모르겠음 -> python으로 할껀지 cmake2개를 해결해서 할껀지 정해서 해결해야함~~   
-> 3/20일 해결 (python으로 실행)  
~~3. 명령어가 너무 많기에 한번에 해주는 launch 파일 같은거 제작~~     
-> 1개의 거대한 main파일 만드는 걸로 대체 성공  
~~4. 실제 로봇에 연결 테스트~~  
-> 3/20 1차 데모 해결 -> 3/21 main 코드 성공 -> 3/22 aruco_tag 합쳐서 총 데모 성공
***
___


### 3. 주요 코드 위치
```
라이다의 경우 압축을 풀면 됩니다  : $ unzip ros2_4leg_robot.zip
라이다 데이터 pub             : ros2_4leg_robot/src/ros2_robot_rider.py
받은 라이다 데이터를 움직임으로   : robot/src/my_test_pkg_py/control.py
음원에서 나온 각도를 움직임으로   : Sound/final/main.cpp
aruco_tag 인식해서 움직임으로   : quad25_ws-main/src/tag_follow/fake_robot.py
                            : quad25_ws-main/src/tag_follow/follwer.py
                            : quad25_ws-main/src/tag_follow/tag_location.py
```


### 4. 빌드 방법
```
라이다 
$ unzip ros2_4leg_robot.zip
$ cd ros2_4leg_robot
$ colcon build
$ source install/setup.bash

aruco_tag 파일
$ cd quad25_ws-main
$ colcon build
$ source install/local_setup.bash

오디오 - 사전 requirement 설치해야합니다 위치 : Sound/requirement.txt
$ cd Sound
$ mkdir build
$ cd build
$ cmake ..
$ make

main_control 코드
$ cd robot
$ colcon build --symlink-install --packages-select my_test_pkg_py
$ source install/setup.bash

cyclonedds 연결방법 
"노션 -> 회의록 -> 2/22 연구원님 미팅" 참고
```


### 5. 실행 방법
```
만약 setup.py 혹은 cmakelist를 수정하였다면 빌드 과정 다시 진행
실행코드만 수정하였다면 source install/setup.bash 만 수행

1번 터미널 - 라이다 rviz
$ cd ros2_4leg_robot 
$ source install/setup.bash
$ ros2 launch rplidar_ros view_rplidar.launch.py

2번 터미널 - 라이다 pub node
$ cd ros2_4leg_robot
$ python3 ros2_robot_ridar.py

3번 터미널 - aruco_tag 
$ cd quad25_ws-main 
$ source install/local_setup.py
$ ros2 launch tag_follow full.launch.py

4번 터미널
$ cd robot
$ python3 main.py


예상 출력값
```


### 6. 에러 해결책
```
1. 환경
2. ros2 launch rplidar_ros view_rplidar.launch.py 실행시 라이다 rviz 빨간점들이 보이지 않을때 
3. 주소 위치 에러
4. main 못 찾았습니다 에러
5. 기타 
```

##### 1. 환경 
```
듀얼 부팅으로 ubuntu 설치되어야함 
anaconda와 같이 기본 python 위치를 바꾸는 프로그램 제거 필요
```

##### 2. ros2 launch rplidar_ros view_rplidar.launch.py 실행시 라이다 rviz 빨간점들이 보이지 않을때 
```
아래는 ttyUSB0의 읽고 쓸 권한을 부여한다. 

$ sudo chmod 777 /dev/ttyUSB0
안되면 777 대신 666써보자

아래는 ttyUSB0에 연결이 되어있는지 확인하는 코드이다. rplidar a1 package를 까보면 보이는데
여기선 default 값이 ttyUSB0를 기본으로 가져간다.

$ ls /dev/ttyUSB*
/dev/ttyUSB0가 나오면, 연결이 되어있다는 뜻이다.

포트가 충돌로 다른 프로세스가 사용하고 있다면, 연결이 되어있어도 작동이 안될수 있다.
$ sudo lsof /dev/ttyUSB0
위 과정은 현재 사용중인 프로세스를 보여준다. 만약 뭔가 있다면
$ sudo kill -9 <PID>
위 과정으로 제거한다.

권한의 마지막 과정이다. 위의 ttyUSB0를 쓸 권한을 부여했다.
본 사용자가 이 권한을 획득하는 것은 또 다른 문제인듯하다. 따라서 본 사용자가 권한이 있는지 확인해야한다.

$ ls -l /dev/ttyUSB0
crw-rw---- 1 root dialout 188, 0 ...
위의 코드는 포트에 대해 부여받은 권한이 있는지 확인하는 것이고, 윗줄의 주석처럼 결과가 나오면 성공이다

이제 본 사용자가 dialout 그룹에 포함되어있어야 본사용자가 ttyUSB0를 쓸수 있다.
$ groups $USER
이 코드의 결과로 dialout이 없다면,
$ sudo usermod -a -G dialout $USER
권한을 부여하면된다. 이후 bash shell 을 끈 후 다시 키면 된다.
```

##### 3. ros2 run my_test_pkg_py control 실행시 pt의 위치가 잘못 되었다고 나올때
```
control.py의 146번째 줄의 /home/muwon/sound_and_test 부분을 본인에 맞게 수정

self.model.load_state_dict(torch.load('/home/muwon/sound_and_test/robot_connection-master/src/my_test_pkg_py/my_test_pkg_py/pt/4action_4_model.pt'))


quad25_ws-main/src/tag_follow/tag_location.py의 44번째 줄 본인에 맞게 수정
inner_matrix = np.load("/home/havi/quad25_ws-main/src/tag_follow/matrix.npy")
distortion = np.load("/home/havi/quad25_ws-main/src/tag_follow/distortion.npy")
```


##### 4. 
```
코드 문제일 가능성이 높습니다 연락부탁드립니다.
```

##### 5. 기타 
```
[0.202s] WARNING:colcon.colcon_core.prefix_path.colcon:The path '/home/havi/Downloads/Lider_sound_move-main/robot_connection-master/install' in the environment variable COLCON_PREFIX_PATH doesn't exist
[0.202s] WARNING:colcon.colcon_core.prefix_path.colcon:The path '/home/havi/lidar_sound_move/robot_connection-master/install' in the environment variable COLCON_PREFIX_PATH doesn't exist
[0.202s] WARNING:colcon.colcon_ros.prefix_path.ament:The path '/home/havi/Downloads/Lider_sound_move-main/robot_connection-master/install/my_test_pkg_py' in the environment variable AMENT_PREFIX_PATH doesn't exist
[0.202s] WARNING:colcon.colcon_ros.prefix_path.ament:The path '/home/havi/lidar_sound_move/robot_connection-master/install/my_test_pkg_py' in the environment variable AMENT_PREFIX_PATH doesn't exist

위 4개중 몇개가 발생했다면 아래 명령어 실행후 다시 colcon build 하기
$ unset COLCON_PREFIX_PATH
$ unset AMENT_PREFIX_PATH
```
