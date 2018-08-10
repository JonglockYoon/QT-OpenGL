
#ifndef GLWIDGET4_H
#define GLWIDGET4_H

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

class GLWidget4 : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit GLWidget4(QWidget *parent = 0);
    ~GLWidget4();

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

public:
    GLuint vertexbuffer;

};

#endif // MAINWIDGET_H
