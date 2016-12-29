project(GameEngineBretsko)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lboost_graph -pthread")
#-lentityx -lassimp -lboost -lglut -lglfw3 -lGL -lGLU -lGLEW

find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)
set(GL_LIBRARY GL GLU GLEW glut glfw3 X11)

#-------------------HEADERS-----------------

include_directories(${PROJECT_SOURCE_DIR})

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(SUBDIRS ${PROJECT_SOURCE_DIR})

SET(HEADERS "")
FOREACH(subdir ${SUBDIRS})
    #LIST(APPEND FOLDERS ${subdir})
    include_directories(${subdir})
ENDFOREACH()


#-------------------SOURCES-----------------

# include_directories(".")

aux_source_directory(. SRC_ROOT)

file(GLOB_RECURSE CPP_SUBDIRS ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE C_SUBDIRS ${PROJECT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE CC_SUBDIRS ${PROJECT_SOURCE_DIR}/*.cc)

FILE(GLOB_RECURSE hppFiles "*.hpp")
FILE(GLOB_RECURSE hhFiles "*.hh")
FILE(GLOB_RECURSE hFiles "*.h")
add_custom_target(headers SOURCES ${hppFiles} ${hFiles} ${hhFiles})

set(SRC_LIST ${CPP_SUBDIRS} ${C_SUBDIRS} ${CC_SUBDIRS} ${SRC_ROOT})

add_executable(${PROJECT_NAME} ${SRC_LIST})

#------------------- BULLET(STATIC)-----------------


target_link_libraries(${PROJECT_NAME}
    ${GL_LIBRARY}
   # boost
    entityx
    assimp
    Xxf86vm dl X11 Xi Xrandr Xxf86vm Xinerama Xcursor
    /usr/local/lib/libBulletDynamics.a
    /usr/local/lib/libBulletCollision.a
    /usr/local/lib/libLinearMath.a
#  /usr/local/lib/libglfw3.a
#/usr/lib/x86_64-linux-gnu/libGL.so
#/usr/lib/x86_64-linux-gnu/libGLEW.so
)
