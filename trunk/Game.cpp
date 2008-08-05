#include "LogicGame.h"
#include "GameWorld.h"
#include "Client.h"
#include "Server.h"
#include "Player.h"
#include "FPS_COUNTER.h"
#include <gl/glut.h>
#include <iostream>
#include <process.h>

using namespace std;

#pragma warning(disable:4832)
//STANDALONE_WITHOUT_NETWORK 使用这个宏编译无网络的独立版本



const int ANI_MSEL = 10;	//一帧动画的毫秒数
float dt = (float) ANI_MSEL / 1000;

LogicGame* game;
GameWorld* world;
Player* pServer;
Player* pClient;

int ServerNumber;

//Frames per second counter
FPS_COUNTER fpsCounter;


//计时针对动作
int time_count = 0;
int stateCommand = -1;

#define CONVERT(x)  ( (stateCommand = x , (char*) & stateCommand) )
#define GETCOM(x,y)  ( CommandLookUpTable::getNewCommand( x,y ) )
//------------------------------------------------------------

void showFPS()
{
	//Update frames per second counter
	fpsCounter.Update();
	//Print fps
	static char fpsString[32];
	sprintf_s(fpsString, "fps:%.2f", fpsCounter.GetFps());
	//Set matrices for ortho
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//Print text
	glRasterPos2f(-1.0f, 0.9f);
	for(unsigned int i=0; i<strlen(fpsString); ++i)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fpsString[i]);

	//reset matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


//------------------------------------------------------------

void reshape( int w, int h )
{
	glViewport ( 0, 0, w, h );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ();
	gluPerspective( 45.0, ( double ) w / h, 1.0f, 4096.0f );
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ();
}
//------------------------------------------------------------

void display()
{
	game->getRenderSys()->Draw();	
	game->UpdateGame( dt );
	showFPS();

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();

}

#ifndef STANDALONE_WITHOUT_NETWORK

#define SERVER_PUSHSEND(x) world->getMainCommandQueue()->push(  GETCOM( x ,pServer)  );\
	world->getServer()->write(  CONVERT( x) );
#define CLIENT_PUSHSEND(x) world->getMainCommandQueue()->push(  GETCOM( x ,pClient)  );\
	world->getClient()->write(  CONVERT( x) );

#else
#define SERVER_PUSHSEND(x)  world->getMainCommandQueue()->push(  GETCOM( x ,pServer)  );  
#define CLIENT_PUSHSEND(x)  world->getMainCommandQueue()->push(  GETCOM( x ,pClient)  ); 

#endif


