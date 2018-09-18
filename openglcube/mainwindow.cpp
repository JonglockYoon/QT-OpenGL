#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <GL/glu.h>

void MainWindow::wheelEvent( QWheelEvent *event )
{
    GLdouble scale = mGLView->scale;

    if ( event->delta() > 0 )
        scale = scale + scale * 0.1f;
    else if ( event->delta() < 0 )
        scale = scale - scale * 0.1f;

    if (scale < 0.0)
        scale = 0.0;
    else if (scale > 30.0)
        scale = 30.0;

    mGLView->scale = scale;

    update();
}

void MainWindow::mousePressEvent(QMouseEvent* pEvent)
{
	if(pEvent->button() & Qt::LeftButton)
		mPosition_Pressed_LMB = pEvent->pos();
	else if(pEvent->button() & Qt::RightButton)
		mPosition_Pressed_RMB = pEvent->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent* pEvent)
{
	if(pEvent->buttons() & Qt::LeftButton)
	{
		GLfloat dx = 180 * GLfloat(pEvent->x() - mPosition_Pressed_LMB.x()) / mGLView->width();
		GLfloat dy = 180 * GLfloat(pEvent->y() - mPosition_Pressed_LMB.y()) / mGLView->height();

		if(pEvent->modifiers() & Qt::ShiftModifier)
			mGLView->Camera_RotateScene(dy, 0, dx);// Rotate around oX and oZ axises.
		else
			mGLView->Camera_RotateScene(dy, dx, 0);// Rotate around oX and oY axises.

		mPosition_Pressed_LMB = pEvent->pos();
	}

	if(pEvent->buttons() & Qt::RightButton)
	{
		GLfloat dx = 180 * GLfloat(pEvent->x() - mPosition_Pressed_RMB.x()) / mGLView->width();
		GLfloat dy = 180 * GLfloat(pEvent->y() - mPosition_Pressed_RMB.y()) / mGLView->height();

		if(pEvent->modifiers() & Qt::ShiftModifier)
			mGLView->Camera_Rotate(dy, 0, dx);// Rotate around oX and oZ axises.
		else
			mGLView->Camera_Rotate(dy, dx, 0);// Rotate around oX and oY axises.

		mPosition_Pressed_RMB = pEvent->pos();
	}
}

void MainWindow::keyPressEvent(QKeyEvent* pEvent)
{
GLfloat step;

	if(pEvent->modifiers() & Qt::ControlModifier)
		step = 10;
	else if(pEvent->modifiers() & Qt::AltModifier)
		step = 100;
	else
		step = 1;

	if(pEvent->key() == Qt::Key_A)
		mGLView->Camera_Translate(-step, 0, 0);
	else if(pEvent->key() == Qt::Key_D)
		mGLView->Camera_Translate(step, 0, 0);
	else if(pEvent->key() == Qt::Key_W)
		mGLView->Camera_Translate(0, step, 0);
	else if(pEvent->key() == Qt::Key_S)
		mGLView->Camera_Translate(0, -step, 0);
	else if(pEvent->key() == Qt::Key_Up)
		mGLView->Camera_Translate(0, 0, -step);
	else if(pEvent->key() == Qt::Key_Down)
		mGLView->Camera_Translate(0, 0, step);

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	// Create OpenGL widget
	mGLView = new CGLView(this);
    //mGLView->setMinimumSize(400, 400);
    mGLView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mGLView->setFocusPolicy(Qt::StrongFocus);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->insertWidget(0, mGLView);
    setCentralWidget(mGLView);

}

MainWindow::~MainWindow()
{
	if(mGLView != nullptr) delete mGLView;
	delete ui;
}
