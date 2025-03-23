"""
Aruco Tag follower
1) subscribe transition and distance message
2) calculate linear & angular velocity
3) publish twist topic to /command/Action

"""


import numpy as np
import cv2
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Point, PoseStamped, Twist
from std_msgs.msg import Bool, UInt32 #Float32MultiArray
from rclpy.qos import QoSProfile


def calc_velocity(transition, mode):
    velocity = Twist()
    #mode received by string
    #1) linear velocity calculation with z

    if transition[2] < 100: #under 30cm
        velocity.linear.x = -0.4
    elif transition[2] > 100:
        velocity.linear.x = 0.4
    else:
        velocity.linear.x = 0.0
    
    #2. angular velocity calculation -> differ with mode
    if transition[0] > 20: #horizontal movement over 20cm
        if mode == "revolute":
            velocity.angular.z = -0.4
        elif mode == "prismatic":
            velocity.linear.y = 1.0
        else:
            print("unknown mode")# or logger... then should be the method of FOLLOWER

    elif transition[0] < -20:
        if mode == "revolute":
            velocity.angular.z = 0.4
        elif mode == "prismatic":
            velocity.linear.y = -1.0
        else:
            print("unknown mode")
    
    return velocity


class FOLLOWER(Node):
    def __init__(self):
        super().__init__('follower')
        qos_profile = QoSProfile(depth=10)

        self.signal = 0
        self.transition = np.array([0.0,0.0,400.0])

        #subscriber
        self.tag_subscription = self.create_subscription(
            Point,
            '/tag_location', # length 3 vector of integer angles
            self.save_location,
            qos_profile
        )
        self.tag_subscription #prevent unused variable warning

        self.ret_subscription = self.create_subscription(
            Bool,
            '/tag_return', # length 3 vector of integer angles
            self.ret_check,
            qos_profile
        )
        self.ret_subscription #prevent unused variable warning

        #audio finish flag -> /signal, UInt32
        # 0 if audio
        # 1 if aruco tag
        # 2 control
        self.signal_subscription = self.create_subscription(
            UInt32,
            '/signal', # length 3 vector of integer angles
            self.signal_received,
            qos_profile
        )
        self.ret_subscription #prevent unused variable warning

        #publisher
        self.publisher = self.create_publisher(Twist,
        '/mcu/command/manual_twist', qos_profile)
        #publisher
        self.publisher_signal = self.create_publisher(UInt32,
        '/signal', qos_profile)

        #set timer
        self.timer = self.create_timer(0.1, self.publish_twist)

        self.flag = False

    def ret_check(self, msg):
        self.flag = msg.data

    def save_location(self, msg):
        #self.get_logger().info('transition received')
        self.transition = np.array([msg.x, msg.y, msg.z])


    def publish_twist(self):
        #if false, no frame or corner returned -> 0 velocity
        #if true, calculate and publish (if no published, no moving)
        if self.flag == True and self.signal == 1:
            velocity = calc_velocity(self.transition, "revolute") # or prismatic
            self.publisher.publish(velocity)

        if abs(self.transition[2] - 200.0) < 30 and abs(self.transition[0]) < 30 and self.signal == 1:
            self.signal = 2
            self.publisher_signal.publish(UInt32(data=2))
            self.get_logger().info("success")
        self.get_logger().info("signal: %d" %self.signal)


    def signal_received(self, msg):
        # when 0, do not publish twist
        # when 1
        self.signal = msg.data



def main(args=None):
    #main function call
    rclpy.init(args=args)
    node = FOLLOWER()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    """main function"""
    main()