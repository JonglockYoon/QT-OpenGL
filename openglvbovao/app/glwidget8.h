
#ifndef GLWIDGET8_H
#define GLWIDGET8_H

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

typedef struct {
    GLfloat x, y, z;
} vec3;


typedef struct {
    vec3     xlate;   /* Translation */
    GLfloat  angle;
    vec3     axis;
} XForm;



class GLWidget8 : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget8(QWidget *parent = 0);
    ~GLWidget8();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public:
    enum { Cube, Cone, NumVAOs };
    enum { Vertices, Colors, Elements, NumVBOs };
    GLuint  buffers[NumVAOs][NumVBOs];
    GLuint   VAO[NumVAOs];
    GLenum   PrimType[NumVAOs];
    GLsizei  NumElements[NumVAOs];
    GLfloat  Angle = 0.0;
    XForm    Xform[NumVAOs];
};

#endif
