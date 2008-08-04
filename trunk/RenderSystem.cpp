#include "RenderSystem.h"
#include "ViewerSettings.h"

//------------------------------------------------------------

RenderSystem::RenderSystem( GameWorld* w )
{
	proj = false;
	world = w;
	tex = TextureLoad("4.bmp", GL_FALSE, GL_LINEAR, GL_LINEAR, GL_REPEAT);
	InitViewerSettings();
	InitGL();
	InitProjection();
}
//------------------------------------------------------------

RenderSystem::~RenderSystem()
{
	glDeleteTextures(1, &tex);
}
//------------------------------------------------------------

bool RenderSystem::LoadScene( const char* filename )
{
	if ( !scene.LoadScene( filename ) )
		return false;
	CompileScene();
	return true;
}
//------------------------------------------------------------

void RenderSystem::CompileScene()
{
	if ( list_scene )
		glDeleteLists( list_scene, 1 );
	list_scene = glGenLists( 1 );
	glNewList( list_scene, GL_COMPILE );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glEnable(GL_TEXTURE_2D);

	glVertexPointer( 3, GL_FLOAT, sizeof( Vector3D ), scene.vertex );
	glNormalPointer( GL_FLOAT, sizeof( Vector3D ), scene.normal );

	glBindTexture(GL_TEXTURE_2D,tex);
	glTexCoordPointer( 2, GL_FLOAT, sizeof( Vector2D ), scene.uv);

	glDrawElements( GL_TRIANGLES, scene.num_indices, GL_UNSIGNED_SHORT, scene.index );

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisable(GL_TEXTURE_2D);

	glEndList();
}
//------------------------------------------------------------

void RenderSystem::InitGL()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Lights and materials
	float gLight0Position[] = {-40.0f, 10.0f,  80.0f, 1.0f};
	float gLight0Ambient[]  = {0.8f, 0.8f, 0.8f, 1.0f};
	float gLight0Diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
	float gLight0Specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

	float gLight1Position[] = { 90.5f, 70.0f,-100.5f, 1.0f};
	float gLight1Ambient[]  = {0.3f, 0.1f, 0.1f, 1.0f};
	float gLight1Diffuse[]  = {0.3f, 0.1f, 0.1f, 1.0f};
	float gLight1Specular[] = {1.0f, 0.3f, 0.3f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, gLight0Position);
	glLightfv(GL_LIGHT1, GL_POSITION, gLight1Position);

	glLightfv(GL_LIGHT0, GL_AMBIENT,  gLight0Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  gLight0Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, gLight0Specular);

	glLightfv(GL_LIGHT1, GL_AMBIENT,  gLight1Ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  gLight1Diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, gLight1Specular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);

	float ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
	float diffuse[] = {0.6f, 0.6f, 0.6f, 1.0f};
	float specularOn[] = {1.0f, 1.0f, 1.0f, 1.0f};
	float specularOff[] = {0.0f, 0.0f, 0.0f, 0.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularOn);
	glMaterialf(GL_FRONT, GL_SHININESS, 80.0f);

	glDepthFunc(GL_LEQUAL);
	//glDepthFunc(GL_LESS);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	
}
//------------------------------------------------------------

void RenderSystem::InitProjection()
{
	//初始化纹理分片坐标和屏幕分片坐标，为分片贴纹理准备
	float texOffsetX = ( float ) 1 / segmentX;
	float texOffsetY = ( float ) 1 / segmentY;
	float screenOffsetX = ( float ) width / segmentX;
	float screenOffsetY = ( float ) height / segmentY;

	texPoint[0][0].x=0;
	texPoint[0][0].y=0;
	texPoint[0][segmentX].x=1;
	texPoint[0][segmentX].y=0;

	screenPoint[0][0].x=0;
	screenPoint[0][0].y=0;
	screenPoint[0][segmentX].x=width;
	screenPoint[0][segmentX].y=0;

	for (int i=0;i< segmentY+1; i++){
		if (i>0){
			texPoint[i][0].x=0;
			texPoint[i][0].y=texPoint[i-1][0].y+texOffsetY;
			texPoint[i][segmentX].x=1;
			texPoint[i][segmentX].y=texPoint[i][0].y;

			screenPoint[i][0].x=0;
			screenPoint[i][0].y=screenPoint[i-1][0].y+screenOffsetY;
			screenPoint[i][segmentX].x=width;
			screenPoint[i][segmentX].y=screenPoint[i][0].y;
		}
		for (int j=1;j<segmentX; j++){
			texPoint[i][j].x=texPoint[i][j-1].x+texOffsetX;
			texPoint[i][j].y=texPoint[i][j-1].y;

			screenPoint[i][j].x=screenPoint[i][j-1].x+screenOffsetX;
			screenPoint[i][j].y=screenPoint[i][j-1].y;
		}
	}

#ifdef GL_VERSION_1_1
	glGenTextures(1, &texName);
#endif
}
//------------------------------------------------------------

