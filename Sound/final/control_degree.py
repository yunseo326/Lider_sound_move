import subprocess 
import rclpy, time 
from rclpy.node import Node 
from std_msgs.msg import UInt32 
from geometry_msgs.msg import Vector3, Twist 
from nav_msgs.msg import Odometry 
from std_srvs.srv import Empty 

EXE_PATH = "/home/havi/Lider_sound_move-main/Sound/final/build/main"


def Audio():
    result = subprocess.run(EXE_PATH, capture_output=True, text=True)
    print(result.returncode)
    if result.returncode == 255 :
        print("error")
        return Audio()
    return result.returncode

"""
Example to move the robot in blind mode using ROS2 API without services.
"""

Iteration = 100
Speed_X = 0.5
Speed_Y = 0.5
Speed_Angular = 1.0

class TestMoveBlindNoService(Node):
    def __init__(self):
        super().__init__("MoveBlindNoService")
        self.pub_action = self.create_publisher(UInt32, '/command/setAction', 10)
        self.pub_run = self.create_publisher(UInt32, '/command/setRun', 10)
        self.pub_control_mode = self.create_publisher(UInt32, '/command/setControlMode', 10)
        self.pub_twist = self.create_publisher(Twist, '/mcu/command/manual_twist', 10)

    def Initialize(self):
        self.get_logger().info("Setting control mode=170")
        self.pub_control_mode.publish(UInt32(data=170))
        time.sleep(0.01)
        self.pub_action.publish(UInt32(data=1)) # stand
        time.sleep(1)

        self.get_logger().info("Setting action=2")
        self.pub_action.publish(UInt32(data=2)) # walk mode

    def TurnRight(self, angular):
        self.get_logger().info("Commanding TurnRight twist")
        for i in range(angular):
            self.pub_twist.publish(Twist(angular=Vector3(z=Speed_Angular)))
            time.sleep(0.01) # do this instead of sleep(2) to avoid timeout

        self.get_logger().info("Setting action=0")
        self.pub_twist.publish(Twist()) # zero twist

    def TurnLeft(self, angular):
        self.get_logger().info("Commanding TurnLeft twist")
        for i in range(angular):
            self.pub_twist.publish(Twist(angular=Vector3(z=-Speed_Angular)))
            time.sleep(0.01) # do this instead of sleep(2) to avoid timeout

        self.get_logger().info("Setting action=0")
        self.pub_twist.publish(Twist()) # zero twist

    def Endmode(self):
        self.pub_action.publish(UInt32(data=0)) # sit
        time.sleep(5)
        self.get_logger().info("Setting control mode=180")
        self.pub_control_mode.publish(UInt32(data=180))

class send2image(Node):
    def __init__(self):
        super().__init__("send2image")
        self.send2image_pub = self.create_publisher(UInt32, '/signal', 10)
        

def ControlDegree():
    rclpy.init(args=None)
    node = TestMoveBlindNoService()
    send_image_node = send2image()
    node.Initialize()
    while True:
        degree = Audio()
        if degree :
            if  0 < degree < 180 :
                step = degree // 30 * 100 
                node.TurnLeft(step)

            elif degree < 360 :
                step = (360 - degree) // 30 * 100
                node.TurnRight(step)
            print("audio")                
            send_image_node.send2image_pub.publish(UInt32(data=1))
            return 1
        else :
            print("error")

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__' :
    ControlDegree()
