from Sound.final.control_degree import *  # 또는 필요한 함수/클래스
from robot.src.my_test_pkg_py.my_test_pkg_py.control import *  # 또는 필요한 함수/클래스


def main():
    # Sound.final.control_degree 모듈의 함수/클래스 사용
    # robot.src.my_test_pkg_py.my_test_pkg_py 모듈의 함수/클래스 사용
    initial = 0
    initial = ControlDegree()
    #aruco tag code
    control = Control()
    while(initial) :
        control.Control()
        if control.lidar.action == 0:
            initial = initial +1
            if initial == 10:
                break
    
if __name__ == '__main__':
    main()