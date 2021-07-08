# generated from catkin/cmake/template/pkg.context.pc.in
CATKIN_PACKAGE_PREFIX = ""
PROJECT_PKG_CONFIG_INCLUDE_DIRS = "${prefix}/include".split(';') if "${prefix}/include" != "" else []
PROJECT_CATKIN_DEPENDS = "nodelet;roscpp;sensor_msgs;hexapod_msgs".replace(';', ' ')
PKG_CONFIG_LIBRARIES_WITH_PREFIX = "-lhexapod_teleop_joystick".split(';') if "-lhexapod_teleop_joystick" != "" else []
PROJECT_NAME = "hexapod_teleop_joystick"
PROJECT_SPACE_DIR = "/usr/local"
PROJECT_VERSION = "0.2.0"
