/*

 OpenGL VBO,VAO Sample program
 Copyright 2018, Created by Yoon Jong-lock <jerry1455@gmail.com>

*/

#include "glwidget3.h"
#include <QMouseEvent>
#include <QCoreApplication>
#include <QOpenGLShaderProgram>
#include <math.h>


#include <iostream>
#include <string>
#include <fstream>
#include <vector>

GLWidget3::GLWidget3(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFixedSize(640, 640);
}

GLWidget3::~GLWidget3()
{
}

void GLWidget3::initializeGL()
{
    GLenum GlewInitResult = glewInit();

    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
    };

    // 초기화
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    glEnableClientState(GL_VERTEX_ARRAY);
}

void GLWidget3::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 렌더링
    // 버퍼의 첫번째 속성값(attribute) : 버텍스들
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
       0,        // 0번째 속성(attribute). 0 이 될 특별한 이유는 없지만, 쉐이더의 레이아웃(layout)와 반드시 맞추어야 합니다.
       3,        // 크기(size)
       GL_FLOAT,       // 타입(type)
       GL_FALSE,        // 정규화(normalized)?
       0,                  // 다음 요소 까지 간격(stride)
       (void*)0          // 배열 버퍼의 오프셋(offset; 옮기는 값)
    );
    // 삼각형 그리기!
    glDrawArrays(GL_TRIANGLES, 0, 3); // 버텍스 0에서 시작해서; 총 3개의 버텍스로 -> 하나의 삼각형
    glDisableVertexAttribArray(0);
}

void GLWidget3::resizeGL(int w, int h)
{
}

void GLWidget3::mousePressEvent(QMouseEvent *event)
{
}

void GLWidget3::mouseMoveEvent(QMouseEvent *event)
{
}