void RenderSystem::SetupRenderMode()
{
	if (g_viewerSettings.renderMode == RM_WIREFRAME)
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
		glDisable (GL_TEXTURE_2D);
		glDisable (GL_CULL_FACE);
		glEnable (GL_DEPTH_TEST);
	}
	else if (g_viewerSettings.renderMode == RM_FLATSHADED ||
		g_viewerSettings.renderMode == RM_SMOOTHSHADED)
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
		glDisable (GL_TEXTURE_2D);
		glEnable (GL_CULL_FACE);
		glEnable (GL_DEPTH_TEST);

		if (g_viewerSettings.renderMode == RM_FLATSHADED)
			glShadeModel (GL_FLAT);
		else
			glShadeModel (GL_SMOOTH);
	}
	else if (g_viewerSettings.renderMode == RM_TEXTURED)
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
		glEnable (GL_TEXTURE_2D);
		glEnable (GL_CULL_FACE);
		glEnable (GL_DEPTH_TEST);
		glShadeModel (GL_SMOOTH);
	}
}
//------------------------------------------------------------
//画人物模型
void RenderSystem::DrawModel( RenderModel& model )
{
	if ( list_model )
		glDeleteLists( list_model, 1 );
	list_model = glGenLists( 1 );
	glNewList( list_model, GL_COMPILE_AND_EXECUTE );

	glCullFace( GL_FRONT );
	model.Draw();

	glEndList();
}
//------------------------------------------------------------
////更改状态，ani代表了动作序列,speed代表动作速度，dt代表走路跳跃跨度
//void RenderSystem::Update( float dt )
//{
//	for( int i = 0; i < world->getPlayerNum(); ++ i )
//	{
//		int ani = 1;
//		float speed = 2.0f;
//
//		Player* p = world->getPlayer(i), *p2;
//
//		for ( int j = 0; j < world->getPlayerNum(); ++ j )	//模型AABB碰撞检测
//		{
//			p2 = world->getPlayer(j);
//			Vector2D v = p2->getPosition() - p->getPosition();
//
//			if ( i != j && world->getModel(i)->AABBTest( *world->getModel(j), v.x, v.y, p2->getHeight() - p->getHeight() ) )
//			{
//				p->setCollPlayer( p2 );
//				p2->setCollPlayer( p );
//			}
//		}
//
//		p2 = p->getCollPlayer();
//
//		if ( p2 != NULL && p2->currentCommand != NULL )
//		{			
//			p2->currentCommand->TargetPlayer = p;
//		}
//
//		ani = p->AnimationSequence;
//		speed = p->AnimationSpeed;
//
//		world->getModel(i)->SetSequence( ani );//根据状态跟新角色模型的这些数据
//		world->getModel(i)->Update( dt, speed );
//
//		//等待一个动作做完设置为闲置状态
//
//			p->setSpeedHor( Vector2D() );
//			if ( world->getModel(i)->IsAniEnd() )
//			{
//				p->AnimationSequence = 1;
//				p->AnimationSpeed = 2.0f;
//				p->isForceAnimation = false;
//			}
//
//
//	}
//}
////------------------------------------------------------------

