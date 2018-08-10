
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "glwidget9.h"
#include <QMouseEvent>
#include <QCoreApplication>
#include <math.h>

GLWidget9::GLWidget9(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFixedSize(640, 480);
}

GLWidget9::~GLWidget9()
{
}

void GLWidget9::initializeGL()
{

}

void GLWidget9::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);

    update();
}

void GLWidget9::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget9::mousePressEvent(QMouseEvent *event)
{
}

void GLWidget9::mouseMoveEvent(QMouseEvent *event)
{
}

