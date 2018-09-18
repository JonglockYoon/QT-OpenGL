
QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = openglcube
TEMPLATE = app

win32 {
INCLUDEPATH += ".\assimp-4.1.0\include"
INCLUDEPATH += ".\DevIL\include"
LIBS += -L"..\openglcube\assimp-4.1.0\lib\x64\Debug" -lassimp-vc140-mt
LIBS += -L"..\openglcube\DevIL\lib\x64\Debug" -lDevIL -lILU -lILUT
LIBS += -lopengl32
LIBS += -lGLU32
}

linux {
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
