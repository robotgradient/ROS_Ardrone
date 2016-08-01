cmake_minimum_required(VERSION 2.8.3)
project(jm_final_project)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  message_generation #PARA METER MENSAJES
  roscpp
  rospy
  std_msgs
  geometry_msgs
  std_srvs
  sensor_msgs
  tf
  dynamic_reconfigure
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


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
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a run_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)
## Generate messages in the 'msg' folder
 add_message_files(
   #FILES
 #giro_toggle.msg #AÑADIR MENSAJE PROPIO
#   Message1.msg
#   Message2.msg
 )

## Generate services in the 'srv' folder
 add_service_files(
  FILES
   yawrange.srv
   watch_marker.srv
   zrange.srv
#   Service2.srv
 )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
 generate_messages(
   DEPENDENCIES
   std_msgs
 )

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a run_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )
generate_dynamic_reconfigure_options(
cfg/take_land.cfg
cfg/start_rqt.cfg
)



###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES jm_final_project
  CATKIN_DEPENDS roscpp rospy std_msgs std_srvs dynamic_reconfigure sensor_msgs message_runtime
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
)

 add_executable(take_off_node src/take_off_node.cpp)
 add_executable(keyboardControl src/keyboardControl.cpp)
 add_executable(arsuscriber_marker1 src/arsuscriber_marker1.cpp)

add_executable(take_land_node src/take_land_node.cpp)

add_executable(Brain src/Brain.cpp)

add_executable(ar_sight src/ar_sight.cpp)

add_executable(spin_controller src/spin_controller.cpp)

add_executable(arsuscriber2 src/arsuscriber2.cpp)


add_executable(messagepub src/messagepub.cpp)

add_executable(arsuscriberforsimulation src/arsuscriberforsimulation.cpp)

add_executable(controller_final src/controller_final.cpp)

add_executable(yaw_server src/yaw_server.cpp)

add_executable(yaw_client src/yaw_client.cpp)

add_executable(watchmarker_server src/watchmarker_server.cpp)


add_executable(z_server src/z_server.cpp)


add_executable(rqt_commander src/rqt_commander.cpp)


add_executable(sonar_control src/sonar_control.cpp)



#add_dependencies(take_land_node ${PROJECT_NAME}_gencfg)

add_dependencies(rqt_commander ${PROJECT_NAME}_gencfg)
## Declare a C++ library
## add_library(jm_final_project
##  src/${PROJECT_NAME}/jm_final_project.cpp
## )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
add_dependencies(take_land_node ${PROJECT_NAME}_gencfg)
add_dependencies(Brain ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
#add_dependencies(z_server ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
#add_dependencies(keyboardControl ${PROJECT_NAME}}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
#add_dependencies(arsuscriber_marker1 ${PROJECT_NAME}}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
#add_dependencies(take_land_node ${PROJECT_NAME}}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})





## Declare a C++ executable
target_link_libraries(take_off_node ${catkin_LIBRARIES})
target_link_libraries(keyboardControl ${catkin_LIBRARIES})
target_link_libraries(Brain ${catkin_LIBRARIES})
target_link_libraries(arsuscriber_marker1 ${catkin_LIBRARIES})
target_link_libraries(take_land_node ${catkin_LIBRARIES})
target_link_libraries(ar_sight ${catkin_LIBRARIES})
target_link_libraries(spin_controller ${catkin_LIBRARIES})
target_link_libraries(messagepub ${catkin_LIBRARIES})
target_link_libraries(arsuscriber2 ${catkin_LIBRARIES})
target_link_libraries(arsuscriberforsimulation ${catkin_LIBRARIES})
target_link_libraries(controller_final ${catkin_LIBRARIES})
target_link_libraries(yaw_server ${catkin_LIBRARIES})
target_link_libraries(yaw_client ${catkin_LIBRARIES})
target_link_libraries(watchmarker_server ${catkin_LIBRARIES})
target_link_libraries(z_server ${catkin_LIBRARIES})
target_link_libraries(rqt_commander ${catkin_LIBRARIES})
target_link_libraries(sonar_control ${catkin_LIBRARIES})



#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS jm_final_project jm_final_project_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
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
# catkin_add_gtest(${PROJECT_NAME}-test test/test_jm_final_project.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
