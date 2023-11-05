rosrun camera_calibration cameracalibrator.py --size 8x6 --square 0.024 image:=/camera/color/image_raw
rosbag record -O my_bag.bag /camera/color/image_raw