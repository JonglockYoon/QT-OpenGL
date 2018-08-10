/*

 OpenGL VBO,VAO Sample program
 Copyright 2018, Created by Yoon Jong-lock <jerry1455@gmail.com>

 Ref : https://www.khronos.org/opengl/wiki/VBO_-_just_examples

*/

#include "glwidget5.h"
#include <QMouseEvent>
#include <QCoreApplication>
#include <math.h>

GLWidget5::GLWidget5(QWidget *parent) :
    QOpenGLWidget(parent)
{
    setFixedSize(640, 480);
}

GLWidget5::~GLWidget5()
{
    glDeleteBuffers(1, &VertexVBOID);
    glDeleteBuffers(1, &IndexVBOID);
}


void GLWidget5::initializeGL()
{
    GLenum GlewInitResult = glewInit();

    // VERTEX 0
      pvertex[0].x = 0.0;
      pvertex[0].y = 0.0;
      pvertex[0].z = 0.0;
      pvertex[0].nx = 0.0;
      pvertex[0].ny = 0.0;
      pvertex[0].nz = 1.0;
      pvertex[0].s0 = 0.0;
      pvertex[0].t0 = 0.0;
      // VERTEX 1
      pvertex[1].x = 1.0;
      pvertex[1].y = 0.0;
      pvertex[1].z = 0.0;
      pvertex[1].nx = 0.0;
      pvertex[1].ny = 0.0;
      pvertex[1].nz = 1.0;
      pvertex[1].s0 = 1.0;
      pvertex[1].t0 = 0.0;
      // VERTEX 2
      pvertex[2].x = 0.0;
      pvertex[2].y = 1.0;
      pvertex[2].z = 0.0;
      pvertex[2].nx = 0.0;
      pvertex[2].ny = 0.0;
      pvertex[2].nz = 1.0;
      pvertex[2].s0 = 0.0;
      pvertex[2].t0 = 1.0;

      glGenBuffers(1, &VertexVBOID);
      glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
      glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex)*3, &pvertex[0].x, GL_STATIC_DRAW);

      pindices[0] = 0;
      pindices[1] = 1;
      pindices[2] = 2;

      glGenBuffers(1, &IndexVBOID);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ushort)*3, pindices, GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(0));    // The starting point of the VBO, for the vertices

      glEnableClientState(GL_NORMAL_ARRAY);
      glNormalPointer(GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(12));      // The starting point of normals, 12 bytes away

      glClientActiveTexture(GL_TEXTURE0);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(24)); // The starting point of texcoords, 24 bytes away
}

void GLWidget5::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glBindVertexArray(IndexVBOID);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
    // To render, we can either use glDrawElements or glDrawRangeElements
    // The is the number of indices. 3 indices needed to make a single triangle
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));    // The starting point of the IBO
    // 0 and 3 are the first and last vertices
    // glDrawRangeElements(GL_TRIANGLES, 0, 3, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));    // The starting point of the IBO
    // glDrawRangeElements may or may not give a performance advantage over glDrawElements
}

void GLWidget5::resizeGL(int w, int h)
{
}

void GLWidget5::mousePressEvent(QMouseEvent *e)
{
}

void GLWidget5::mouseReleaseEvent(QMouseEvent *e)
{
}
