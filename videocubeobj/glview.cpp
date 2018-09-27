#include "glview.hpp"
#include <QTime>
#include <QDebug>
#include <QMessageBox>

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

void CGLView::InitTextures()
{
    QMessageBox msgBox;

    // Before calling ilInit() version should be checked.
    if(ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
    {
        msgBox.setText("ERROR IL_VERSION");
        msgBox.setStandardButtons(QMessageBox::Ok);
        int ret = msgBox.exec();
        exit(0);
    }

    ilInit();// Initialization of DevIL.

    glGenTextures(6, id_ogl_texture);// Texture ID generation.

    capture.open(videoFile);

    if(!capture.isOpened()){
        //cout<<"ERROR ACQUIRING VIDEO FEED\n";
        //getchar();

        msgBox.setText("ERROR ACQUIRING VIDEO FEED");
        msgBox.setStandardButtons(QMessageBox::Ok);
        //msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        exit(0);
    }
    //set height and width of capture frame
    //capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
    //capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
}

std::string CGLView::intToString(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}
void CGLView::drawObject(int x, int y,cv::Mat &frame)
{
    cv::circle(frame,cv::Point(x,y),20,cv::Scalar(0,255,0),2);
    if(y-25>0)
        cv::line(frame,cv::Point(x,y),cv::Point(x,y-25),cv::Scalar(0,255,0),2);
    else line(frame,cv::Point(x,y),cv::Point(x,0),cv::Scalar(0,255,0),2);
    if(y+25<FRAME_HEIGHT)
        cv::line(frame,cv::Point(x,y),cv::Point(x,y+25),cv::Scalar(0,255,0),2);
    else line(frame,cv::Point(x,y),cv::Point(x,FRAME_HEIGHT),cv::Scalar(0,255,0),2);
    if(x-25>0)
        cv::line(frame,cv::Point(x,y),cv::Point(x-25,y),cv::Scalar(0,255,0),2);
    else line(frame,cv::Point(x,y),cv::Point(0,y),cv::Scalar(0,255,0),2);
    if(x+25<FRAME_WIDTH)
        cv::line(frame,cv::Point(x,y),cv::Point(x+25,y),cv::Scalar(0,255,0),2);
    else line(frame,cv::Point(x,y),cv::Point(FRAME_WIDTH,y),cv::Scalar(0,255,0),2);

    cv::putText(frame,intToString(x)+","+intToString(y),cv::Point(x,y+30),1,1,cv::Scalar(0,255,0),2);

}
void CGLView::morphOps(cv::Mat &thresh)
{
    //create structuring element that will be used to "dilate" and "erode" image.
    cv::Mat erodeElement = cv::getStructuringElement( cv::MORPH_RECT,cv::Size(11,11));
    cv::Mat dilateElement = cv::getStructuringElement( cv::MORPH_RECT,cv::Size(17,17));
    cv::erode(thresh,thresh,erodeElement);
    cv::dilate(thresh,thresh,dilateElement);
}

void CGLView::trackFilteredObject(int &x, int &y, cv::Mat threshold, cv::Mat &cameraFeed)
{
    cv::Mat temp;
    threshold.copyTo(temp);
    //these two vectors needed for output of findContours
    std::vector< std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    //find contours of filtered image using openCV findContours function
    cv::findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
    //use moments method to find our filtered object
    double refArea = 0;
    bool objectFound = false;
    if (hierarchy.size() > 0) {
        int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS){
            for (int index = 0; index >= 0; index = hierarchy[index][0]) {

                cv::Moments moment = cv::moments((cv::Mat)contours[index]);
                double area = moment.m00;

                //if the area is less than 20 px by 20px then it is probably just noise
                //if the area is the same as the 3/2 of the image size, probably just a bad filter
                //we only want the object with the largest area so we safe a reference area each
                //iteration and compare it to the area in the next iteration.
                if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
                    x = moment.m10/area;
                    y = moment.m01/area;
                    objectFound = true;
                    refArea = area;
                }else objectFound = false;
            }
            //let user know you found an object
            if(objectFound ==true){
                cv::putText(cameraFeed,"Tracking Object",cv::Point(0,50),2,1,cv::Scalar(0,255,0),2);
                //draw object location on screen
                drawObject(x,y,cameraFeed);}

        }else cv::putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",cv::Point(0,50),1,2,cv::Scalar(0,0,255),2);
    }
}

