//
// https://www.khronos.org/opengl/wiki/VBO_-_just_examples
//

#include "glwidget8.h"
#include <QMouseEvent>
#include <QCoreApplication>
#include <math.h>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#define M_PI 3.14159

GLWidget8::GLWidget8(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFixedSize(640, 480);

    Xform[0] = { { -1.5, 0.0, 0.0 }, -45.0, { 1.0, 1.0, 0.0 } };
    Xform[1] = { {  1.5, 0.0, 1.0 }, -40.0, { 1.0, 0.0, 0.0 } };

}

GLWidget8::~GLWidget8()
{
}

void GLWidget8::initializeGL()
{
    GLenum GlewInitResult = glewInit();

    glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);				// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation
    glEnable(GL_NORMALIZE);

    glGenVertexArrays(NumVAOs, VAO);

    {
        static GLfloat  cubeVerts[][3] = {
            { -1.0, -1.0, -1.0 },
            { -1.0, -1.0,  1.0 },
            { -1.0,  1.0, -1.0 },
            { -1.0,  1.0,  1.0 },
            {  1.0, -1.0, -1.0 },
            {  1.0, -1.0,  1.0 },
            {  1.0,  1.0, -1.0 },
            {  1.0,  1.0,  1.0 },
        };

        static GLfloat  cubeColors[][3] = {
            {  0.0,  0.0,  0.0 },
            {  0.0,  0.0,  1.0 },
            {  0.0,  1.0,  0.0 },
            {  0.0,  1.0,  1.0 },
            {  1.0,  0.0,  0.0 },
            {  1.0,  0.0,  1.0 },
            {  1.0,  1.0,  0.0 },
            {  1.0,  1.0,  1.0 },
        };

        static GLushort  cubeIndices[] = {
            0, 1, 3, 2,
            4, 6, 7, 5,
            2, 3, 7, 6,
            0, 4, 5, 1,
            0, 2, 6, 4,
            1, 5, 7, 3
        };

        glBindVertexArray(VAO[Cube]);
        glGenBuffers(NumVBOs, buffers[Cube]);

        glBindBuffer(GL_ARRAY_BUFFER, buffers[Cube][Vertices]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
        glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
        glEnableClientState(GL_VERTEX_ARRAY);

        glBindBuffer(GL_ARRAY_BUFFER, buffers[Cube][Colors]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColors), cubeColors, GL_STATIC_DRAW);
        glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
        glEnableClientState(GL_COLOR_ARRAY);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Cube][Elements]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

        PrimType[Cube] = GL_QUADS;
        NumElements[Cube] = NumberOf(cubeIndices);

        glBindVertexArray(0);
    }

    {
        int i, idx;
        float  dTheta;
#define NumConePoints  36
        /* We add one more vertex for the cone's apex */
        static GLfloat  coneVerts[NumConePoints+1][3] = {
            {0.0, 0.0, 1.0}
        };
        static GLfloat  coneColors[NumConePoints+1][3] = {
            {1.0, 1.0, 1.0}
        };
        static GLushort  coneIndices[NumConePoints+1];

        dTheta = 2*M_PI / (NumConePoints - 1);
        idx = 1;
        for (i = 0; i < NumConePoints; ++i, ++idx) {
            float theta = i*dTheta;
            coneVerts[idx][0] = cos(theta);
            coneVerts[idx][1] = sin(theta);
            coneVerts[idx][2] = 0.0;

            coneColors[idx][0] = cos(theta);
            coneColors[idx][1] = sin(theta);
            coneColors[idx][2] = 0.0;

            coneIndices[idx] = idx;
        }

        glBindVertexArray(VAO[Cone]);
        glGenBuffers(NumVBOs, buffers[Cone]);
        glBindBuffer(GL_ARRAY_BUFFER, buffers[Cone][Vertices]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(coneVerts), coneVerts, GL_STATIC_DRAW);
        glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
        glEnableClientState(GL_VERTEX_ARRAY);

        glBindBuffer(GL_ARRAY_BUFFER, buffers[Cone][Colors]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(coneColors), coneColors, GL_STATIC_DRAW);
        glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
        glEnableClientState(GL_COLOR_ARRAY);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Cone][Elements]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(coneIndices), coneIndices, GL_STATIC_DRAW);

        PrimType[Cone] = GL_TRIANGLE_FAN;
        NumElements[Cone] = NumberOf(coneIndices);

        glBindVertexArray(0);
    }
}

void GLWidget8::paintGL()
{
    int  i;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glPushMatrix();
    glRotatef(Angle, 0.0, 1.0, 0.0);

    float scale = 0.3;
    glScalef(scale, scale, scale);

    i = 0;
    for (i = 0; i < NumVAOs; ++i)
    {
        glPushMatrix();
        glTranslatef(Xform[i].xlate.x, Xform[i].xlate.y, Xform[i].xlate.z);
        glRotatef(Xform[i].angle, Xform[i].axis.x, Xform[i].axis.y, Xform[i].axis.z);
        glBindVertexArray(VAO[i]);
        glDrawElements(PrimType[i], NumElements[i], GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
        glBindVertexArray(0);
        glPopMatrix();
    }

    glPopMatrix();
    update();
}


void GLWidget8::resizeGL(int w, int h)
{
    GLdouble fov = 0.0;
    GLdouble as = (GLdouble)w / (GLdouble)h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, as, 1.0, 100000.0);
}

void GLWidget8::mousePressEvent(QMouseEvent *event)
{
}

void GLWidget8::mouseMoveEvent(QMouseEvent *event)
{
}


