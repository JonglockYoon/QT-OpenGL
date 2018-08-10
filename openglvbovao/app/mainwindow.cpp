/*

Copyright 2018, Created by Yoon Jong-lock <jerry1455@gmail.com>
OpenGL test1 program

*/

#include <QApplication>
#include <QPushButton>
#include <QStackedWidget>
#include <QTimer>
#include <QMenuBar>
#include <QUrl>
#include <QFileDialog>
#include <QMessageBox>

// Local
#include "mainwindow.h"

struct MainWindow::Private
{
    MainWindow* q;
    QWidget* mContentWidget;
    QStackedWidget* mViewStackedWidget;
    QActionGroup* mViewModeActionGroup;

    QMenu *fileMenu;
    QMenu *applicationMenu;

    QAction *exitAct;

    QAction *app1;
    QAction *app2;
    QAction *app3;
    QAction *app4;
    QAction *app5;
    QAction *app6;
    QAction *app7;
    QAction *app8;
    QAction *app9;

    void setupWidgets()
    {
        mContentWidget = new QWidget(q);
        q->setCentralWidget(mContentWidget);

    }

    void setupActions()
    {
        exitAct = new QAction(QIcon(), tr("E&xit"), q);
        exitAct->setShortcuts(QKeySequence::Quit);
        connect(exitAct, SIGNAL(triggered()), q, SLOT(close()));

        app1 = new QAction(QIcon(), tr("&App1 glBegin"), q);
        connect(app1, SIGNAL(triggered(bool)), q, SLOT(setApplication1()));
        app2 = new QAction(QIcon(), tr("&App2 glDrawArrays"), q);
        connect(app2, SIGNAL(triggered(bool)), q, SLOT(setApplication2()));
        app3 = new QAction(QIcon(), tr("&App3 VBO glDrawArrays"), q);
        connect(app3, SIGNAL(triggered(bool)), q, SLOT(setApplication3()));
        app4 = new QAction(QIcon(), tr("&App4 VBO glDrawElements"), q);
        connect(app4, SIGNAL(triggered(bool)), q, SLOT(setApplication4()));
        app5 = new QAction(QIcon(), tr("&App5 VBO#2"), q);
        connect(app5, SIGNAL(triggered(bool)), q, SLOT(setApplication5()));
        app6 = new QAction(QIcon(), tr("&App6 VAO"), q);
        connect(app6, SIGNAL(triggered(bool)), q, SLOT(setApplication6()));
        app7 = new QAction(QIcon(), tr("&App7 VAO#2"), q);
        connect(app7, SIGNAL(triggered(bool)), q, SLOT(setApplication7()));
        app8 = new QAction(QIcon(), tr("&App8 VAO#3"), q);
        connect(app8, SIGNAL(triggered(bool)), q, SLOT(setApplication8()));
        app9 = new QAction(QIcon(), tr("&App9 reserved"), q);
        connect(app9, SIGNAL(triggered(bool)), q, SLOT(setApplication9()));

        fileMenu = new QMenu(tr("&File"), q);
        fileMenu->addAction(exitAct);

        applicationMenu = new QMenu(tr("&Application"), q);
        applicationMenu->addAction(app1);
        applicationMenu->addAction(app2);
        applicationMenu->addAction(app3);
        applicationMenu->addAction(app4);
        applicationMenu->addAction(app5);
        applicationMenu->addAction(app6);
        applicationMenu->addAction(app7);
        applicationMenu->addAction(app8);
        applicationMenu->addAction(app9);

        q->menuBar()->addMenu(fileMenu);
        q->menuBar()->addMenu(applicationMenu);

    }

};

MainWindow* theMainWindow = 0;

MainWindow::MainWindow()
: d(new MainWindow::Private)
{
    theMainWindow = this;

    setMinimumSize(640, 480);

    d->q = this;
    d->setupWidgets();
    d->setupActions();

    glWidget1 = nullptr;
    glWidget2 = nullptr;
    glWidget3 = nullptr;
    glWidget4 = nullptr;
    glWidget5 = nullptr;
    glWidget6 = nullptr;
    glWidget7 = nullptr;
    glWidget8 = nullptr;
    glWidget9 = nullptr;

}


MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (glWidget1) delete glWidget1;
    if (glWidget2) delete glWidget2;
    if (glWidget3) delete glWidget3;
    if (glWidget4) delete glWidget4;
    if (glWidget5) delete glWidget5;
    if (glWidget6) delete glWidget6;
    if (glWidget7) delete glWidget7;
    if (glWidget8) delete glWidget8;
    if (glWidget9) delete glWidget9;
}

void MainWindow::setApplication1()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    glWidget1 = new GLWidget1(this);
    mainLayout->addWidget(glWidget1);
    glWidget1->show();
}


void MainWindow::setApplication2()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    glWidget2 = new GLWidget2(this);
    mainLayout->addWidget(glWidget2);
    glWidget2->show();
}

void MainWindow::setApplication3()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    glWidget3 = new GLWidget3(this);
    mainLayout->addWidget(glWidget3);
    glWidget3->show();
}

void MainWindow::setApplication4()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    glWidget4 = new GLWidget4(this);
    mainLayout->addWidget(glWidget4);
    glWidget4->show();
}

void MainWindow::setApplication5()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    glWidget5 = new GLWidget5(this);
    mainLayout->addWidget(glWidget5);
    glWidget5->show();
}

void MainWindow::setApplication6()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    glWidget6 = new GLWidget6(this);
    mainLayout->addWidget(glWidget6);
    glWidget6->show();
}

void MainWindow::setApplication7()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    glWidget7 = new GLWidget7(this);
    mainLayout->addWidget(glWidget7);
    glWidget7->show();
}

void MainWindow::setApplication8()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    glWidget8 = new GLWidget8(this);
    mainLayout->addWidget(glWidget8);
    glWidget8->show();
}

void MainWindow::setApplication9()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    glWidget9 = new GLWidget9(this);
    mainLayout->addWidget(glWidget9);
    glWidget9->show();
}
