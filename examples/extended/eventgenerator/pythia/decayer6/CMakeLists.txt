#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(decayer6)

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

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find Pythia6 (required package)
#
find_package(Pythia6 QUIET)
if(NOT PYTHIA6_FOUND)
  message(STATUS "G4 Examples: Pythia6 package not found. --> decayer6 example disabled")  
  return()
endif()

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${PROJECT_SOURCE_DIR}/../../../common/detectorConstruction/include
                    ${PROJECT_SOURCE_DIR}/../../../common/userActions/include
                    ${PROJECT_SOURCE_DIR}/../../../common/primaryGenerator/include
                    ${Geant4_INCLUDE_DIR}
                    ${PYTHIA6_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Get examples sources from common and shared
#
list(APPEND sources 
  ${PROJECT_SOURCE_DIR}/../../../common/detectorConstruction/src/ExG4DetectorConstruction01.cc
  ${PROJECT_SOURCE_DIR}/../../../common/detectorConstruction/src/ExG4DetectorConstruction01Messenger.cc
  ${PROJECT_SOURCE_DIR}/../../../common/userActions/src/ExG4EventAction01.cc
  ${PROJECT_SOURCE_DIR}/../../../common/userActions/src/ExG4EventAction01Messenger.cc
  ${PROJECT_SOURCE_DIR}/../../../common/userActions/src/ExG4RunAction01.cc
  ${PROJECT_SOURCE_DIR}/../../../common/userActions/src/ExG4RunAction01Messenger.cc
  ${PROJECT_SOURCE_DIR}/../../../common/primaryGenerator/src/ExG4PrimaryGeneratorAction01.cc)
  
#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(pythia6_decayer pythia6_decayer.cc ${sources} ${headers})
target_link_libraries(pythia6_decayer ${Geant4_LIBRARIES} ${PYTHIA6_LIBRARIES} )

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build decayer6. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(decayer6_SCRIPTS
    init.mac init_vis.mac pythia6_decayer.in pythia6_decayer.out vis.mac
  )

foreach(_script ${decayer6_SCRIPTS})
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
add_custom_target(decayer6 DEPENDS pythia6_decayer)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS pythia6_decayer DESTINATION bin)

