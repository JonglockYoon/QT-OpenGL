#pragma once

#include <QtWidgets>
#include "glview.hpp"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:

	Ui::MainWindow *ui;

    CGLView* mGLView;
    QPoint mPosition_Pressed_LMB;
    QPoint mPosition_Pressed_RMB;

protected:

    void wheelEvent( QWheelEvent *event );

	void mousePressEvent(QMouseEvent* pEvent) override;

	void mouseMoveEvent(QMouseEvent* pEvent) override;

	void keyPressEvent(QKeyEvent* pEvent) override;

public:
	explicit MainWindow(QWidget* pParent = 0);

	~MainWindow();

private slots:

};
