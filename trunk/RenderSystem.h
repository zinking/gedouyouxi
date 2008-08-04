#pragma once

#include "GameWorld.h"
#include "RenderModel.h"
#include "RenderScene.h"
#include "texture.h"
#include <vector>
#include <gl/glut.h>

using namespace std;

class RenderSystem
{
	bool proj;	//是否投影

	GLuint list_scene;
	GLuint list_model;
	GLuint tex;	//场景纹理
	GameWorld* world;

	RenderScene scene;

	static const int width = 800;
	static const int height = 600;

	static const int segmentX = 64;
	static const int segmentY = 64;

	//储存矩阵，为求视口对应的世界坐标
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	GLdouble wx, wy, wz;

	GLubyte image[height][width][3];
	Vector2D segmentPoint[segmentX+1][segmentY+1];
	Vector2D texPoint[segmentX+1][segmentY+1];
	Vector2D screenPoint[segmentX+1][segmentY+1];

#ifdef GL_VERSION_1_1
	GLuint texName;
#endif

public:
	RenderSystem( GameWorld* w );
	~RenderSystem();

	bool LoadScene( const char* filename );
	void CompileScene();

	void InitGL();
	void InitProjection();

	void SetupRenderMode();

	void DrawModel( RenderModel& model );

//	void Update( float dt );
	void Draw();

	void SwitchViewMode();
	void getRealCoordinate( Vector2D& in, Vector2D& out );
	void Projection();

	static int getWidth(){ return width; }
	static int getHeight(){ return height; }
};