void CGLView::ImportTextures()
{
    if(capture.get(CV_CAP_PROP_POS_FRAMES)<capture.get(CV_CAP_PROP_FRAME_COUNT)-1) {
        //store image to matrix
        capture.read(cameraFeed);
    } else {
        capture.release();
        capture.open(videoFile);
        capture.read(cameraFeed);
    }

    //convert frame from BGR to HSV colorspace
    cv::cvtColor(cameraFeed,HSV,cv::COLOR_BGR2HSV);
    //filter HSV image between values and store filtered image to
    //threshold matrix
    cv::inRange(HSV,cv::Scalar(H_MIN,S_MIN,V_MIN),cv::Scalar(H_MAX,S_MAX,V_MAX),threshold);
    //perform morphological operations on thresholded image to eliminate noise
    //and emphasize the filtered object(s)
    morphOps(threshold);

    trackFilteredObject(x,y,threshold,cameraFeed);

    std::vector<uchar> buf;
    std::vector<int> params(2);
    params[0] = CV_IMWRITE_JPEG_QUALITY;
    params[1] = 80;

    cv::imencode(".bmp", cameraFeed, buf);
    //cv::imencode(".jpg", cameraFeed, buf, params );
    ILuint size = buf.size();

    ILboolean success;

    ILuint id_image;// DevIL image ID.

    ilGenImages(1, &id_image);// Generate DevIL image ID.
    ilBindImage(id_image);

    success = ilLoadL(IL_BMP, (const void *)&buf[0], size);
    if(!success)
    {
        return ;
    }
    // Convert every colour component into unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA.
    success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    if(!success)
    {
        return ;
    }

    auto LoadTexture = [&](int n) -> bool
    {
        glBindTexture(GL_TEXTURE_2D, id_ogl_texture[n]);// Binding of texture ID.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// We will use linear interpolation for magnification filter.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// We will use linear interpolation for minifying filter.
        glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0,
                        ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());// Texture specification.

	    return true;
    };

    for(size_t idx_material = 0; idx_material < 6; idx_material++)
	{
        LoadTexture(idx_material);
    }

    //Cleanup
    ilDeleteImages(1, &id_image);// Because we have already copied image data into texture data we can release memory used by image.

}

void CGLView::BBox_GetForNode(const aiNode& pNode, const aiMatrix4x4& pParent_TransformationMatrix, SBBox& pNodeBBox, bool& pFirstAssign)
{
    aiMatrix4x4 mat_trans = pParent_TransformationMatrix * pNode.mTransformation;

    // Check if node has meshes
    for(size_t idx_idx_mesh = 0; idx_idx_mesh < pNode.mNumMeshes; idx_idx_mesh++)
    {
        size_t idx_mesh;
        SBBox bbox_local;
        aiVector3D bbox_vertices[8];

        idx_mesh = pNode.mMeshes[idx_idx_mesh];
        // Get vertices of mesh BBox
        BBox_GetVertices(mHelper_Mesh[idx_mesh]->BBox, bbox_vertices);
        // Transform vertices
        for(size_t idx_vert = 0; idx_vert < 8; idx_vert++) bbox_vertices[idx_vert] *= mat_trans;

        // And create BBox for transformed mesh
        BBox_GetFromVertices(bbox_vertices, 8, bbox_local);

        if(!pFirstAssign)
        {
            BBox_Extend(bbox_local, pNodeBBox);
        }
        else
        {
            pFirstAssign = false;
            pNodeBBox = bbox_local;
        }
    }// for(size_t idx_idx_mesh = 0; idx_idx_mesh < pNode.mNumMeshes; idx_idx_mesh++)

    for(size_t idx_node = 0; idx_node < pNode.mNumChildren; idx_node++)
    {
        BBox_GetForNode(*pNode.mChildren[idx_node], mat_trans, pNodeBBox, pFirstAssign);
    }
}

void CGLView::BBox_Extend(const SBBox& pChild, SBBox& pParent)
{
    // search minimal...
    AssignIfLesser(&pParent.Minimum.x, pChild.Minimum.x);
    AssignIfLesser(&pParent.Minimum.y, pChild.Minimum.y);
    AssignIfLesser(&pParent.Minimum.z, pChild.Minimum.z);
    // and maximal values
    AssignIfGreater(&pParent.Maximum.x, pChild.Maximum.x);
    AssignIfGreater(&pParent.Maximum.y, pChild.Maximum.y);
    AssignIfGreater(&pParent.Maximum.z, pChild.Maximum.z);
}

