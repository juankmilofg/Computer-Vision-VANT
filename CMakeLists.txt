cmake_minimum_required(VERSION 2.8)
project( TestesVant )

FIND_PACKAGE(OpenMP) 
FIND_PACKAGE( OpenCV REQUIRED )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

IF(MSVC)
   ADD_DEFINITIONS(/arch:SSE2)
ENDIF(MSVC)

IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-strict-aliasing")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wno-write-strings")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -ansi")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ftree-vectorize -funroll-loops")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

find_package( OpenCV REQUIRED )
set(TestesVant_srcs
      SIFT_VANT.cpp   ASIFT_VANT.cpp   ASIFT_ANGULOS_VANT.cpp
      SURF_VANT.cpp   ASURF_VANT.cpp   ASURF_ANGULOS_VANT.cpp
      AKAZE_VANT.cpp  AAKAZE_VANT      AAKAZE_ANGULOS_VANT.cpp
      ORB_VANT.cpp  
      acuracia.cpp
      main.cpp ) 
add_executable( TestesVant ${TestesVant_srcs})
target_link_libraries( TestesVant ${OpenCV_LIBS} )
