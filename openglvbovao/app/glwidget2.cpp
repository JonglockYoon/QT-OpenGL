/*

 OpenGL VBO,VAO Sample program
 Copyright 2018, Created by Yoon Jong-lock <jerry1455@gmail.com>

*/

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "glwidget2.h"
#include <QMouseEvent>
#include <QCoreApplication>
#include <math.h>

GLWidget2::GLWidget2(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFixedSize(640, 480);
}

GLWidget2::~GLWidget2()
{
}


void GLWidget2::initializeGL()
{
}

void GLWidget2::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
    };

    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, &g_vertex_buffer_data[0] );
    glDrawArrays(GL_TRIANGLES, 0, 3); // 버텍스 0에서 시작해서; 총 3개의 버텍스로 -> 하나의 삼각형

    update();
}

void GLWidget2::resizeGL(int w, int h)
{
}

void GLWidget2::mousePressEvent(QMouseEvent *event)
{
}

void GLWidget2::mouseMoveEvent(QMouseEvent *event)
{
}