void CGLView::BBox_GetVertices(const SBBox& pBBox, aiVector3D pVertex[8])
{
    pVertex[0] = pBBox.Minimum;
    pVertex[1].Set(pBBox.Minimum.x, pBBox.Minimum.y, pBBox.Maximum.z);
    pVertex[2].Set(pBBox.Minimum.x, pBBox.Maximum.y, pBBox.Maximum.z);
    pVertex[3].Set(pBBox.Minimum.x, pBBox.Maximum.y, pBBox.Minimum.z);

    pVertex[4].Set(pBBox.Maximum.x, pBBox.Minimum.y, pBBox.Minimum.z);
    pVertex[5].Set(pBBox.Maximum.x, pBBox.Minimum.y, pBBox.Maximum.z);
    pVertex[6] = pBBox.Maximum;
    pVertex[7].Set(pBBox.Maximum.x, pBBox.Maximum.y, pBBox.Minimum.z);

}

void CGLView::BBox_GetFromVertices(const aiVector3D* pVertices, const size_t pVerticesQuantity, SBBox& pBBox)
{
    if(pVerticesQuantity == 0)
    {
        pBBox.Maximum.Set(0, 0, 0);
        pBBox.Minimum.Set(0, 0, 0);

        return;
    }

    // Assign first values.
    pBBox.Minimum = pVertices[0];
    pBBox.Maximum = pVertices[0];

    for(size_t idx_vert = 1; idx_vert < pVerticesQuantity; idx_vert++)
    {
        const GLfloat x = pVertices[idx_vert].x;
        const GLfloat y = pVertices[idx_vert].y;
        const GLfloat z = pVertices[idx_vert].z;

        // search minimal...
        AssignIfLesser(&pBBox.Minimum.x, x);
        AssignIfLesser(&pBBox.Minimum.y, y);
        AssignIfLesser(&pBBox.Minimum.z, z);
        // and maximal values
        AssignIfGreater(&pBBox.Maximum.x, x);
        AssignIfGreater(&pBBox.Maximum.y, y);
        AssignIfGreater(&pBBox.Maximum.z, z);
    }
}


/********************************************************************/
/************************** Draw functions **************************/
/********************************************************************/

void CGLView::Draw_Node(const aiNode* pNode)
{
    aiMatrix4x4 mat_node = pNode->mTransformation;

    // Apply node transformation matrix.
    mat_node.Transpose();
    glPushMatrix();
    glMultMatrixf((GLfloat*)&mat_node);
    // Draw all meshes assigned to this node
    for(size_t idx_mesh_arr = 0; idx_mesh_arr < pNode->mNumMeshes; idx_mesh_arr++)
        Draw_Mesh(pNode->mMeshes[idx_mesh_arr]);

    // Draw all children nodes
    for(size_t idx_node = 0; idx_node < pNode->mNumChildren; idx_node++)
        Draw_Node(pNode->mChildren[idx_node]);

    // Restore transformation matrix.
    glPopMatrix();
}

void CGLView::Draw_Mesh(const size_t pMesh_Index)
{
    // Check argument
    if(pMesh_Index >= mHelper_Mesh_Quantity) return;

    aiMesh& mesh_cur = *mScene->mMeshes[pMesh_Index];

    if(!mesh_cur.HasPositions()) return;// Nothing to draw.

    //glBindTexture(GL_TEXTURE_2D, -1);
    // If mesh use material then apply it
//    if(mScene->HasMaterials())
//        Material_Apply(mScene->mMaterials[mesh_cur.mMaterialIndex]);

    //
    // Vertices array
    //
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, mesh_cur.mVertices);

    if(mesh_cur.HasVertexColors(0))
    {
        glEnable(GL_COLOR_MATERIAL);///TODO: cache
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, 0, mesh_cur.mColors[0]);
    }

    //
    // Texture coordinates array
    //
    if(mesh_cur.HasTextureCoords(0))
    {
        //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        //glTexCoordPointer(2, GL_FLOAT, sizeof(aiVector3D), mesh_cur.mTextureCoords[0]);
    }

    //
    // Normals array
    //
    if(mesh_cur.HasNormals())
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, mesh_cur.mNormals);
    }

    //
    // Draw arrays
    //
    SHelper_Mesh& helper_cur = *mHelper_Mesh[pMesh_Index];

    if(helper_cur.Quantity_Triangle > 0)
        glDrawElements(GL_TRIANGLES, helper_cur.Quantity_Triangle * 3, GL_UNSIGNED_INT, helper_cur.Index_Triangle);
    if(helper_cur.Quantity_Line > 0)
        glDrawElements(GL_LINES,helper_cur.Quantity_Line * 2, GL_UNSIGNED_INT, helper_cur.Index_Line);
    if(helper_cur.Quantity_Point > 0)
        glDrawElements(GL_POINTS, helper_cur.Quantity_Point, GL_UNSIGNED_INT, helper_cur.Index_Point);

    //
    // Clean up
    //
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
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

    InitTextures();

}

