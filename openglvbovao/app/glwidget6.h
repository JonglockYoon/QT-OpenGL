
#ifndef GLWIDGET6_H
#define GLWIDGET6_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GLM_FORCE_RADIANS
#ifdef Q_OS_WIN
#include <qglobal.h>
#endif

/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/freeglut.h>


#include <QOpenGLWidget>
#include <QOpenGLBuffer>

#define BUFFER_OFFSET(i) ((void*)(i))

class GLWidget6 : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget6(QWidget *parent = 0);
    ~GLWidget6();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

public:
    GLuint VertexArrayID;
    GLuint vertexbuffer;

};

#endif
