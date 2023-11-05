#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import rospy
import actionlib
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from std_msgs.msg import String
from sensor_msgs.msg import Image
from std_msgs.msg import String
from actionlib_msgs.msg import GoalStatus


def callback(msg):
    message = msg.data
    rospy.loginfo("receive target %s"%message)
    x, y = 0, 0
    if(message == "A"):
        x, y = 8.4196, 6.0695
    if(message == "B"):
        x, y = 5.8831, 4.0171
    if(message == "C"):
        x, y = 9.2778, 2.3851
    if(message == "D"):
        x, y = 7.2339, -1.0274
    set_goal_position(x, y)

def set_goal_position(x, y):
    # 创建一个SimpleActionClient，连接到move_base服务器
    rospy.loginfo("set target")
    client = actionlib.SimpleActionClient('move_base', MoveBaseAction)
    client.wait_for_server()

    # 创建一个MoveBaseGoal对象，并设置目标位置
    goal = MoveBaseGoal()
    goal.target_pose.header.frame_id = 'map'
    goal.target_pose.pose.position.x = x
    goal.target_pose.pose.position.y = y
    goal.target_pose.pose.orientation.z = 0
    goal.target_pose.pose.orientation.w = 1.0

    # 发送目标位置到move_base服务器
    client.send_goal(goal)
    rospy.loginfo("set target success")

    # 等待移动完成
    client.wait_for_result(rospy.Duration.from_sec(11.4514))

    # 获取移动的结果
    result = client.get_state()

    # 输出移动结果
    if result == GoalStatus.SUCCEEDED:
        rospy.loginfo("\033[1;32m mission success\033[0m")
    else:
        rospy.loginfo("\033[1;31m mission failed\033[0m")

def nav_goal_node():
    rospy.init_node('nav_goal_node', anonymous=True)
    rospy.loginfo("\033[1;32m----> nav_goal_node Start\033[0m");   
    
    # 创建订阅者
    rospy.Subscriber("/task4/recognized_letter_result", String, callback)

    rospy.spin()


if __name__ == '__main__':
    try:
        nav_goal_node()
    except rospy.ROSInterruptException:
        pass