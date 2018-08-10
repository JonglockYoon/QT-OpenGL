/*

 OpenGL VBO,VAO Sample program
 Copyright 2018, Created by Yoon Jong-lock <jerry1455@gmail.com>

*/

#include "glwidget4.h"
#include <QMouseEvent>
#include <QCoreApplication>
#include <math.h>

GLWidget4::GLWidget4(QWidget *parent) :
    QOpenGLWidget(parent)
{
    setFixedSize(640, 480);
}

GLWidget4::~GLWidget4()
{
}


void GLWidget4::initializeGL()
{
    GLenum GlewInitResult = glewInit();

    static const GLfloat g_vertex_buffer_data[] = {
        0.0f, 0.0f, 0.0f,  // Vertex 0
        -0.5f, 0.5f, 0.0f,// Vertex 1
        0.5f, 0.5f, 0.0f, // Vertex 2
        1.0f, 0.0f, 0.0f, // Vertex 3
        0.5f, -0.5f, 0.0f,// Vertex 4
        -0.5f, -0.5f, 0.0f,// Vertex 5
        -1.0f, 0.0f, 0.0f,// Vertex 6
    };

    // 초기화
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    glEnableClientState(GL_VERTEX_ARRAY);
}

void GLWidget4::paintGL()
{
    static const GLubyte indices[] = {
        0,2,1, // 삼각형 1
        0,3,2,// 삼각형 2
        0,4,3,// 삼각형 3
        0,5,4,// 삼각형 4
        0,6,5,// 삼각형 5
        0,1,6,// 삼각형 6
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 렌더링
    // 버퍼의 첫번째 속성값(attribute) : 버텍스들
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0,  3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // 삼각형 그리기!
    glDrawElements(GL_TRIANGLE_STRIP, sizeof(indices)/sizeof(GLubyte), GL_UNSIGNED_BYTE, indices);
    glDisableVertexAttribArray(0);

}

void GLWidget4::resizeGL(int w, int h)
{
}

void GLWidget4::mousePressEvent(QMouseEvent *e)
{
}

void GLWidget4::mouseReleaseEvent(QMouseEvent *e)
{
}
