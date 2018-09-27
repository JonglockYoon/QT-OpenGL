
QT       += core
QT       += gui
QT       += xml
QT       += widgets
QT       += multimedia
QT       += multimediawidgets
QT       += opengl

TEMPLATE = app

win32 {
TARGET = ../../videocubeobj/bin/videocubeobj
INCLUDEPATH += ".\assimp-4.1.0\include"
INCLUDEPATH += ".\DevIL\include"
LIBS += -L"..\videocubeobj\assimp-4.1.0\lib\x64\Debug" -lassimp-vc140-mt
LIBS += -L"..\videocubeobj\DevIL\lib\x64\Debug" -lDevIL -lILU -lILUT
LIBS += -lopengl32
LIBS += -lGLU32

DEFINES += Q_OS_WIN
LIBS += -L"..\videocubeobj\winlib"
LIBS += -lws2_32
INCLUDEPATH += "..\videocubeobj\winlib\include"

CONFIG(debug, debug|release) {
LIBS += "..\videocubeobj\winlib\opencv_world340d.lib"
LIBS += "..\videocubeobj\winlib\Debug\qroilib.lib"
LIBS += "..\videocubeobj\winlib\Debug\jpeg.lib"
LIBS += "..\videocubeobj\winlib\Debug\png.lib"
LIBS += "..\videocubeobj\winlib\Debug\zlib.lib"
LIBS += "..\videocubeobj\winlib\Debug\lcms2.lib"
}
CONFIG(release, debug|release) {
LIBS += "..\videocubeobj\winlib\opencv_world340.lib"
LIBS += "..\videocubeobj\winlib\Release\qroilib.lib"
LIBS += "..\videocubeobj\winlib\Release\jpeg.lib"
LIBS += "..\videocubeobj\winlib\Release\png.lib"
LIBS += "..\videocubeobj\winlib\Release\zlib.lib"
LIBS += "..\videocubeobj\winlib\Release\lcms2.lib"
}

}

linux {
TARGET = ../videocubeobj/bin/videocubeobj
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lassimp
LIBS += -lIL -lILU -lILUT -lGLU

QMAKE_CXXFLAGS += -std=c++11 -pthread
LIBS += -ljpeg -lexiv2 -lpng -lz -llcms2 -lX11
LIBS += -L/usr/local/lib
LIBS += -L/usr/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
LIBS += -lopencv_videoio
LIBS += -lopencv_imgcodecs
LIBS += -lopencv_ml
LIBS += -lopencv_shape
LIBS += -lgstreamer-1.0
LIBS += -lgobject-2.0
LIBS += -lglib-2.0
LIBS += -lv4l2
LIBS += -lqroilib
LIBS += -lglut
LIBS += -lGLEW -lGL -lGLU
LIBS += -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp

}

INCLUDEPATH += .

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += glview.hpp mainwindow.hpp
FORMS += mainwindow.ui
SOURCES += glview.cpp main.cpp mainwindow.cpp

RESOURCES +=
