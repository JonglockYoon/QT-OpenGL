/*

 OpenGL VBO,VAO Sample program
 Copyright 2018, Created by Yoon Jong-lock <jerry1455@gmail.com>

*/

#include "glwidget6.h"
#include <QMouseEvent>
#include <QCoreApplication>
#include <math.h>

GLWidget6::GLWidget6(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFixedSize(640, 480);
}

GLWidget6::~GLWidget6()
{
}

void GLWidget6::initializeGL()
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


    // VAO 초기화
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // VBO 초기화
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    glEnableClientState(GL_VERTEX_ARRAY);

    // 전달Data 명시
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,  3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindVertexArray(0);

}

void GLWidget6::paintGL()
{
    static const GLushort indices[] = {
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
    glBindVertexArray(VertexArrayID);
    glDrawElements(GL_TRIANGLE_STRIP, sizeof(indices)/sizeof(GLushort), GL_UNSIGNED_SHORT, indices);
    glBindVertexArray(0);

    update();

}

void GLWidget6::resizeGL(int w, int h)
{

}

void GLWidget6::mousePressEvent(QMouseEvent *event)
{

}

void GLWidget6::mouseMoveEvent(QMouseEvent *event)
{
}
