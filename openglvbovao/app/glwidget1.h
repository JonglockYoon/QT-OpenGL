#ifndef GLWIDGET1_H
#define GLWIDGET1_H

#include <QOpenGLWidget>
#include <iostream>
using namespace std;

class GLWidget1 : public QOpenGLWidget {
    Q_OBJECT
public:
    GLWidget1(QWidget *parent = 0);
    ~GLWidget1();

protected:
    void initializeGL();
    void paintGL();


};

#endif // GLWIDGET1_H
