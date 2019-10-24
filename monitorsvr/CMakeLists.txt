cmake_minimum_required(VERSION 2.8.3)
project(monitorsvr  C CXX)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
    roscpp
    std_msgs
	sensor_msgs
 	message_generation	#add
  	darknet_ros		#add
  	darknet_ros_msgs	#add
#    genmsg
)


## System dependencies are found with CMake's conventions
include( FindPkgConfig REQUIRED)
# pkg_check_modules (GLIB2 REQUIRED glib-2.0)
# if(GLIB2_FOUND)
#     message("Found glib-2.0")
# else(GLIB2_FOUND)
#     message("Not Found glib-2.0")
# endif(GLIB2_FOUND)

########## set application version ##########
SET (monitorsvr_VERSION_MAJOR 1)
SET (monitorsvr_VERSION_MINOR 0)
SET (_OS_LINUX "ON")
SET (DEBUG_BUILD "ON")
SET (BUILD_LOG_DETAIL "ON")
SET (_TEST_DATA_SUPPORT "ON")

#ADD_DEFINITIONS(-D _OS_LINUX)

if(_TEST_DATA_SUPPORT)
ADD_DEFINITIONS(-D_TEST_DATA_SUPPORT)
endif(_TEST_DATA_SUPPORT)

if(BUILD_LOG_DETAIL)
MESSAGE( "cmake & make verbose mode is ON." )
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
endif(BUILD_LOG_DETAIL)

if(_OS_LINUX)
ADD_DEFINITIONS( -D_OS_LINUX )
MESSAGE( ">>>>>>>>>>>>>>>>>>>>>> _OS_LINUX <<<<<<<<<<<<<<<<<<<<<<<<" )
endif(_OS_LINUX)

if(_OS_WINDOWS)
ADD_DEFINITIONS( -D_OS_WINDOWS )
MESSAGE( ">>>>>>>>>>>>>>>>>>>>>> _OS_WINDOWS <<<<<<<<<<<<<<<<<<<<<<<<" )
endif(_OS_WINDOWS)


if(DEBUG_BUILD)
    ADD_DEFINITIONS(-DDEBUG_BUILD_TYPE)
    SET(BUILD_TYPE "debug")
    SET(CMAKE_BUILD_TYPE Debug)
    MESSAGE("BUILD_TYPE - debug was triggered.")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
    ADD_DEFINITIONS(-D_DEBUG)
else(DEBUG_BUILD)
    ADD_DEFINITIONS(-DRELEASE_BUILD_TYPE)
    SET(BUILD_TYPE "release")
    SET(CMAKE_BUILD_TYPE Release)
    MESSAGE("BUILD_TYPE - release was triggered.")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
endif(DEBUG_BUILD)

MESSAGE( "DEBUG_BUILD:${DEBUG_BUILD}" )
MESSAGE( "DEBUG_LOG:${DEBUG_LOG}" )
MESSAGE( "BUILD_LOG_DETAIL:${BUILD_LOG_DETAIL}" )
MESSAGE( "CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}" )
MESSAGE( "CMAKE_INSTALL_PREFIX:${CMAKE_INSTALL_PREFIX}" )

SET (ARCH ${CMAKE_SYSTEM_PROCESSOR})
SET (HOST ${CMAKE_SYSTEM_NAME})
SET (TARGET_NAME ${PROJECT_NAME}_${HOST}_${ARCH}_${BUILD_TYPE})


SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wcast-align -fPIC -Wcast-qual -Wextra -Wno-unused-parameter \
                  -Wshadow -Wno-write-strings -fno-common -fno-omit-frame-pointer \
                  -fno-optimize-sibling-calls -fno-strict-aliasing -fsigned-char -fstrict-overflow \
                  -Wno-ignored-qualifiers -Wno-array-bounds -Wno-empty-body -fpermissive -g ")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wcast-align -fPIC -Wcast-qual -Wextra -Wnon-virtual-dtor \
                    -Wno-unused-parameter -Wshadow -Wno-write-strings -fno-omit-frame-pointer \
                    -fno-optimize-sibling-calls -fno-strict-aliasing -fsigned-char -fstrict-overflow \
                    -Wno-ignored-qualifiers -Wno-c++0x-compat -Wno-array-bounds -Wno-empty-body \
                    -fpermissive -std=c++11 -g ")

# SET(GLIB_COMPONENTS)
#LIST(APPEND GLIB_COMPONENTS glib
#                            gio
#                            gmodule
#                            gobject
#                            gthread)

###############################################################################
# Cconfigure all header files to pass some of the CMake settings
# to the source code
###############################################################################
configure_file (
    "${PROJECT_SOURCE_DIR}/project_config.h.in"
    "${PROJECT_BINARY_DIR}/project_config.h"
    )

include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists_variables.txt)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a exec_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a exec_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)


################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
	
#  INCLUDE_DIRS include
#  LIBRARIES monitorsvr
   CATKIN_DEPENDS roscpp std_msgs darknet_ros_msgs 
#  DEPENDS system_lib Boost

)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
#include_directories(


INCLUDE_DIRECTORIES(${catkin_INCLUDE_DIRS}
					${CMAKE_SOURCE_DIR}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/include/monitorsvr
                    ${CMAKE_CURRENT_SOURCE_DIR}/src
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/signal_handler/inc
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/inc
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/monitor_svr/inc
  					include			
                   )

link_directories(		#add
  /usr/local/cuda/lib64
)

########## Set a source files ##########
FILE( GLOB_RECURSE DOG_TRACKER_SVR_MOD_SRC 
   "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
					)				       
## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/monitorsvr.cpp
# )

MESSAGE( "DOG_TRACKER_SVR_MOD_SRC : ${DOG_TRACKER_SVR_MOD_SRC}" )


#LINK_DIRECTORIES( ${GLIB2_LIBRARY_DIRS}  )
#MESSAGE( "GLIB2_INCLUDE_DIRS : ${GLIB2_INCLUDE_DIRS}" )
#MESSAGE( "GLIB2_LIBRARY_DIRS : ${GLIB2_LIBRARY_DIRS}" )
#MESSAGE( "GLIB2_LIBRARIES : ${GLIB2_LIBRARIES}" )
#MESSAGE( "TARGET_NAME : ${TARGET_NAME}" )

ADD_EXECUTABLE(${TARGET_NAME} ${DOG_TRACKER_SVR_MOD_SRC})
TARGET_LINK_LIBRARIES(  ${TARGET_NAME}  
						${catkin_LIBRARIES} 
						pthread
						stdc++	#add
   						cuda		#add
   						cudart	#add
   						cublas	#add
						
)
TARGET_include_directories( ${TARGET_NAME} PUBLIC  )
MESSAGE( "[${TARGET_NAME}] will be installed to [${CMAKE_INSTALL_PREFIX}]." )


## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
# add_executable(${PROJECT_NAME}_node src/monitorsvr_node.cpp)

## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
# target_link_libraries(${PROJECT_NAME}_node
#   ${catkin_LIBRARIES}
# )

#############
## Install ##
#############
INSTALL( TARGETS ${TARGET_NAME} RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION} )


# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_executables.html
# install(TARGETS ${PROJECT_NAME}_node
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark libraries for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_libraries.html
# install(TARGETS ${PROJECT_NAME}
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_monitorsvr.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
