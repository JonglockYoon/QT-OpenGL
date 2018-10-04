
QT       += core
QT       += gui
QT       += xml
QT       += widgets
QT       += multimedia
QT       += multimediawidgets
QT       += opengl

TEMPLATE = app

win32 {
TARGET = ../../bin/openglobj
INCLUDEPATH += "..\assimp-4.1.0\include"
INCLUDEPATH += "..\DevIL\include"
LIBS += -L"..\assimp-4.1.0\lib\x64\Debug" -lassimp-vc140-mt
LIBS += -L"..\DevIL\lib\x64\Debug" -lDevIL -lILU -lILUT
LIBS += -lopengl32
LIBS += -lGLU32
}

linux {
TARGET = ../bin/openglobj
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lassimp
LIBS += -lIL -lILU -lILUT -lGLU
}

INCLUDEPATH += .

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += glview.hpp mainwindow.hpp
FORMS += mainwindow.ui
SOURCES += glview.cpp main.cpp mainwindow.cpp

RESOURCES +=
