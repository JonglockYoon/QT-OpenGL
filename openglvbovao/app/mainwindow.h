//
// mainwindow.h
//
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef Q_OS_WIN
#include <qglobal.h>
#endif

#include <GL/glew.h>
#include <GL/freeglut.h>

// Qt
#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QMutex>
#include <QTime>
#include <QDateTime>
#include <QVBoxLayout>

#include "glwidget1.h"
#include "glwidget2.h"
#include "glwidget3.h"
#include "glwidget4.h"
#include "glwidget5.h"
#include "glwidget6.h"
#include "glwidget7.h"
#include "glwidget8.h"
#include "glwidget9.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:
    struct Private;
    MainWindow::Private* const d;

public  Q_SLOTS:
    void setApplication1();
    void setApplication2();
    void setApplication3();
    void setApplication4();
    void setApplication5();
    void setApplication6();
    void setApplication7();
    void setApplication8();
    void setApplication9();

public:
    GLWidget1 *glWidget1;
    GLWidget2 *glWidget2;
    GLWidget3 *glWidget3;
    GLWidget4 *glWidget4;
    GLWidget5 *glWidget5;
    GLWidget6 *glWidget6;
    GLWidget7 *glWidget7;
    GLWidget8 *glWidget8;
    GLWidget9 *glWidget9;

};

//extern MainWindow* theMainWindow;

#endif /* MAINWINDOW_H */
