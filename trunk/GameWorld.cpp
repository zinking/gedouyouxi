#include "GameWorld.h"

GameWorld::GameWorld()
{
	MainCommandQueue = new CommandQueue();
	NetCommandQueue = new CommandQueue();
//	DataPackQueue = new queue<DataPackage*>();//��ʼ���������У�Ĭ�Ϲ��캯����������û�õģ�
}

GameWorld::GameWorld(bool server_or_client)
{
	MainCommandQueue = new CommandQueue();
	  NetCommandQueue = new CommandQueue();
//	DataPackQueue = new queue<DataPackage*>();
	ServerOrClient = server_or_client;
	this->queue_front_lock = false;
	this->queue_pop_lock = false;//��������
	
	//�жϿͻ��˻��������Ȼ���ʼ��
	if(server_or_client)
	{
		this->InitAsServer();
	}
	else
	{
		this->InitAsClient();
	}

}

GameWorld::~GameWorld()
{
	if ( MainCommandQueue ) delete MainCommandQueue;
	if ( NetCommandQueue )   delete NetCommandQueue;
//	if ( DataPackQueue ) delete DataPackQueue;
	if ( server ) delete server;
	if ( client ) delete client;
}

int GameWorld::getPlayerNum()
{
	return players.size();
}

void GameWorld::setPlayerNum( int n )
{
	players.resize( n );
	models.resize( n );

	//for ( int i=0; i<n; i++ )
	//{
	//	IndexTable[&players[i]] = i;
	//}
}

Player* GameWorld::getPlayer( int n )
{
	return &players[n];
}

RenderModel* GameWorld::getModel( int n )
{
	return &models[n];
}

bool GameWorld::LoadModel( int i, const char* filename )
{
	bool b = models[i].LoadModel( filename );
	players[i].setStature( models[i].CalcStature() );
	return b;//�˴���Ȼ�޳�����
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

void GameWorld::InitAsServer(void)
{
	server = new Server(1001);

//	char* tmp = new char[100];
	
//	cout << "Input the message :" << endl;

}

void GameWorld::InitAsClient(void)
{
	client = new Client("127.0.0.1", 1001);
	//�����������Ҫ��Ļ���������Լ�����

	//client = new Client("192.168.1.101", 1001);
	//client = new Client("192.168.18.123", 1001);

//	char* tmp = new char[100];

//	cout << "Input the message :" << endl;

}

//queue<DataPackage*>* GameWorld::getDataPackageQueue(void)
//{
//	return this->DataPackQueue;
//}

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
