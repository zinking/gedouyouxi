#include "GameWorld.h"


// 默认构造函数，实际并未使用

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

	//初始化队列管理锁
	this->queue_front_lock = false;
	this->queue_pop_lock = false;
	
	//判断客户端或服务器，然后初始化
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

//游戏世界中模型和Player的初始化
//通过VECTOR 默认构造完成
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


//TODO:模型文件的加载，因为当文件不存在时候也没有给出提示信息
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
/////GameWorld中的锁设计来是处理潜在的同步互斥问题的
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
////初始化服务端和客户端
//////////////////////////////////////////////////////////////////////////
void GameWorld::InitAsServer(void)
{
	server = new Server(1001);
	//如果对网络有要求的话，这里可以继续做

}

void GameWorld::InitAsClient(void)
{
	client = new Client("127.0.0.1", 1001);
	//如果对网络有要求的话，这里可以继续做

	//client = new Client("192.168.1.101", 1001);
	//client = new Client("192.168.18.123", 1001);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

