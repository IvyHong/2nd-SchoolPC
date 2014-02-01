#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T11:08:35
#
#-------------------------------------------------

# QT specifies which of the Qt library modules are used by the project
QT       += core

QT       -= gui

#QT     -= gui core

QT       += opengl

TARGET = ANITA

CONFIG   += console
CONFIG   -= app_bundle

# TEMPLATE specifies the qmake template type, controlling the form
# of the generated makefile.

TEMPLATE = app

# SOURCES is used to tell the IDE which source files
# to include in the project. This will show these source
# files in the Project window, and if any of these files
# change, the project will be considered out of date.
# Here you should only list those files that are part of
# the current project.

SOURCES += main.cpp \
    src/detectorconstruction.cpp \
    src/physicslist.cpp \
    src/primarygeneratoraction.cpp \
    src/cellparameterisation.cpp \
    src/calorimeterhit.cpp \
    src/calorimeter.cpp


# HEADERS is used to tell the IDE which header files
# to include in the project. This will show these header
# files in the Project window, and if any of these files
# change, the project will be considered out of date.
# Here you should only list those files that are part of
# the current project.

HEADERS += \
    include/detectorconstruction.h \
    include/physicslist.h \
    include/primarygeneratoraction.h \
    include/cellparameterisation.h \
    include/calorimeterhit.h \
    include/calorimeter.h

# INCLUDEPATH is used to tell the compiler where to look
# (which directories) for header files. These directories
# contain header files that are part of the current project
# and also other header files, for example associated with
# library code, such as Geant4 and CLHEP.

# Geant4 include path
INCLUDEPATH += $$(G4INCLUDE)

# CLHEP include path
INCLUDEPATH += $$(CLHEP_INCLUDE_DIR)

# ROOT include path
INCLUDEPATH += $$(ROOTSYS)/include

# Project include path
INCLUDEPATH += include

# LIBS is used to tell the linker which files to link (-l option)
# and where to look for them (which directories, -L option).

# Geant4 library path and files
LIBS += -L$$(G4LIB) -lG4global -lG4physicslists \
    -lG4graphics_reps -lG4RayTracer -lG4analysis -lG4intercoms -lG4readout \
    -lG4interfaces -lG4run -lG4digits_hits -lG4materials \
    -lG4tracking -lG4error_propagation -lG4modeling -lG4track -lG4event \
    -lG4OpenGL -lG4Tree -lG4FR -lG4parmodels -lG4visHepRep -lG4geometry \
    -lG4particles -lG4vis_management -lG4gl2ps -lG4persistency -lG4visXXX   \
    -lG4VRML -lG4GMocren -lG4processes -lG4zlib

# ROOT library path and files
LIBS += -L$$(ROOTSYS)/lib  -lTree -lHist -lGui -lCore -lCint -lRIO  \
    -lNet -lGraf -lGraf3d -lGpad -lRint -lPostscript    \
    -lMatrix -lPhysics -lMathCore -lThread -lm -rdynamic -pthread


# OTHER_FILES is used to tell the IDE which other files
# to include in the project. This will show these other
# files in the Project window. The project will not be
# considered out of date if these files change.


