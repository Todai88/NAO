cmake_minimum_required(VERSION 2.8)
project(NaoFP)

find_package(qibuild)

# Set Sources
set (_srcs 
	NaoFP.cpp
	Grabber.cpp
	Grabber.h
	Motionizr.cpp
	Motionizr.h
	xGamepad.cpp
	xGamepad.h
	xGUI.cpp
	xGUI.h
	)

# Create executable
qi_create_bin(NaoFP WIN32 ${_srcs})

# Link libs

qi_use_lib(NaoFP ALCOMMON ALVALUE ALVISION ALAUDIO ALERROR QI ALEXTRACTOR OPENCV2_CORE OPENCV2_HIGHGUI BOOST)

