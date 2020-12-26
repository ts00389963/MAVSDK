# Generated from FlightMonitor.pro.

#####################################################################
## FlightMonitor Binary:
#####################################################################

cmake_minimum_required(VERSION 3.5)

project(FlightMonitor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(FlightMonitor
    GUI
    SOURCES
        connectdialog.cpp connectdialog.h connectdialog.ui
        main.cpp
        mainwindow.cpp mainwindow.h mainwindow.ui
        qcustomplot.cpp qcustomplot.h
    PUBLIC_LIBRARIES
        Qt::Gui
        Qt::PrintSupport
        Qt::SerialPort
        Qt::Widgets
    ENABLE_AUTOGEN_TOOLS
        uic
)

# Resources:
set(Resource_resource_files
    "Resource/Connect.png"
    "Resource/Disconnect.png"
    "Resource/Plane.png"
    "Resource/Quit.png"
)

qt_add_resource(FlightMonitor "Resource"
    PREFIX
        "/"
    FILES
        ${Resource_resource_files}
)


#### Keys ignored in scope 1:.:.:FlightMonitor.pro:<TRUE>:
# DISTFILES = <EMPTY>

## Scopes:
#####################################################################

qt_extend_target(FlightMonitor CONDITION (QT_MAJOR_VERSION GREATER 4)
    PUBLIC_LIBRARIES
        Qt::Widgets
)

#### Keys ignored in scope 3:.:.:FlightMonitor.pro:QNX:
# target.path = "/tmp/$${TARGET}/bin"

#### Keys ignored in scope 5:.:.:FlightMonitor.pro:UNIX AND NOT ANDROID:
# target.path = "/opt/$${TARGET}/bin"

#### Keys ignored in scope 6:.:.:FlightMonitor.pro:NOT target.path_ISEMPTY:
# INSTALLS = "target"
