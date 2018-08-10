
#include "glwidget7.h"
#include <QMouseEvent>
#include <QCoreApplication>
#include <math.h>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>


GLWidget7::GLWidget7(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFixedSize(640, 480);
}

GLWidget7::~GLWidget7()
{
}

void GLWidget7::initializeGL()
{

    GLenum GlewInitResult = glewInit();

    glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);				// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation
    glEnable(GL_NORMALIZE);


    static const GLfloat g_vertex_buffer_data[] = {
        0.0f, 0.0f, 0.0f,  // Vertex 0
        -0.5f, 0.5f, 0.0f,// Vertex 1
        0.5f, 0.5f, 0.0f, // Vertex 2
        1.0f, 0.0f, 0.0f, // Vertex 3
        0.5f, -0.5f, 0.0f,// Vertex 4
        -0.5f, -0.5f, 0.0f,// Vertex 5
        -1.0f, 0.0f, 0.0f,// Vertex 6
    };


    static const GLushort indices[] = {
        0,2,1, // 삼각형 1
        0,3,2,// 삼각형 2
        0,4,3,// 삼각형 3
        0,5,4,// 삼각형 4
        0,6,5,// 삼각형 5
        0,1,6,// 삼각형 6
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

    glGenBuffers(1, &indexbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    NumElements = NumberOf(indices);

    glBindVertexArray(0);
}

void GLWidget7::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float scale = 0.7;
    glScalef(scale, scale, scale);
    //glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
    glDisable(GL_TEXTURE_2D);

    // 렌더링
    glBindVertexArray(VertexArrayID);
    glDrawElements(GL_TRIANGLES, NumElements, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
    glBindVertexArray(0);

    update();
}


void GLWidget7::resizeGL(int w, int h)
{
    GLdouble fov = 45.0;
    GLdouble as = (GLdouble)w / (GLdouble)h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, as, 1.0, 100000.0);
}

void GLWidget7::mousePressEvent(QMouseEvent *event)
{
}

void GLWidget7::mouseMoveEvent(QMouseEvent *event)
{
}

