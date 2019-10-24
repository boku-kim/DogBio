cmake_minimum_required(VERSION 2.8.3)
project(ydlidar_ros)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#add_definitions(-std=c++11) # Use C++11

# Define path of darknet folder here. #add
find_path(DARKNET_PATH
  NAMES "README.md"
  HINTS "${CMAKE_CURRENT_SOURCE_DIR}/../darknet/")
message(STATUS "Darknet path dir = ${DARKNET_PATH}")
add_definitions(-DDARKNET_FILE_PATH="${DARKNET_PATH}")

# Find CUDA	#add
find_package(CUDA QUIET)
if (CUDA_FOUND)
  find_package(CUDA REQUIRED)
  message(STATUS "CUDA Version: ${CUDA_VERSION_STRINGS}")
  message(STATUS "CUDA Libararies: ${CUDA_LIBRARIES}")
  set(
    CUDA_NVCC_FLAGS
    ${CUDA_NVCC_FLAGS};
    -O3
    -gencode arch=compute_30,code=sm_30
    -gencode arch=compute_35,code=sm_35
    -gencode arch=compute_50,code=[sm_50,compute_50]
    -gencode arch=compute_53,code=[sm_53,compute_53]
    -gencode arch=compute_61,code=sm_61
    -gencode arch=compute_62,code=sm_62
  )
  add_definitions(-DGPU)
else()
  list(APPEND LIBRARIES "m")
endif()

# Find X11	#add
message ( STATUS "Searching for X11..." )
find_package ( X11 REQUIRED )
if ( X11_FOUND )
  include_directories ( ${X11_INCLUDE_DIR} )
  link_libraries ( ${X11_LIBRARIES} )
  message ( STATUS " X11_INCLUDE_DIR: " ${X11_INCLUDE_DIR} )
  message ( STATUS " X11_LIBRARIES: " ${X11_LIBRARIES} )
endif ( X11_FOUND )


## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages

# Find rquired packeges		#add
find_package(Boost REQUIRED COMPONENTS thread)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(catkin REQUIRED COMPONENTS
  rosconsole
  roscpp
  sensor_msgs
  #cv_bridge		#add
  #actionlib		#add
  #image_transport	#add
  message_generation	#add
  darknet_ros		#add
  darknet_ros_msgs	#add
  
)


# Enable OPENCV in darknet	#add
add_definitions(-DOPENCV)
add_definitions(-O4 -g)


## Generate added messages and services with any dependencies listed here
 generate_messages(
   DEPENDENCIES
   std_msgs  # Or other packages containing msgs
 )


#add_subdirectory(sdk)

set(SDK_PATH "./sdk/")

FILE(GLOB SDK_SRC 
  "${SDK_PATH}/src/impl/unix/*.cpp"
  "${SDK_PATH}/src/*.cpp"
  "${SDK_PATH}/src/*.c"
)

catkin_package(		#add
  INCLUDE_DIRS 
   #include
  LIBRARIES 
   ${PROJECT_NAME}_lib
  CATKIN_DEPENDS #add
   roscpp 
   std_msgs 
   darknet_ros_msgs
   #cv_bridge
   #actionlib
   #image_transport
   #darknet
  DEPENDS
    Boost	
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/sdk/include
  ${PROJECT_SOURCE_DIR}/sdk/src				
  ${DARKNET_PATH}/src		#add
  ${DARKNET_PATH}/include	#add
  include			
  ${Boost_INCLUDE_DIRS}		#add

)


link_directories(		#add
  /usr/local/cuda/lib64
)



add_executable(ydlidar_node src/ydlidar_node.cpp  ${SDK_SRC})
add_executable(subscriber src/subscriber.cpp)

target_link_libraries(ydlidar_node
   ${catkin_LIBRARIES}
   stdc++	#add
   cuda		#add
   cudart	#add
   cublas	#add
   ${Boost_LIBRARIES}	#add
   ${OpenCV_LIBRARIES}	#add
   ${OpenCV_LIBS}	#add
   #${Darknet_LIBS}	#add
   
 )

target_link_libraries(subscriber
   ${catkin_LIBRARIES}
   stdc++	#add
   cuda		#add
   cudart	#add
   cublas	#add
   ${Boost_LIBRARIES}	#add
   ${OpenCV_LIBRARIES}	#add
   ${OpenCV_LIBS}	#add
   #${Darknet_LIBS}	#add
   
 )

install(TARGETS ydlidar_node subscriber
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)


install(DIRECTORY launch startup sdk
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  USE_SOURCE_PERMISSIONS
)


