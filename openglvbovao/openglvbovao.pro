
QT       += core
QT       += gui
QT       += xml
QT       += widgets
QT       += multimedia
QT       += multimediawidgets
QT       += opengl

win32 {

INCLUDEPATH += "..\winlib\freeglut\include"
INCLUDEPATH += "..\winlib\glew-2.1.0\include"
INCLUDEPATH += "..\winlib\glfw-3.2.1\include\GLFW"

LIBS += -L"..\winlib\freeglut\lib\x64" -lfreeglut
LIBS += -L"..\winlib\glew-2.1.0\lib\Release\x64" -lglew32
LIBS += -L"..\winlib\glfw-3.2.1\lib\x64" -lglfw3

DEFINES += Q_OS_WIN

CONFIG(debug, debug|release) {
}
CONFIG(release, debug|release) {
}

TARGET = ../../bin/openglvbovao
}

linux {
QMAKE_CXXFLAGS += -std=c++11 -pthread
LIBS += -L/usr/local/lib
LIBS += -L/usr/lib
LIBS += -lglut
LIBS += -lGLEW -lGL -lGLU
LIBS += -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp

INCLUDEPATH += /usr/local/include
TARGET = ../bin/openglvbovao
}

TEMPLATE = app

INCLUDEPATH += . \
   ./app \

# Input

HEADERS +=  \
        app/mainwindow.h \
        app/glwidget1.h \
        app/glwidget2.h \
        app/glwidget3.h \
        app/glwidget4.h \
        app/glwidget5.h \
        app/glwidget6.h \
        app/glwidget7.h \
        app/glwidget8.h \
        app/glwidget9.h \

FORMS += \


SOURCES +=  \
        app/main.cpp \
        app/mainwindow.cpp \
        app/glwidget1.cpp \
        app/glwidget2.cpp \
        app/glwidget3.cpp \
        app/glwidget4.cpp \
        app/glwidget5.cpp \
        app/glwidget6.cpp \
        app/glwidget7.cpp \
        app/glwidget8.cpp \
        app/glwidget9.cpp \

RESOURCES += \
   resources.qrc \

