#include "glview.hpp"
#include <QTime>
#include <QDebug>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

#define M_PI 3.14159

#include <GL/glu.h>
#include <IL/il.h>
#include <QOpenGLTexture>

GLfloat		xrot;
GLfloat		yrot;
GLfloat		zrot;

/**********************************/
/********** SHelper_Mesh **********/
/**********************************/

CGLView::SHelper_Mesh::SHelper_Mesh(const size_t pQuantity_Point, const size_t pQuantity_Line, const size_t pQuantity_Triangle, const SBBox& pBBox)
    : Quantity_Point(pQuantity_Point), Quantity_Line(pQuantity_Line), Quantity_Triangle(pQuantity_Triangle), BBox(pBBox)
{
    Index_Point = pQuantity_Point ? new GLuint[pQuantity_Point * 1] : nullptr;
    Index_Line = pQuantity_Line ? new GLuint[pQuantity_Line * 2] : nullptr;
    Index_Triangle = pQuantity_Triangle ? new GLuint[pQuantity_Triangle * 3] : nullptr;
}

CGLView::SHelper_Mesh::~SHelper_Mesh()
{
    if(Index_Point != nullptr) delete [] Index_Point;
    if(Index_Line != nullptr) delete [] Index_Line;
    if(Index_Triangle != nullptr) delete [] Index_Triangle;
}

/**********************************/
/********** SHelper_Mesh **********/
/**********************************/

void CGLView::SHelper_Camera::SetDefault()
{
    Position.Set(0, 0, 0);
    Target.Set(0, 0, -1);
    Rotation_AroundCamera = aiMatrix4x4();
    Rotation_Scene = aiMatrix4x4();
    Translation_ToScene.Set(0, 0, 2);

}


void CGLView::ImportTextures(const QString& pScenePath)
{
    auto LoadTexture = [&](int n, const QString& pFileName) -> bool
    {
        ILboolean success;

        ILuint id_image;// DevIL image ID.
        QString basepath = pScenePath.left(pScenePath.lastIndexOf('/') + 1);// path with '/' at the end.
        QString fileloc = (basepath + pFileName);

        fileloc.replace('\\', "/");
        ilGenImages(1, &id_image);// Generate DevIL image ID.
        ilBindImage(id_image);
        success = ilLoadImage(fileloc.toLocal8Bit());
        if(!success)
        {
            return false;
        }

        // Convert every colour component into unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA.
        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        if(!success)
        {
            return false;
        }

        glBindTexture(GL_TEXTURE_2D, id_ogl_texture[n]);// Binding of texture ID.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// We will use linear interpolation for magnification filter.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// We will use linear interpolation for minifying filter.
        glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0,
                        ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());// Texture specification.

        //Cleanup
        ilDeleteImages(1, &id_image);// Because we have already copied image data into texture data we can release memory used by image.

	    return true;
    };


	// Before calling ilInit() version should be checked.
	if(ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	{
		return;
	}

	ilInit();// Initialization of DevIL.

    //
	// Load textures.
	//
	// Get textures file names and number of textures.
    glGenTextures(6, id_ogl_texture);// Texture ID generation.

    for(size_t idx_material = 0; idx_material < 6; idx_material++)
	{
        QString str;
        str = QString("side%1.png").arg(idx_material+1);
        LoadTexture(idx_material, str);
    }

}

void CGLView::initializeGL()
{
    initializeOpenGLFunctions();

    const GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat LightPosition[]= { 0.0f, 0.0f, 0.0f, 1.0f };

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClearDepth(1.0f);				// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);    // Uses default lighting parameters
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_NORMALIZE);

    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
    glEnable(GL_LIGHT1);

    ImportTextures("./images/TexturedCube.obj");
}

