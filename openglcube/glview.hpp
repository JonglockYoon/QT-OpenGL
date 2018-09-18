#pragma once

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

#include <assimp/scene.h>
#include <QOpenGLBuffer>

QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

class CGLView : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

private:

    /// \struct SBBox
    /// Bounding box for object.
    struct SBBox
    {
        aiVector3D Minimum;///< Minimum values of coordinates.
        aiVector3D Maximum;///< Maximum values of coordinates.
    };

    /// \struct SHelper_Mesh
    /// Helper object for fast rendering of mesh (\ref aiMesh).
    struct SHelper_Mesh
    {
        const size_t Quantity_Point;///< Quantity of points.
        const size_t Quantity_Line;///< Quantity of lines.
        const size_t Quantity_Triangle;///< Quantity of triangles.
        GLuint* Index_Point;///< Array of indices for drawing points.
        GLuint* Index_Line;///< Array of indices for drawing lines.
        GLuint* Index_Triangle;///< Array of indices for drawing triangles.

        const SBBox BBox;///< BBox of mesh.

        /// \fn explicit SHelper_Mesh(const size_t pQuantity_Point, const size_t pQuantity_Line, const size_t pQuantity_Triangle, const SBBox& pBBox = {{0, 0, 0}, {0, 0, 0}})
        /// Constructor.
        /// \param [in] pQuantity_Point - quantity of points.
        /// \param [in] pQuantity_Line - quantity of lines.
        /// \param [in] pQuantity_Triangle - quantity of triangles.
        /// \param [in] pBBox - BBox of mesh.
        explicit SHelper_Mesh(const size_t pQuantity_Point, const size_t pQuantity_Line, const size_t pQuantity_Triangle, const SBBox& pBBox = {{0, 0, 0}, {0, 0, 0}});

        /// \fn ~SHelper_Mesh()
        /// Destructor.
        ~SHelper_Mesh();
    };


    /// \struct SHelper_Camera
    /// Information about position of the camera in space.
    struct SHelper_Camera
    {
        aiVector3D Position;///< Coordinates of the camera.
        aiVector3D Target;///< Target point of the camera.
        // Transformation path:
        // set Camera -> Rotation_AroundCamera -> Translation_ToScene -> Rotation_Scene -> draw Scene
        aiMatrix4x4 Rotation_AroundCamera;///< Rotation matrix which set rotation angles of the scene around camera.
        aiMatrix4x4 Rotation_Scene;///< Rotation matrix which set rotation angles of the scene around own center.
        aiVector3D Translation_ToScene;///< Translation vector from camera to the scene.

        /// \fn void SetDefault()
        /// Set default parameters of camera.
        void SetDefault();
    };


private:

    // Cameras
    SHelper_Camera mHelper_Camera;///< Information about current camera placing in space.
    SHelper_Camera mHelper_CameraDefault;///< Information about default camera initial placing in space.
    bool mCamera_DefaultAdded = true;///< If true then scene has no defined cameras and default was added, if false - scene has defined cameras.
    GLdouble mCamera_FOVY = 45.0;///< Specifies the field of view angle, in degrees, in the y direction.
    GLdouble mCamera_Viewport_AspectRatio;///< Specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height).


	void ImportTextures(const QString& pScenePath);

    aiCamera camera_cur;

    QOpenGLTexture *textures[6];

	/********************************************************************/
	/*********************** Override functions ************************/
	/********************************************************************/

protected:
    void drawCoordSystem();
	/// \fn void initializeGL() override
	/// Override function to initialise OpenGL.
	void initializeGL() override;

	/// \fn void resizeGL(int pWidth, int pHeight) override
	/// \param [in] pWidth - new width of viewport.
	/// \param [in] pHeight - new height of viewport.
	void resizeGL(int pWidth, int pHeight) override;

	/// \fn void paintGL() override
	/// Override function for rendering.
	void paintGL() override;

public:

	/********************************************************************/
	/********************** Constructor/Destructor **********************/
	/********************************************************************/

	/// \fn explicit CGLView(QWidget* pParent)
	/// Constructor.
	/// \param [in] pParent - parent widget.
	explicit CGLView(QWidget* pParent);

	/// \fn virtual ~CGLView()
	/// Destructor.
	virtual ~CGLView();

	/********************************************************************/
	/******************** Cameras control functions *********************/
	/********************************************************************/

	/// \fn void Camera_Set(const size_t pCameraNumber)
	/// Set view from pointed camera.
	/// \param [in] pCamera_Index - index of the camera (\ref aiScene::mCameras).
	void Camera_Set(const size_t pCameraNumber);

	/// \fn void Camera_RotateScene(const GLfloat pAngle_X, const GLfloat pAngle_Y, const GLfloat pAngle_Z)
	/// Rotate scene around axisees.
	/// \param [in] pAngle_X - specifies the angle of rotation around axis oX, in degrees.
	/// \param [in] pAngle_Y - specifies the angle of rotation around axis oY, in degrees.
	/// \param [in] pAngle_Z - specifies the angle of rotation around axis oZ, in degrees.
	void Camera_RotateScene(const GLfloat pAngle_X, const GLfloat pAngle_Y, const GLfloat pAngle_Z);

	/// \fn void Camera_Rotate(const GLfloat pAngle_X, const GLfloat pAngle_Y, const GLfloat pAngle_Z)
	/// Rotate camera around axisees.
	/// \param [in] pAngle_X - specifies the angle of rotation around axis oX, in degrees.
	/// \param [in] pAngle_Y - specifies the angle of rotation around axis oY, in degrees.
	/// \param [in] pAngle_Z - specifies the angle of rotation around axis oZ, in degrees.
	void Camera_Rotate(const GLfloat pAngle_X, const GLfloat pAngle_Y, const GLfloat pAngle_Z);

	/// \fn void Camera_Translate(const size_t pTranslate_X, const size_t pTranslate_Y, const size_t pTranslate_Z)
	/// Translate camera along axises. In local coordinates.
	/// \param [in] pTranslate_X - specifies the X coordinate of translation vector.
	/// \param [in] pTranslate_Y - specifies the Y coordinate of translation vector.
	/// \param [in] pTranslate_Z - specifies the Z coordinate of translation vector.
	void Camera_Translate(const GLfloat pTranslate_X, const GLfloat pTranslate_Y, const GLfloat pTranslate_Z);

public:

    void qglColor(QColor color);
    void qglClearColor(QColor clearColor);

public:
    GLfloat scale;

    void DoCubeDisplay();

    GLuint id_ogl_texture[6];// OpenGL texture ID.

};// class CGLView
