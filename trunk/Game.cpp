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

const int ANI_MSEL = 10;	//一帧动画的毫秒数

LogicGame* game;
GameWorld* world;
Player* pServer;
Player* pClient;

int ServerNumber;

//Frames per second counter
FPS_COUNTER fpsCounter;

//针对动作的判断
bool idle = true;
bool run = false;
bool runback = false;
bool jump = false;
bool attack = false;//我靠全部耦合起来了，ANIMATION放在全局了，这怎么做
bool state_left = false;
bool state_right = false;

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
	showFPS();

	glFlush();
	glutSwapBuffers();

}
//------------------------------------------------------------

void timer( int p )
{
	time_count++;
	float dt = (float) ANI_MSEL / 1000;

	game->UpdateGame( dt );

	glutPostRedisplay();
	glutTimerFunc( ANI_MSEL, timer, 0 );
}
//------------------------------------------------------------
void keyboard( unsigned char key, int x, int y )
{
	//char* jump = "JUMP";
	//char* exit_flag = "EXIT";
//	if(!attack){
		if(game->getWorld()->IsServer())
		{
			switch ( key )
			{
				case 27:
					world->getServer()->write( CONVERT( 1000) );//
					exit(0);
					break;
				case 32:
					cout << "jump" << endl;
					//world->getMainCommandQueue()->push(jump);
					world->getMainCommandQueue()->push(  GETCOM(JUMP,pServer)  );
					world->getServer()->write(  CONVERT( JUMP) );
					break;
				case 'a':
					cout << "weakfight" << endl;
					//world->getMainCommandQueue()->push("WEAKFIGHT");
					world->getMainCommandQueue()->push( GETCOM( WEAKFIGHT,pServer ) );
					world->getServer()->write( CONVERT( WEAKFIGHT) );
					break;
				case 's':
					cout << "strongfight" << endl;
					//world->getMainCommandQueue()->push("STRONGFIGHT");
					world->getMainCommandQueue()->push(  GETCOM(  STRONGFIGHT,pServer ) );
					world->getServer()->write( CONVERT( STRONGFIGHT ) );
					break;
				case 'd':
					cout << "qigong" << endl;
					//world->getMainCommandQueue()->push("QIGONG");
					world->getMainCommandQueue()->push( GETCOM( QIGONG,pServer ) );
					world->getServer()->write( CONVERT( QIGONG ) );
					break;
				case 'z':
					cout << "weakfoot" << endl;
					//world->getMainCommandQueue()->push("WEAKFOOT");
					world->getMainCommandQueue()->push(  GETCOM(  WEAKFOOT,pServer ) );
					world->getServer()->write( CONVERT( WEAKFOOT ) );
					break;
				case 'x':
					cout << "strongfoot" << endl;
					//world->getMainCommandQueue()->push("STRONGFOOT");
					world->getMainCommandQueue()->push(  GETCOM(  STRONGFOOT,pServer  ) );
					world->getServer()->write(  CONVERT( STRONGFOOT ) );
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
					world->getMainCommandQueue()->push(  GETCOM(JUMP,pClient) );
					world->getClient()->write(  CONVERT( JUMP) );
					break;
				case 'a':
					cout << "weakfight" << endl;
					world->getMainCommandQueue()->push( GETCOM(WEAKFIGHT,pClient) );
					world->getClient()->write( CONVERT(  WEAKFIGHT )  );
					break;
				case 's':
					cout << "strongfight" << endl;
					world->getMainCommandQueue()->push( GETCOM(STRONGFIGHT,pClient) );
					world->getClient()->write( CONVERT( STRONGFIGHT) );
					break;
				case 'd':
					cout << "qigong" << endl;
					world->getClient()->write( CONVERT( QIGONG )  );
					world->getMainCommandQueue()->push( GETCOM( QIGONG,pClient ) );
					break;
				case 'z':
					cout << "weakfoot" << endl;
					world->getMainCommandQueue()->push( GETCOM( WEAKFOOT,pClient) );
					world->getClient()->write( CONVERT( WEAKFOOT) );
					break;
				case 'x':
					cout << "strongfoot" << endl;
					world->getClient()->write(  CONVERT( STRONGFOOT ) );
					world->getMainCommandQueue()->push( GETCOM(STRONGFOOT,pClient) );
					break;

			}
		}
	//	attack = true;
	//}
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
					run = true;	
					world->getServer()->write(  CONVERT( RUN ) );
					world->getMainCommandQueue()->push(  GETCOM(RUN,pServer) );
					break;
				case GLUT_KEY_DOWN:
					cout << "down" << endl;
					runback = true;
					world->getMainCommandQueue()->push(  GETCOM( RUNBACK,pServer)  );
					world->getServer()->write(  CONVERT( RUNBACK ) );
					break;
				case GLUT_KEY_LEFT:
					cout << "left" << endl;
					state_left = true;
					world->getServer()->write(  CONVERT( LEFT ) );
					world->getMainCommandQueue()->push(  GETCOM(LEFT,pServer)  );
					break;
				case GLUT_KEY_RIGHT:
					cout << "right" << endl;
					state_right = true;
					world->getMainCommandQueue()->push(  GETCOM(RIGHT,pServer)  );
					world->getServer()->write(  CONVERT( RIGHT ) );
					break;
			}
		} 
		else
		{
			switch ( key )
			{
				case GLUT_KEY_UP:
					cout << "up" << endl;
					run = true;	
					world->getClient()->write(  CONVERT( RUN ) );
					world->getMainCommandQueue()->push(  GETCOM(RUN,pClient)  );
					break;
				case GLUT_KEY_DOWN:
					cout << "down" << endl;
					runback = true;
					world->getMainCommandQueue()->push(  GETCOM(RUNBACK,pClient)  );
					world->getClient()->write(  CONVERT( RUNBACK ) );
					break;
				case GLUT_KEY_LEFT:
					cout << "left" << endl;
					state_left = true;
					world->getClient()->write(  CONVERT( LEFT ) );
					world->getMainCommandQueue()->push(  GETCOM(LEFT,pClient) );
					break;
				case GLUT_KEY_RIGHT:
					cout << "right" << endl;
					state_right = true;
					world->getClient()->write(  CONVERT( RIGHT ) );
					world->getMainCommandQueue()->push(  GETCOM( RIGHT,pClient)  );
					break;
			}
		}

}

