cmake_minimum_required(VERSION 3.15)

project(CA-TAMP)

if(NOT DEFINED CMAKE_CACHE_MAJOR_VERSION)
  execute_process(COMMAND make -j1 -C ${CMAKE_CURRENT_SOURCE_DIR}/rai unityAll)
endif()

set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ version selection") # Update this line to use C++17
find_package(Python REQUIRED)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 REQUIRED)
set(PYBIND11_PYTHON_VERSION 3.10 CACHE STRING "python version")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/.local" CACHE PATH "..." FORCE)
endif()
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

option(USE_PYBIND "build python bindings" ON)
option(USE_BULLET "link to bullet" OFF)
option(USE_PHYSX "link to physx" ON)
option(USE_OPENCV "link to opencv" OFF)
option(USE_REALSENSE "link to realsense2 driver library" OFF)
option(USE_LIBFRANKA "link to libfranka" OFF)
option(USE_PCL "link to pcl library" OFF)

add_compile_options(
        -O3 -g -Wall -Wpedantic -fPIC
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++17> # Update this line to use C++17
        $<$<COMPILE_LANGUAGE:CXX>:-Wno-terminate>
)

################################################################################

add_definitions(
        -DRAI_ROOT_PATH=\"${CMAKE_SOURCE_DIR}/rai\"
        -DRAI_X11 -DRAI_LAPACK -DRAI_EIGEN -DRAI_PTHREAD
        -DRAI_GL
        -DRAI_GLFW -DRAI_ANN -DRAI_QHULL8 -DRAI_ASSIMP -DNO_CYCLE_DETECTION -DRAI_PNG -DRAI_PLY
        -DRAI_GJK -DRAI_CCD -DRAI_FCL -DRAI_Lewiner
        -BOOST_ERROR_CODE_HEADER_ONLY
)

################################################################################

include_directories(
        rai/src
        /usr/include/eigen3
        $ENV{HOME}/.local/include
)

link_directories(
        $ENV{HOME}/.local/lib
        $ENV{HOME}/.local/lib/PhysX
)

file(GLOB SRC_GeoGJK rai/src/Geo/GJK/*.c)
file(GLOB SRC_GeoPly rai/src/Geo/ply/*.c)
file(GLOB SRC_GeoCcd rai/src/Geo/ccd_rai/*.c)
file(GLOB SRC_GeoLewiner rai/src/Geo/Lewiner/*.cpp)

add_library(rai SHARED
        rai/src/Core/unity.cxx
        rai/src/Optim/unity.cxx
        rai/src/Geo/unity.cxx
        ${SRC_GeoGJK}
        ${SRC_GeoPly}
        ${SRC_GeoCcd}
        ${SRC_GeoLewiner}
        rai/src/Algo/unity.cxx
        rai/src/Search/unity.cxx
        rai/src/PathAlgos/unity.cxx
        rai/src/Gui/unity.cxx
        rai/src/Kin/unity.cxx
        rai/src/KOMO/unity.cxx
        rai/src/Logic/unity.cxx
        rai/src/Control/unity.cxx
        rai/src/LGP/unity.cxx
)

target_link_libraries(rai
        jsoncpp lapack blas pthread rt X11
        assimp ccd fcl qhull ann glfw GLEW glut GLU GL png
        boost_system
)

################################################################################

find_package(PkgConfig REQUIRED)

if(USE_BULLET)
  find_package(Bullet REQUIRED)
  add_definitions(-DRAI_BULLET)
  include_directories(${BULLET_INCLUDE_DIRS})
  target_link_libraries(rai ${BULLET_LIBRARIES})
  message(STATUS "[rai] using bullet libs:" ${BULLET_LIBRARIES})
endif()

if(USE_PHYSX)
  add_definitions(-DRAI_PHYSX -DNDEBUG -DPX_DISABLE_FLUIDS -DCORELIB -DPX32 -DLINUX)
  target_link_libraries(rai PhysXExtensions_static_64 PhysX_static_64 PhysXCooking_static_64 PhysXCommon_static_64 PhysXFoundation_static_64)
  message(STATUS "[rai] using physx libs from ~/.local")
endif()

if(USE_OPENCV)
  find_package(OpenCV REQUIRED)
  add_definitions(-DRAI_OPENCV)
  target_link_libraries(rai ${OpenCV_LIBS})
  message(STATUS "[rai] using OpenCV libs:" ${OpenCV_LIBS})
endif()

if(USE_OPENCV_LOCAL)
  add_definitions(-DRAI_OPENCV)
  include_directories($ENV{HOME}/.local/include/opencv4)
  target_link_libraries(rai opencv_core opencv_highgui opencv_imgproc opencv_videoio)
  message(STATUS "[rai] using OpenCV libs from ~/.local")
endif()

if(USE_REALSENSE)
  add_definitions(-DRAI_REALSENSE)
  target_link_libraries(rai realsense2)
  message(STATUS "[rai] using realsense libs")
endif()

if(USE_LIBFRANKA)
  add_definitions(-DRAI_FRANKA)
  target_link_libraries(rai franka PocoNet PocoFoundation)
  message(STATUS "[rai] using libfranka")
endif()

################################################################################

if(USE_PYBIND)
  set(PYTHON_EXECUTABLE "python${PYBIND11_PYTHON_VERSION}")
  message(STATUS "[rai] using python executable " ${PYTHON_EXECUTABLE} )
  execute_process(
          COMMAND bash "-c" "${PYTHON_EXECUTABLE} -m pybind11 --cmakedir"
          OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE pybind11_DIR
  )
  message(STATUS "[rai] using pybind11_DIR " ${pybind11_DIR} )

  find_package(pybind11)
  add_definitions(-DRAI_PYBIND)
  include_directories(${pybind11_INCLUDE_DIRS})

endif()

################################################################################

get_property(_defs DIRECTORY PROPERTY COMPILE_DEFINITIONS)
message(STATUS "[rai] compiler flags: " "${_defs}")

################################################################################

add_executable(kinEdit rai/bin/src_kinEdit/main.cpp)
target_link_libraries(kinEdit rai)

add_executable(meshTool rai/bin/src_meshTool/main.cpp)
target_link_libraries(meshTool rai)

add_executable(serving test/serving/main.cpp
        src/GenericFolFile.cpp
        src/GenericFolFile.h
        src/GenerateDecisionRule.cpp
        src/GenerateDecisionRule.h
        src/MiniLGP.cpp
        src/MiniLGP.h
        src/CounterfactualLGP.cpp
        src/CounterfactualLGP.h
        src/Utils.cpp
        src/Utils.h
)
target_link_libraries(serving rai)

add_executable(pouring test/pouring/main.cpp
        src/GenericFolFile.cpp
        src/GenericFolFile.h
        src/GenerateDecisionRule.cpp
        src/GenerateDecisionRule.h
        src/MiniLGP.cpp
        src/MiniLGP.h
        src/CounterfactualLGP.cpp
        src/CounterfactualLGP.h
        src/Utils.cpp
        src/Utils.h
)
target_link_libraries(pouring rai)

################################################################################

install(
        TARGETS rai
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        COMPONENT pkgconfig)

install(
        TARGETS kinEdit meshTool serving pouring
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