void keyboard( unsigned char key, int x, int y )
{

		if(game->getWorld()->IsServer())
		{
			switch ( key )
			{
				case 27:
					world->getServer()->write( CONVERT( 1000) );
					exit(0);
					break;
				case 32:
					cout << "jump" << endl;
					SERVER_PUSHSEND(JUMP);
					break;
				case 'a':
					cout << "weakfight" << endl;
					SERVER_PUSHSEND(WEAKFIGHT)
					break;
				case 's':
					cout << "strongfight" << endl;
					SERVER_PUSHSEND(STRONGFIGHT)
					break;
				case 'd':
					cout << "qigong" << endl;
					SERVER_PUSHSEND(QIGONG)
					break;
				case 'z':
					cout << "weakfoot" << endl;
					SERVER_PUSHSEND(WEAKFOOT)
					break;
				case 'x':
					cout << "strongfoot" << endl;
					SERVER_PUSHSEND(STRONGFOOT)
					break;
			}
		}
		else
		{
			switch ( key )
			{
				case 27:
					world->getClient()->write( CONVERT( 1000) );
					exit(0);
				case 32:
					cout << "jump" << endl;
					CLIENT_PUSHSEND(JUMP)
					break;
				case 'a':
					cout << "weakfight" << endl;
					CLIENT_PUSHSEND(WEAKFIGHT)
					break;
				case 's':
					cout << "strongfight" << endl;
					CLIENT_PUSHSEND(STRONGFIGHT)
					break;
				case 'd':
					cout << "qigong" << endl;
					CLIENT_PUSHSEND(QIGONG)
					break;
				case 'z':
					cout << "weakfoot" << endl;
					CLIENT_PUSHSEND(WEAKFOOT)
					break;
				case 'x':
					cout << "strongfoot" << endl;
					CLIENT_PUSHSEND(STRONGFOOT)
					break;

			}
		}

		display();
}
//------------------------------------------------------------
void keyboard2( int key, int x, int y )
{
	if ( key == GLUT_KEY_F1 )
	{
		//F1换投影模式
		game->getRenderSys()->SwitchViewMode();
	}

		if ( game->getWorld()->IsServer() )
		{
			switch ( key )
			{
				case GLUT_KEY_UP:
					cout << "up" << endl;
					SERVER_PUSHSEND(RUN);
					break;
				case GLUT_KEY_DOWN:
					cout << "down" << endl;
					SERVER_PUSHSEND(RUNBACK);
					break;
				case GLUT_KEY_LEFT:
					cout << "left" << endl;
					SERVER_PUSHSEND(LEFT);
					break;
				case GLUT_KEY_RIGHT:
					cout << "right" << endl;
					SERVER_PUSHSEND(RIGHT);
					break;
			}
		} 
		else
		{
			switch ( key )
			{
				case GLUT_KEY_UP:
					cout << "up" << endl;
					CLIENT_PUSHSEND(JUMP);
					break;
				case GLUT_KEY_DOWN:
					cout << "down" << endl;
					CLIENT_PUSHSEND(JUMP);
					break;
				case GLUT_KEY_LEFT:
					cout << "left" << endl;
					CLIENT_PUSHSEND(JUMP);
					break;
				case GLUT_KEY_RIGHT:
					cout << "right" << endl;
					CLIENT_PUSHSEND(JUMP);
					break;
			}
		}

		display();
}

//------------------------------------------------------------
void keyboard1Up( unsigned char key, int x, int y )
{
}

//------------------------------------------------------------
void keyboard2Up( int key, int x, int y )
{
	switch ( key )
	{
		case GLUT_KEY_UP:
			cout << "up_up" << endl;
			break;
		case GLUT_KEY_DOWN:
			cout << "down_up" << endl;
			break;
		case GLUT_KEY_LEFT:
			cout << "left_up" << endl;
			break;
		case GLUT_KEY_RIGHT:
			cout << "right_up" << endl;
			break;
	}

}




void ClientReceiveMessage(void* )
{
	char str[20];
	while(true)
	{
		world->getClient()->read(str);//要阻塞也是在这个线程里
		int stateCommand = * ( (int*) str );
		world->getNetCommandQueue()->push( GETCOM( (CommandCategory)stateCommand,pServer) );
	}

}

void ServerReceiveMessage(void *)
{	
	char str[20];
	while(true)
	{
		world->getServer()->read(str);//要阻塞也是在这个线程里		
		//重构代码
		int stateCommand = * ( (int*) str );
		world->getNetCommandQueue()->push( GETCOM((CommandCategory)stateCommand,pClient) );

	}
}

void main( int argc, char** argv )
{

#ifndef STANDALONE_WITHOUT_NETWORK
	cout << "0 for server and other for client: ";
	cin >> ServerNumber;
	if( ServerNumber == 0 )
	{
		world = new GameWorld(true);
		_beginthread(ServerReceiveMessage, 0, NULL);

	}
	else
	{
		world = new GameWorld(false);
		_beginthread(ClientReceiveMessage, 0, NULL);
	}
#else
	world = new GameWorld(true);
#endif


	glutInit( &argc, argv );
	glutInitWindowSize( RenderSystem::getWidth(), RenderSystem::getHeight() );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH ); // stereo display mode for glut
	glutCreateWindow( "Game Test" );

	glutReshapeFunc( reshape );
	glutDisplayFunc( display );
//	glutTimerFunc( ANI_MSEL, timer, 0 );
	glutKeyboardFunc( keyboard );
	glutKeyboardUpFunc(keyboard1Up);
	glutSpecialFunc( keyboard2 );
	glutSpecialUpFunc( keyboard2Up );

	game = new LogicGame( world );



	pServer = world->getPlayer(0);
	pClient = world->getPlayer(1);

	glutMainLoop();

	delete game;
	delete world;
}
