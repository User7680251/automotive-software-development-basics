#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

import numpy as np
import rospy
import cv2
from sensor_msgs.msg import Image
from std_msgs.msg import String

from BaiduOCR import BaiduOCR


result_pub = None
previous_time = None
interval = rospy.Duration(1.0)  # 1秒的时间间隔

def image_callback(image_msg):
    assert isinstance(image_msg, Image)
    cv_img = np.frombuffer(image_msg.data, dtype=np.uint8).reshape(image_msg.height, image_msg.width, -1)

    global previous_time
    # 检查计时器是否已经启动
    if previous_time is None:
        # 第一次调用回调函数时，记录当前时间并执行OCR
        previous_time = rospy.Time.now()
        perform_ocr(cv_img)
    else:
        # 检查时间间隔是否已经过去
        current_time = rospy.Time.now()
        elapsed_time = current_time - previous_time
        if elapsed_time >= interval:
            # 时间间隔已经过去，执行OCR
            previous_time = current_time
            perform_ocr(cv_img)
        else:
            # 时间间隔还未过去，跳过OCR
            #rospy.loginfo("Skipping OCR due to time interval not elapsed.")
            pass

def perform_ocr(cv_img):
    # 使用baidu的api进行OCR
    result = BaiduOCR(cv_img)

    # 打印识别结果
    rospy.loginfo("letter recognition result: %s"%result)

    # 发布识别结果到ROS话题
    result_pub.publish(result)

def letter_recognition_node():
    global result_pub
    rospy.init_node('letter_recognition_node', anonymous=True)
    rospy.loginfo("\033[1;32m----> letter_recognition_node Start\033[0m");   

    # 创建识别结果发布者
    result_pub = rospy.Publisher("/task4/recognized_letter_result", String, queue_size=10)
    
    # 创建图像订阅者
    rospy.Subscriber("/camera/color/image_raw", Image, image_callback)

    # 创建一个rospy.Rate对象，设置识别频率为1Hz（每秒1次）
    rate = rospy.Rate(1)

    while not rospy.is_shutdown():
        # 等待1秒，控制识别频率为1Hz
        rate.sleep()


if __name__ == '__main__':
    try:
        letter_recognition_node()
    except rospy.ROSInterruptException:
        pass