//最终画场景的方法
void RenderSystem::Draw()
{
	//glViewport (0, 0, (GLsizei) width, (GLsizei) height); 
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ();
	gluPerspective( 45.0, (double)width / height, 1.0f, 4096.0f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glClearColor( 0.0, 0.0, 1.0, 0.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();

	if ( proj )
	{
		float up_x=world->getCamera()->dx - world->getCamera()->x;
		float up_y=world->getCamera()->dy - world->getCamera()->y;
		float up_z=(1+up_y*up_y)*up_x*up_x/(world->getCamera()->dz - world->getCamera()->z);

		gluLookAt( world->getCamera()->x, world->getCamera()->y, world->getCamera()->z,
			world->getCamera()->dx, world->getCamera()->dy, world->getCamera()->dz,
			up_x,up_y,up_z);
	}
	else
	{
		gluLookAt( world->getCamera()->x, world->getCamera()->y, world->getCamera()->z,
			world->getCamera()->dx, world->getCamera()->dy, world->getCamera()->dz,
			world->getCamera()->ux, world->getCamera()->uy, world->getCamera()->uz );
	}

	glGetIntegerv (GL_VIEWPORT, viewport);
	glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

	glDisable( GL_CULL_FACE );
	glEnable ( GL_DEPTH_TEST );
	glCallList( list_scene );	//执行之前定义的画图列表,画场景


	//人物绘制
	Player* p;

	for( int i = 0; i < world->getPlayerNum(); ++ i )
	{
		p = world->getPlayer(i);

		glPushMatrix();

		glTranslatef( p->getPosition().x, p->getPosition().y, p->getHeight() + p->getStature() / 2 );
		glRotatef( p->getRotation(), 0.0f, 0.0f, 1.0f );

		g_vright[0] = g_vright[1] = g_viewerSettings.trans[2];
		SetupRenderMode();

		DrawModel( *world->getModel(i) );

		glPopMatrix();
	}
	glPopMatrix();

	//Projection();
	//glutSwapBuffers();
	//glutSwapBuffers();
	
}
//------------------------------------------------------------

void RenderSystem::SwitchViewMode()
{
	proj = !proj;
}
//------------------------------------------------------------

void RenderSystem::getRealCoordinate( Vector2D& in, Vector2D& out )
{
	FixedCamera* eye = world->getCamera();
	gluUnProject (in.x, in.y, 0.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
	out.x = wx-wz*(wx-eye->x)/(wz-eye->z);
	out.y = wy-wz*(wy-eye->y)/(wz-eye->z);
}
//------------------------------------------------------------

void RenderSystem::Projection()
{
	glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,image);

#ifdef GL_VERSION_1_1
	glBindTexture(GL_TEXTURE_2D, texName);
#endif

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

#ifdef GL_VERSION_1_1
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 
		0, GL_RGB, GL_UNSIGNED_BYTE, image);
#endif

	//glViewport (0, 0, (GLsizei) width, (GLsizei) height); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(-200.0, 200.0, -200.0, 200.0);   //投影的范围
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

#ifdef GL_VERSION_1_1
	glBindTexture(GL_TEXTURE_2D, texName);
#endif

	if ( proj )
	{
		for (int i=0;i< segmentY+1; i++){
			for (int j=0;j<segmentX+1; j++){
				getRealCoordinate(screenPoint[i][j],segmentPoint[i][j]);
			}
		}

		for ( int i = 0; i < segmentY; i++ ){
			for ( int j = 0; j < segmentX; j++ ){
				glBegin(GL_QUADS);
				glTexCoord2f(texPoint[i][j].x, texPoint[i][j].y);
				glVertex2f(segmentPoint[i][j].x, segmentPoint[i][j].y);
				glTexCoord2f(texPoint[i+1][j].x, texPoint[i+1][j].y);
				glVertex2f(segmentPoint[i+1][j].x,segmentPoint[i+1][j].y);
				glTexCoord2f(texPoint[i+1][j+1].x, texPoint[i+1][j+1].y);
				glVertex2f(segmentPoint[i+1][j+1].x,segmentPoint[i+1][j+1].y);
				glTexCoord2f(texPoint[i][j+1].x, texPoint[i][j+1].y);
				glVertex2f(segmentPoint[i][j+1].x,segmentPoint[i][j+1].y);
				glEnd();
			}
		}
	}

	glDisable(GL_TEXTURE_2D);
}