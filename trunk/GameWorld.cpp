#include "GameWorld.h"


// Ĭ�Ϲ��캯����ʵ�ʲ�δʹ��

GameWorld::GameWorld()
{
	MainCommandQueue = new CommandQueue();
	NetCommandQueue = new CommandQueue();

}

GameWorld::GameWorld(bool server_or_client)
{
	MainCommandQueue = new CommandQueue();
	NetCommandQueue = new CommandQueue();

	ServerOrClient = server_or_client;

	//��ʼ�����й�����
	this->queue_front_lock = false;
	this->queue_pop_lock = false;
	
	//�жϿͻ��˻��������Ȼ���ʼ��
#ifndef STANDALONE_WITHOUT_NETWORK
	if(server_or_client)
	{
		this->InitAsServer();
	}
	else
	{
		this->InitAsClient();
	}
#endif

}

GameWorld::~GameWorld()
{
	if ( MainCommandQueue ) delete MainCommandQueue;
	if ( NetCommandQueue )   delete NetCommandQueue;

	if ( server ) delete server;
	if ( client ) delete client;

	Player * p = NULL;
	RenderModel* pRM= NULL;
	for ( int i=0;i < players.size(); i++ )
	{
		p = players[i];
		pRM = models[i];
		if ( p ) delete p;
		if ( pRM ) delete pRM;
	}
}

int GameWorld::getPlayerNum()
{
	return players.size();
}

//��Ϸ������ģ�ͺ�Player�ĳ�ʼ��
//ͨ��VECTOR Ĭ�Ϲ������
void GameWorld::setPlayerNum( int n )
{
	players.resize( n );
	models.resize( n );

	for ( int i=0;i<n;i++ )
	{
		players[i] = new Player();
		models[i] = new RenderModel();
	}
}

Player* GameWorld::getPlayer( int n )
{
	return players[n];
}

RenderModel* GameWorld::getModel( int n )
{
	return models[n];
}


//TODO:ģ���ļ��ļ��أ���Ϊ���ļ�������ʱ��Ҳû�и�����ʾ��Ϣ
bool GameWorld::LoadModel( int i, const char* filename )
{
	bool b = models[i]->LoadModel( filename );
	players[i]->setStature( models[i]->CalcStature() );
	return b;
}

FixedCamera* GameWorld::getCamera()
{
	return &cam;
}

CommandQueue* GameWorld::getMainCommandQueue()
{
	return MainCommandQueue;
}

CommandQueue* GameWorld::getNetCommandQueue()
{
	return NetCommandQueue;
}


bool GameWorld::IsServer(void)
{
	return ServerOrClient;
}

void GameWorld::setServer(bool s)
{
	ServerOrClient = s;
}

Server* GameWorld::getServer(void)
{
	return this->server;
}

Client* GameWorld::getClient(void)
{
	return this->client;
}

//////////////////////////////////////////////////////////////////////////
/////GameWorld�е���������Ǵ���Ǳ�ڵ�ͬ�����������
//////////////////////////////////////////////////////////////////////////
void GameWorld::lock_pop(void)
{
	this->queue_pop_lock = true;
}

void GameWorld::unlock_pop(void)
{
	this->queue_pop_lock = false;
}

bool GameWorld::getLock_pop(void)
{
	return this->queue_pop_lock;
}

void GameWorld::lock_front(void)
{
	this->queue_front_lock = true;
}

void GameWorld::unlock_front(void)
{
	this->queue_front_lock = false;
}

bool GameWorld::getLock_front(void)
{
	return this->queue_front_lock;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
////��ʼ������˺Ϳͻ���
//////////////////////////////////////////////////////////////////////////
void GameWorld::InitAsServer(void)
{
	server = new Server(1001);
	//�����������Ҫ��Ļ���������Լ�����

}

void GameWorld::InitAsClient(void)
{
	client = new Client("127.0.0.1", 1001);
	//�����������Ҫ��Ļ���������Լ�����

	//client = new Client("192.168.1.101", 1001);
	//client = new Client("192.168.18.123", 1001);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

