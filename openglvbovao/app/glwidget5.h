
#ifndef GLWIDGET5_H
#define GLWIDGET5_H

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

// Define this somewhere in your header file
#define BUFFER_OFFSET(i) ((void*)(i))

struct MyVertex
{
  float x, y, z;        // Vertex
  float nx, ny, nz;     // Normal
  float s0, t0;         // Texcoord0
};


class GLWidget5 : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit GLWidget5(QWidget *parent = 0);
    ~GLWidget5();

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

public:
    MyVertex pvertex[3];
    ushort pindices[3];

    GLuint VertexVBOID;
    GLuint IndexVBOID;

};

#endif // MAINWIDGET_H