void CGLView::resizeGL(int pWidth, int pHeight)
{
	mCamera_Viewport_AspectRatio = (GLdouble)pWidth / pHeight;
	glViewport(0, 0, pWidth, pHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(mCamera_FOVY, mCamera_Viewport_AspectRatio, 1.0, 100000.0);///TODO: znear/zfar depend on scene size.
}

void CGLView::DoCubeDisplay()
{
    // cube ///////////////////////////////////////////////////////////////////////
    //    v6----- v5
    //   /|      /|
    //  v1------v0|
    //  | |     | |
    //  | |v7---|-|v4
    //  |/      |/
    //  v2------v3
    // vertex array for glDrawElements() and glDrawRangeElement() =================
    GLfloat vertices[] = { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,   1,-1, 1,   // v0,v1,v2,v3 (front)
                           1, 1, 1,   1,-1, 1,   1,-1,-1,   1, 1,-1,   // v0,v3,v4,v5 (right)
                           1, 1, 1,   1, 1,-1,  -1, 1,-1,  -1, 1, 1,   // v0,v5,v6,v1 (top)
                          -1, 1, 1,  -1, 1,-1,  -1,-1,-1,  -1,-1, 1,   // v1,v6,v7,v2 (left)
                          -1,-1,-1,   1,-1,-1,   1,-1, 1,  -1,-1, 1,   // v7,v4,v3,v2 (bottom)
                           1,-1,-1,  -1,-1,-1,  -1, 1,-1,   1, 1,-1 }; // v4,v7,v6,v5 (back)
    // normal array
    GLfloat normals[]  = { 0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,   // v0,v1,v2,v3 (front)
                           1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,   // v0,v3,v4,v5 (right)
                           0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,   // v0,v5,v6,v1 (top)
                          -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,   // v1,v6,v7,v2 (left)
                           0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,   // v7,v4,v3,v2 (bottom)
                           0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1 }; // v4,v7,v6,v5 (back)

    #define B 0.9
    // color array
    GLfloat colors[]   = { 1, 1, 1,B,   1, 1, 0,B,   1, 0, 0,B,   1, 0, 1,B,   // v0,v1,v2,v3 (front)
                           1, 1, 1,B,   1, 0, 1,B,   0, 0, 1,B,   0, 1, 1,B,   // v0,v3,v4,v5 (right)
                           1, 1, 1,B,   0, 1, 1,B,   0, 1, 0,B,   1, 1, 0,B,   // v0,v5,v6,v1 (top)
                           1, 1, 0,B,   0, 1, 0,B,   0, 0, 0,B,   1, 0, 0,B,   // v1,v6,v7,v2 (left)
                           0, 0, 0,B,   0, 0, 1,B,   1, 0, 1,B,   1, 0, 0,B,   // v7,v4,v3,v2 (bottom)
                           0, 0, 1,B,   0, 0, 0,B,   0, 1, 0,B,   0, 1, 1,B }; // v4,v7,v6,v5 (back)

    // texCoord array
    //
    //          v6------v5
    //          |       |
    //          |       |                  v6----- v5
    //          |       |			      /|      /|
    //  v6------v1------v0-------v5      v1------v0|
    //  |       |       |        |	     | |     | |
    //  |       |       |        |	     | |v7---|-|v4
    //  |       |       |        |	     |/      |/
    //  v7------v2------v3-------v4	     v2------v3
    //          |       |
    //          |       |
    //          |       |
    //          v7------v4
    //          |       |
    //          |       |
    //          |       |
    //          v6------v5
    GLfloat texCoords[]  = { 1, 0,   0, 0,   0, 1,   1, 1,             // v0,v1,v2,v3 (front)
                             0, 0,   0, 1,   1, 1,   1, 0,             // v0,v3,v4,v5 (right)
                             1, 1,   1, 0,   0, 0,   0, 1,             // v0,v5,v6,v1 (top)
                             1, 0,   0, 0,   0, 1,   1, 1,             // v1,v6,v7,v2 (left)
                             0, 1,   1, 1,   1, 0,   0, 0,             // v7,v4,v3,v2 (bottom)
                             0, 1,   1, 1,   1, 0,   0, 0, };          // v4,v7,v6,v5 (back)

    // index array of vertex array for glDrawElements() & glDrawRangeElement()
    GLubyte indices[]  = { 0, 1, 2,   2, 3, 0,      // front
                           4, 5, 6,   6, 7, 4,      // right
                           8, 9,10,  10,11, 8,      // top
                          12,13,14,  14,15,12,      // left
                          16,17,18,  18,19,16,      // bottom
                          20,21,22,  22,23,20 };    // back
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);

    // enable and specify pointers to vertex arrays
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glNormalPointer(GL_FLOAT, 0, normals);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    glBindTexture(GL_TEXTURE_2D, id_ogl_texture[0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, &indices[0]);
    glBindTexture(GL_TEXTURE_2D, id_ogl_texture[1]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, &indices[6*1]);
    glBindTexture(GL_TEXTURE_2D, id_ogl_texture[2]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, &indices[6*2]);
    glBindTexture(GL_TEXTURE_2D, id_ogl_texture[3]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, &indices[6*3]);
    glBindTexture(GL_TEXTURE_2D, id_ogl_texture[4]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, &indices[6*4]);
    glBindTexture(GL_TEXTURE_2D, id_ogl_texture[5]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, &indices[6*5]);

    glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glPopMatrix();
    update();

}

void CGLView::paintGL()
{
    QTime time_paintbegin;

	time_paintbegin = QTime::currentTime();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    // Apply current camera transformations.
	glMultMatrixf((GLfloat*)&mHelper_Camera.Rotation_AroundCamera);
	glTranslatef(-mHelper_Camera.Translation_ToScene.x, -mHelper_Camera.Translation_ToScene.y, -mHelper_Camera.Translation_ToScene.z);
	glMultMatrixf((GLfloat*)&mHelper_Camera.Rotation_Scene);
    glDisable( GL_LIGHTING );

    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);
    glRotatef(zrot, 0.0f, 0.0f, 1.0f);

    glScalef(scale, scale, scale);

    glDisable(GL_LIGHTING);

    DoCubeDisplay();

    update();
}