void CGLView::resizeGL(int pWidth, int pHeight)
{
	mCamera_Viewport_AspectRatio = (GLdouble)pWidth / pHeight;
	glViewport(0, 0, pWidth, pHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(mCamera_FOVY, mCamera_Viewport_AspectRatio, 1.0, 100000.0);///TODO: znear/zfar depend on scene size.
}

void CGLView::DoCubeDisplay(const SBBox& pBBox)
{
    // ai를 이용해 obj를 load하면 아래와 같은 face를 가짐.
    //    v3----- v7
    //   /|      /|
    //  v2------v6|
    //  | |     | |
    //  | |v0---|-|v4
    //  |/      |/
    //  v1------v5

    // ==> 변환을 위해 elements[] 도입.

    //    v6----- v5
    //   /|      /|
    //  v1------v0|
    //  | |     | |
    //  | |v7---|-|v4
    //  |/      |/
    //  v2------v3

    GLfloat vertices[12*6];
    GLfloat normals[12*6];

    const GLubyte elements[] =
    {
        6, 2, 1, 5, // front
        6, 5, 4, 7, // right
        6, 7, 3, 2, // top
        2, 3, 0, 1, // left
        0, 4, 5, 1, // bottom
        4, 0, 3, 7, // back
    };

    aiVector3D vertex[8];
    BBox_GetVertices(pBBox, vertex);
    for (int i=0; i<24; i++) {
        aiVector3D a = vertex[elements[i]];
        vertices[i*3] = a.x;
        vertices[i*3+1] = a.y;
        vertices[i*3+2] = a.z;
        //qDebug() << a.x << "," << a.y << "," << a.z<< "," ;
    }
    int no[6] = { 2, 0, 1, 0, 1, 2};
    for (int i=0; i<6; i++) {
        for (int j=0; j<4; j++) {
            normals[i*12+j*3+no[i]] = vertices[i*12+j*3+no[i]];
        }
    }

#define B 0.75
    // color array
    GLfloat colors[]   = { 1, 1, 1,B,   1, 1, 1,B,   1, 1, 1,B,   1, 1, 1,B,   // v0,v1,v2,v3 (front)
                           1, 1, 1,B,   1, 1, 1,B,   1, 1, 1,B,   1, 1, 1,B,   // v0,v3,v4,v5 (right)
                           1, 1, 1,B,   1, 1, 1,B,   1, 1, 1,B,   1, 1, 1,B,   // v0,v5,v6,v1 (top)
                           1, 1, 1,B,   1, 1, 1,B,   1, 1, 1,B,   1, 1, 1,B,   // v1,v6,v7,v2 (left)
                           1, 1, 1,B,   1, 1, 1,B,   1, 1, 1,B,   1, 1, 1,B,   // v7,v4,v3,v2 (bottom)
                           1, 1, 1,B,   1, 1, 1,B,   1, 1, 1,B,   1, 1, 1,B }; // v4,v7,v6,v5 (back)

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
    GLfloat texCoords[]  = { 1, 1,   0, 1,   0, 0,   1, 0,             // v0,v1,v2,v3 (front)
                             0, 1,   0, 0,   1, 0,   1, 1,             // v0,v3,v4,v5 (right)
                             0, 1,   0, 0,   1, 0,   1, 1,             // v0,v5,v6,v1 (top)
                             1, 1,   0, 1,   0, 0,   1, 0,             // v1,v6,v7,v2 (left)
                             0, 0,   1, 0,   1, 1,   0, 1,             // v7,v4,v3,v2 (bottom)
                             0, 0,   1, 0,   1, 1,   0, 1,};          // v4,v7,v6,v5 (back)

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

    if (bFileOpening)
        return;

    //QTime time_paintbegin;
    //time_paintbegin = QTime::currentTime();

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

    mLightingEnabled = false;
    if(mLightingEnabled)
        glEnable(GL_LIGHTING);
    else
        glDisable(GL_LIGHTING);

    glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
    glDisable(GL_TEXTURE_2D);

    if (mScene != nullptr) {
        ImportTextures();
        Draw_Node(mScene->mRootNode);
        DoCubeDisplay(mScene_BBox);
    }
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



void CGLView::FreeScene()
{
    // Set scene to null and after that \ref paintGL will not try to render it.
    mScene = nullptr;
    // Clean helper objects.
    if(mHelper_Mesh != nullptr)
    {
        for(size_t idx_mesh = 0; idx_mesh < mHelper_Mesh_Quantity; idx_mesh++) delete mHelper_Mesh[idx_mesh];

        delete [] mHelper_Mesh;
        mHelper_Mesh = nullptr;
    }

    mHelper_Mesh_Quantity = 0;
    // Delete textures
    //for (int i=0; i<6; i++)
    //     glDeleteTextures(6, id_ogl_texture);

//    const int id_tex_size = mTexture_IDMap.size();

//	if(id_tex_size)
//	{
//		GLuint* id_tex = new GLuint[id_tex_size];
//		QMap<QString, GLuint>::iterator it = mTexture_IDMap.begin();

//		for(int idx = 0; idx < id_tex_size; idx++, it++)
//		{
//			id_tex[idx] = it.value();
//		}

//		glDeleteTextures(id_tex_size, id_tex);
//		mTexture_IDMap.clear();
//		delete [] id_tex;
//	}
}

void CGLView::SetScene(const aiScene *pScene, const QString& pScenePath)
{
    FreeScene();// Clear old data
    // Why checking here, not at begin of function. Because old scene may not exist at know. So, need cleanup.
    if(pScene == nullptr) return;

    mScene = pScene;// Copy pointer of new scene.

    //
    // Meshes
    //
    // Create helper objects for meshes. This allow to render meshes as OpenGL arrays.
    if(mScene->HasMeshes())
    {
        // Create mesh helpers array.
        mHelper_Mesh_Quantity = mScene->mNumMeshes;
        mHelper_Mesh = new SHelper_Mesh*[mScene->mNumMeshes];

        // Walk through the meshes and extract needed data and, also calculate BBox.
        for(size_t idx_mesh = 0; idx_mesh < mScene->mNumMeshes; idx_mesh++)
        {
            aiMesh& mesh_cur = *mScene->mMeshes[idx_mesh];

            //
            // Calculate BBox
            //
            SBBox mesh_bbox;

            BBox_GetFromVertices(mesh_cur.mVertices, mesh_cur.mNumVertices, mesh_bbox);
            //
            // Create vertices indices arrays splited by primitive type.
            //
            size_t indcnt_p = 0;// points quantity
            size_t indcnt_l = 0;// lines quantity
            size_t indcnt_t = 0;// triangles quantity

            if(mesh_cur.HasFaces())
            {
                // Usual way: all faces are triangles
                if(mesh_cur.mPrimitiveTypes == aiPrimitiveType_TRIANGLE)
                {
                    indcnt_t = mesh_cur.mNumFaces;
                }
                else
                {
                    // Calculate count of primitives by types.
                    for(size_t idx_face = 0; idx_face < mesh_cur.mNumFaces; idx_face++)
                    {
                        if(mesh_cur.mFaces[idx_face].mNumIndices == 3)
                            indcnt_t++;
                        else if(mesh_cur.mFaces[idx_face].mNumIndices == 2)
                            indcnt_l++;
                        else if(mesh_cur.mFaces[idx_face].mNumIndices == 1)
                            indcnt_p++;
                    }
                }// if(mesh_cur.mPrimitiveTypes == aiPrimitiveType_TRIANGLE) else

                // Create helper
                mHelper_Mesh[idx_mesh] = new SHelper_Mesh(indcnt_p, indcnt_l, indcnt_t, mesh_bbox);
                // Fill indices arrays
                indcnt_p = 0, indcnt_l = 0, indcnt_t = 0;// Reuse variables as indices
                for(size_t idx_face = 0; idx_face < mesh_cur.mNumFaces; idx_face++)
                {
                    if(mesh_cur.mFaces[idx_face].mNumIndices == 3)
                    {
                        mHelper_Mesh[idx_mesh]->Index_Triangle[indcnt_t++] = mesh_cur.mFaces[idx_face].mIndices[0];
                        mHelper_Mesh[idx_mesh]->Index_Triangle[indcnt_t++] = mesh_cur.mFaces[idx_face].mIndices[1];
                        mHelper_Mesh[idx_mesh]->Index_Triangle[indcnt_t++] = mesh_cur.mFaces[idx_face].mIndices[2];
                    }
                    else if(mesh_cur.mFaces[idx_face].mNumIndices == 2)
                    {
                        mHelper_Mesh[idx_mesh]->Index_Line[indcnt_l++] = mesh_cur.mFaces[idx_face].mIndices[0];
                        mHelper_Mesh[idx_mesh]->Index_Line[indcnt_l++] = mesh_cur.mFaces[idx_face].mIndices[1];
                    }
                    else if(mesh_cur.mFaces[idx_face].mNumIndices == 1)
                    {
                        mHelper_Mesh[idx_mesh]->Index_Point[indcnt_p++] = mesh_cur.mFaces[idx_face].mIndices[0];
                    }
                }// for(size_t idx_face = 0; idx_face < mesh_cur.mNumFaces; idx_face++)
            }// if(mesh_cur.HasFaces())
            else
            {
                // If mesh has no faces then vertices can be just points set.
                indcnt_p = mesh_cur.mNumVertices;
                // Create helper
                mHelper_Mesh[idx_mesh] = new SHelper_Mesh(indcnt_p, 0, 0, mesh_bbox);
                // Fill indices arrays
                for(size_t idx = 0; idx < indcnt_p; idx++) mHelper_Mesh[idx_mesh]->Index_Point[idx] = idx;

            }// if(mesh_cur.HasFaces()) else
        }// for(size_t idx_mesh = 0; idx_mesh < mScene->mNumMeshes; idx_mesh++)
    }// if(mScene->HasMeshes())

    //
    // Scene BBox
    //
    // For calculating right BBox we must walk through all nodes and apply transformation to meshes BBoxes
    if(mHelper_Mesh_Quantity > 0)
    {
        bool first_assign = true;
        aiMatrix4x4 mat_root;

        BBox_GetForNode(*mScene->mRootNode, mat_root, mScene_BBox, first_assign);
        mScene_Center = mScene_BBox.Maximum + mScene_BBox.Minimum;
        mScene_Center /= 2;
    }
    else
    {
        mScene_BBox = {{0, 0, 0}, {0, 0, 0}};
        mScene_Center = {0, 0, 0};
    }// if(mHelper_Mesh_Count > 0) else


    //
    // Light sources
    //
    Lighting_Enable();
    // If scene has no lights then enable default
    if(!mScene->HasLights())
    {
        const GLfloat col_amb[4] = { 0.5, 0.5, 0.5, 1.0 };
        //const GLfloat col_amb[4] = { 0.1, 0.1, 0.1, 1.0 };
        mScene_Center = {0, 0, 10};

        SLightParameters lp;

        lp.Type = aiLightSource_POINT;
        lp.Ambient.r = col_amb[0], lp.Ambient.g = col_amb[1], lp.Ambient.b = col_amb[2], lp.Ambient.a = col_amb[3];
        lp.Diffuse = { 1.0, 1.0, 1.0, 1.0 };
        lp.Specular = lp.Diffuse;
        lp.For.Point.Position = mScene_Center;
        lp.For.Point.Attenuation_Constant = 1;
        lp.For.Point.Attenuation_Linear = 0;
        lp.For.Point.Attenuation_Quadratic = 0;
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, col_amb);
        Lighting_EditSource(0, lp);
        emit SceneObject_LightSource("_default");// Light source will be enabled in signal handler.
    }
    else
    {
        for(size_t idx_light = 0; idx_light < mScene->mNumLights; idx_light++)
        {
            SLightParameters lp;
            QString name;
            const aiLight& light_cur = *mScene->mLights[idx_light];

            auto col3_to_col4 = [](const aiColor3D& pCol3) -> aiColor4D { return aiColor4D(pCol3.r, pCol3.g, pCol3.b, 1.0); };

            ///TODO: find light source node and apply all transformations
            // General properties
            name = light_cur.mName.C_Str();
            lp.Ambient = col3_to_col4(light_cur.mColorAmbient);
            lp.Diffuse = col3_to_col4(light_cur.mColorDiffuse);
            lp.Specular = col3_to_col4(light_cur.mColorSpecular);
            lp.Type = light_cur.mType;
            // Depend on type properties
            switch(light_cur.mType)
            {
                case aiLightSource_DIRECTIONAL:
                    lp.For.Directional.Direction = light_cur.mDirection;
                    break;
                case aiLightSource_POINT:
                    lp.For.Point.Position = light_cur.mPosition;
                    lp.For.Point.Attenuation_Constant = light_cur.mAttenuationConstant;
                    lp.For.Point.Attenuation_Linear = light_cur.mAttenuationLinear;
                    lp.For.Point.Attenuation_Quadratic = light_cur.mAttenuationQuadratic;
                    break;
                case aiLightSource_SPOT:
                    lp.For.Spot.Position = light_cur.mPosition;
                    lp.For.Spot.Direction = light_cur.mDirection;
                    lp.For.Spot.Attenuation_Constant = light_cur.mAttenuationConstant;
                    lp.For.Spot.Attenuation_Linear = light_cur.mAttenuationLinear;
                    lp.For.Spot.Attenuation_Quadratic = light_cur.mAttenuationQuadratic;
                    lp.For.Spot.CutOff = light_cur.mAngleOuterCone;
                    break;
                case aiLightSource_AMBIENT:
                    lp.For.Point.Position = light_cur.mPosition, lp.For.Point.Attenuation_Constant = 1, lp.For.Point.Attenuation_Linear = 0, lp.For.Point.Attenuation_Quadratic = 0;
                    name.append("_unsup_ambient");
                    break;
                case aiLightSource_AREA:
                    lp.For.Point.Position = light_cur.mPosition, lp.For.Point.Attenuation_Constant = 1, lp.For.Point.Attenuation_Linear = 0, lp.For.Point.Attenuation_Quadratic = 0;
                    name.append("_unsup_area");
                    break;
                case aiLightSource_UNDEFINED:
                    lp.For.Point.Position = light_cur.mPosition, lp.For.Point.Attenuation_Constant = 1, lp.For.Point.Attenuation_Linear = 0, lp.For.Point.Attenuation_Quadratic = 0;
                    name.append("_unsup_undefined");
                    break;
                default:
                    lp.For.Point.Position = light_cur.mPosition, lp.For.Point.Attenuation_Constant = 1, lp.For.Point.Attenuation_Linear = 0, lp.For.Point.Attenuation_Quadratic = 0;
                    name.append("_unsupported_invalid");
                    break;
            }// switch(light_cur.mType)

            // Add light source
            if(name.isEmpty()) name += QString("%1").arg(idx_light);// Use index if name is empty.

            Lighting_EditSource(idx_light, lp);
            emit SceneObject_LightSource(name);// Light source will be enabled in signal handler.
        }// for(size_t idx_light = 0; idx_light < mScene->mNumLights; idx_light++)
    }// if(!mScene->HasLights()) else

    //
    // Cameras
    //
    if(!mScene->HasCameras())
    {
        mCamera_DefaultAdded = true;
        mHelper_CameraDefault.SetDefault();
        // Calculate distance from camera to scene. Distance must be enoguh for that viewport contain whole scene.
        const GLfloat tg_angle = tan(mCamera_FOVY / 2);

        GLfloat val_x = ((mScene_BBox.Maximum.x - mScene_BBox.Minimum.x) / 2) / (mCamera_Viewport_AspectRatio * tg_angle);
        GLfloat val_y = ((mScene_BBox.Maximum.y - mScene_BBox.Minimum.y) / 2) / tg_angle;
        GLfloat val_step = val_x;

        AssignIfGreater(val_step, val_y);
        mHelper_CameraDefault.Translation_ToScene.Set(mScene_Center.x, mScene_Center.y, val_step + mScene_BBox.Maximum.z);
        emit SceneObject_Camera("_default");
    }
    else
    {
        mCamera_DefaultAdded = false;
        for(size_t idx_cam = 0; idx_cam < mScene->mNumCameras; idx_cam++)
        {
            emit SceneObject_Camera(mScene->mCameras[idx_cam]->mName.C_Str());
        }
    }// if(!mScene->HasCameras()) else
}


/********************************************************************/
/******************** Lighting control functions ********************/
/********************************************************************/

void CGLView::Lighting_Enable()
{
    mLightingEnabled = true;
    glEnable(GL_LIGHTING);
}

void CGLView::Lighting_Disable()
{
    glDisable(GL_LIGHTING);
    mLightingEnabled = false;
}

void CGLView::Lighting_EditSource(const size_t pLightNumber, const SLightParameters& pLightParameters)
{
const size_t light_num = GL_LIGHT0 + pLightNumber;

GLfloat farr[4];

    if(pLightNumber >= GL_MAX_LIGHTS) return;///TODO: return value;

    glLightfv(light_num, GL_AMBIENT, &pLightParameters.Ambient.r);// Ambient color
    glLightfv(light_num, GL_DIFFUSE, &pLightParameters.Diffuse.r);// Diffuse color
    glLightfv(light_num, GL_SPECULAR, &pLightParameters.Specular.r);// Specular color
    // Other parameters
    switch(pLightParameters.Type)
    {
        case aiLightSource_DIRECTIONAL:
            // Direction
            farr[0] = pLightParameters.For.Directional.Direction.x, farr[2] = pLightParameters.For.Directional.Direction.y;
            farr[2] = pLightParameters.For.Directional.Direction.z; farr[3] = 0;
            glLightfv(light_num, GL_POSITION, farr);
            break;
        case aiLightSource_POINT:
            // Position
            farr[0] = pLightParameters.For.Point.Position.x, farr[2] = pLightParameters.For.Point.Position.y;
            farr[2] = pLightParameters.For.Point.Position.z; farr[3] = 1;
            glLightfv(light_num, GL_POSITION, farr);
            // Attenuation
            glLightf(light_num, GL_CONSTANT_ATTENUATION, pLightParameters.For.Point.Attenuation_Constant);
            glLightf(light_num, GL_LINEAR_ATTENUATION, pLightParameters.For.Point.Attenuation_Linear);
            glLightf(light_num, GL_QUADRATIC_ATTENUATION, pLightParameters.For.Point.Attenuation_Quadratic);
            glLightf(light_num, GL_SPOT_CUTOFF, 180.0);
            break;
        case aiLightSource_SPOT:
            // Position
            farr[0] = pLightParameters.For.Spot.Position.x, farr[2] = pLightParameters.For.Spot.Position.y, farr[2] = pLightParameters.For.Spot.Position.z; farr[3] = 1;
            glLightfv(light_num, GL_POSITION, farr);
            // Attenuation
            glLightf(light_num, GL_CONSTANT_ATTENUATION, pLightParameters.For.Spot.Attenuation_Constant);
            glLightf(light_num, GL_LINEAR_ATTENUATION, pLightParameters.For.Spot.Attenuation_Linear);
            glLightf(light_num, GL_QUADRATIC_ATTENUATION, pLightParameters.For.Spot.Attenuation_Quadratic);
            // Spot specific
            farr[0] = pLightParameters.For.Spot.Direction.x, farr[2] = pLightParameters.For.Spot.Direction.y, farr[2] = pLightParameters.For.Spot.Direction.z; farr[3] = 0;
            glLightfv(light_num, GL_SPOT_DIRECTION, farr);
            glLightf(light_num, GL_SPOT_CUTOFF, pLightParameters.For.Spot.CutOff);
            break;
        default:// For unknown light source types use point source.
            // Position
            farr[0] = pLightParameters.For.Point.Position.x, farr[2] = pLightParameters.For.Point.Position.y;
            farr[2] = pLightParameters.For.Point.Position.z; farr[3] = 1;
            glLightfv(light_num, GL_POSITION, farr);
            // Attenuation
            glLightf(light_num, GL_CONSTANT_ATTENUATION, 1);
            glLightf(light_num, GL_LINEAR_ATTENUATION, 0);
            glLightf(light_num, GL_QUADRATIC_ATTENUATION, 0);
            glLightf(light_num, GL_SPOT_CUTOFF, 180.0);
            break;
    }// switch(pLightParameters.Type)
}

void CGLView::Lighting_EnableSource(const size_t pLightNumber)
{
    if(pLightNumber >= GL_MAX_LIGHTS) return;///TODO: return value;

    glEnable(GL_LIGHT0 + pLightNumber);
}

void CGLView::Lighting_DisableSource(const size_t pLightNumber)
{
    if(pLightNumber >= GL_MAX_LIGHTS) return;///TODO: return value;

    glDisable(GL_LIGHT0 + pLightNumber);
}