//------------------------------------------------------------
void keyboard1Up( unsigned char key, int x, int y )
{
	attack = false;
}

//------------------------------------------------------------
void keyboard2Up( int key, int x, int y )
{
	switch ( key )
	{
		case GLUT_KEY_UP:
			run = false;
			cout << "up_up" << endl;
			break;
		case GLUT_KEY_DOWN:
			runback = false;
			cout << "down_up" << endl;
			break;
		case GLUT_KEY_LEFT:
			state_left = false;
			cout << "left_up" << endl;
			break;
		case GLUT_KEY_RIGHT:
			state_right = false;
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

	glutInit( &argc, argv );
	glutInitWindowSize( RenderSystem::getWidth(), RenderSystem::getHeight() );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH ); // stereo display mode for glut
	glutCreateWindow( "Game Test" );

	glutReshapeFunc( reshape );
	glutDisplayFunc( display );
	glutTimerFunc( ANI_MSEL, timer, 0 );
	glutKeyboardFunc( keyboard );
	glutKeyboardUpFunc(keyboard1Up);
	glutSpecialFunc( keyboard2 );
	glutSpecialUpFunc( keyboard2Up );

	game = new LogicGame( world );

	if ( !game->getPhySys()->LoadScene( "2.s" ) )
		return;
	if ( !game->getRenderSys()->LoadScene( "1.m" ) )
		return;
	game->getWorld()->setPlayerNum( 2 );
	if ( !game->getWorld()->LoadModel( 0, "goku.mdl" ) )
		return;
	if ( !game->getWorld()->LoadModel( 1, "goku.mdl" ) )
		return;

	pServer = world->getPlayer(0);
	pClient = world->getPlayer(1);

	glutMainLoop();

	delete game;
	delete world;
}
