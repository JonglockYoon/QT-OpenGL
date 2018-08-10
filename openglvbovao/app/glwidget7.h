
#ifndef GLWIDGET7_H
#define GLWIDGET7_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef Q_OS_WIN
#include <qglobal.h>
#endif

/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/freeglut.h>

#include <QOpenGLWidget>


//#define BUFFER_OFFSET(offset)  (offset)
#define BUFFER_OFFSET(i) ((void*)(i))
#define NumberOf(array)        (sizeof(array)/sizeof(array[0]))

class GLWidget7 : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget7(QWidget *parent = 0);
    ~GLWidget7();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public:
    GLuint VertexArrayID;
    GLuint vertexbuffer;
    GLuint indexbuffer;
    GLsizei  NumElements;
};

#endif
