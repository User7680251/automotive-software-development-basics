catkin_make --only-pkg-with-deps task4_package
source devel/setup.bash
roslaunch task4_package task4.launch
roslaunch task4_package test.launch

# A 8.4196; 6.0695; -1.2017
# B 5.8831; 4.0171; -0.77925
# C 9.2778; 2.3851; -1.3492
# D 7.2339; -1.0274; -1.0687