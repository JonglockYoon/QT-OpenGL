
#ifndef GLWIDGET2_H
#define GLWIDGET2_H

#include <GL/freeglut.h>
#include <QOpenGLWidget>

class GLWidget2 : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget2(QWidget *parent = 0);
    ~GLWidget2();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

};
#endif
