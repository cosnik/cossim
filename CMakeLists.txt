# $Id: CMakeLists.txt 68058 2013-03-13 14:47:43Z gcosmo $
# xx

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(G4simu)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

message(STATUS " GEANT4 system dir " ${Geant4_DIR}/Modules)
message(STATUS " GEANT4 include dirs " ${Geant4_INCLUDE_DIRS})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${Geant4_DIR}/Modules)
message(STATUS " module path " ${CMAKE_MODULE_PATH})

#----------------------------------------------------------------------------
# find the ROOT package
find_package(ROOT REQUIRED)
message(STATUS " ROOT include dirs " ${ROOT_INCLUDE_DIR})
message(STATUS " ROOT libraries " ${ROOT_LIBRARIES})

# Johns addition to make it work for ROOT6
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include ${Geant4_INCLUDE_DIRS} ${ROOT_INCLUDE_DIR}) 


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
message(STATUS "source files " ${PROJECT_SOURCE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
#set(CMAKE_CXX_FLAGS "-Wno-shadow -std=gnu++0x -std=c++0x" )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-shadow " )
#set(CMAKE_CXX_FLAGS " -Wno-shadow " )
add_executable(G4simu G4simu.cc ${sources} ${headers})
target_link_libraries(G4simu ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} -std=c++0x)
# target_link_libraries(G4simu ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#

foreach(_script ${EXAMPLEB1_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
#add_custom_target(G4simu DEPENDS G4simu)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS G4simu DESTINATION bin)


