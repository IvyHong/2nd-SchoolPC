#(1)
project(Pro)


#SET(QT_USE_QTMAIN TRUE)
#if(NOT CMAKE_BUILD_TYPE)
#   set(CMAKE_BUILD_TYPE Release)
#endif()

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)


#IF(NOT CMAKE_BUILD_TYPE)
#  SET(CMAKE_BUILD_TYPE Debug CACHE STRING
#      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
#      FORCE)
#ENDIF(NOT CMAKE_BUILD_TYPE)



#(2)
#----------------------------------------------------------------------------
# Find Geant4 package, no UI and Vis drivers activated
#
option(WITH_GEANT4_UIVIS "BUILD SPALLATION WITH GEANT4 UI AND VIS DRIVERS" ON)
if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)

else()
    find_package(Geant4 REQUIRED)
    find_package(Geant4 QUIET COMPONENTS qt)
endif()

#(3)
#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# A CMake script which can be included to handle certain CMake steps automatically.
# Most useful for very basic applications
include(${Geant4_USE_FILE})                      #Auto configure includes/flags

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT QUIET)
if(NOT ROOT_FOUND)
  message(STATUS "G4 Examples: ROOT package not found. --> Pro project disabled")
  return()
endif()

#(4)
#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/../shared/include
                    ${Geant4_INCLUDE_DIR}
    ${ROOT_INCLUDE_DIR})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc
    ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh
    ${PROJECT_SOURCE_DIR}/include/*.h)

#(5)
#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
#compile application
add_executable(main main.cpp ${sources} ${headers} ${TOOLS_FORTRAN_OBJECTS})
 #link it to Geant4
target_link_libraries(main ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#(6)
set(PRO_SCRIPTS
    vis.mac
    run.mac
  )

foreach(_script ${PRO_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(Pro DEPENDS main)

#(7)
#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS main DESTINATION bin)
