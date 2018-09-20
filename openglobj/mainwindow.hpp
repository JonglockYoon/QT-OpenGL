#pragma once

#include <QtWidgets>
#include "glview.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:

	Ui::MainWindow *ui;

    const aiScene* mScene;///< Pointer to loaded scene (\ref aiScene).
    Assimp::Importer mImporter;///< Assimp importer.
    CGLView* mGLView;
    QPoint mPosition_Pressed_LMB;
    QPoint mPosition_Pressed_RMB;

protected:

    void wheelEvent( QWheelEvent *event );

	void mousePressEvent(QMouseEvent* pEvent) override;

	void mouseMoveEvent(QMouseEvent* pEvent) override;

	void keyPressEvent(QKeyEvent* pEvent) override;

public:
    QMenu *fileMenu;
    QAction *readFileAct;
    QAction *exitAct;

	explicit MainWindow(QWidget* pParent = 0);

	~MainWindow();

private slots:
    void setReadFile();

};
