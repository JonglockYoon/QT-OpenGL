/*

 OpenGL VBO,VAO Sample program
 Copyright 2018, Created by Yoon Jong-lock <jerry1455@gmail.com>

*/

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "glwidget1.h"
#include <QGLWidget>
#include <QPainter>


GLWidget1::GLWidget1(QWidget *parent) :
    QOpenGLWidget(parent)
{
    setFixedSize(640, 480);
}

GLWidget1::~GLWidget1() {
}

void GLWidget1::initializeGL() {
}

void GLWidget1::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glBegin(GL_TRIANGLES);
        //glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(0.0f, 1.0f);
    glEnd();
}