CGLView::CGLView(QWidget *pParent)
    : QOpenGLWidget(pParent)
{
	static_assert(sizeof(GLfloat) == sizeof(ai_real), "ai_real in Assimp must be equal to GLfloat/float.");///TODO: may be templates can be used.
    memset(textures, 0, sizeof(textures));

	// set initial view
	mHelper_CameraDefault.SetDefault();
	Camera_Set(0);
    scale = 1.0;
}

CGLView::~CGLView()
{
}

void CGLView::Camera_Set(const size_t pCameraNumber)
{
    SHelper_Camera& hcam = mHelper_Camera;// reference with short name for conveniance.
    aiVector3D up;

    if(mCamera_DefaultAdded)
	{
		// Transformation parameters
		hcam = mHelper_CameraDefault;
		up.Set(0, 1, 0);
	}
	else
	{
		aiMatrix4x4 camera_mat;
		aiQuaternion camera_quat_rot;
		aiVector3D camera_tr;

		up = camera_cur.mUp;

		hcam.Position = camera_cur.mLookAt;
		hcam.Target = camera_cur.mPosition;
		hcam.Rotation_AroundCamera = aiMatrix4x4(camera_quat_rot.GetMatrix());
		hcam.Rotation_AroundCamera.Transpose();
		// get components of transformation matrix.
		camera_mat.DecomposeNoScaling(camera_quat_rot, camera_tr);
		hcam.Rotation_Scene = aiMatrix4x4();
		hcam.Translation_ToScene = camera_tr;
	}

	// Load identity matrix - travel to world begin.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Set camera and update picture
	gluLookAt(hcam.Position.x, hcam.Position.y, hcam.Position.z, hcam.Target.x, hcam.Target.y, hcam.Target.z, up.x, up.y, up.z);
}

void CGLView::Camera_RotateScene(const GLfloat pAngle_X, const GLfloat pAngle_Y, const GLfloat pAngle_Z)
{
    auto deg2rad = [](const GLfloat pDegree) -> GLfloat { return pDegree * M_PI / 180.0; };

	aiMatrix4x4 mat_rot;

	mat_rot.FromEulerAnglesXYZ(deg2rad(pAngle_X), deg2rad(pAngle_Y), deg2rad(pAngle_Z));
	mHelper_Camera.Rotation_Scene *= mat_rot;
}

void CGLView::Camera_Rotate(const GLfloat pAngle_X, const GLfloat pAngle_Y, const GLfloat pAngle_Z)
{
    auto deg2rad = [](const GLfloat pDegree) -> GLfloat { return pDegree * M_PI / 180.0; };

	aiMatrix4x4 mat_rot;

	mat_rot.FromEulerAnglesXYZ(deg2rad(pAngle_X), deg2rad(pAngle_Y), deg2rad(pAngle_Z));
	mHelper_Camera.Rotation_AroundCamera *= mat_rot;
}

void CGLView::Camera_Translate(const GLfloat pTranslate_X, const GLfloat pTranslate_Y, const GLfloat pTranslate_Z)
{
    aiVector3D vect_tr(pTranslate_X, pTranslate_Y, pTranslate_Z);

	vect_tr *= mHelper_Camera.Rotation_AroundCamera;
	mHelper_Camera.Translation_ToScene += vect_tr;
}

void CGLView::qglColor(QColor color)
{
    glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

void CGLView::qglClearColor(QColor clearColor)
{
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
}


