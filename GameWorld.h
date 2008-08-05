#pragma once

#include "Player.h"
#include "RenderModel.h"
#include "FixedCamera.h"
#include "Client.h"
#include "Server.h"
#include "Toolkit.h"
#include <queue>
#include <vector>
#include <map>

using namespace std;

typedef CommandQ CommandQueue;

class GameWorld
{
	//游戏的玩家
	vector<Player* > players;
	//玩家对应的模型
	vector<RenderModel* > models;
	
	//存放自身发起的命令信息
	CommandQueue* MainCommandQueue;
	//存放了对方发来的 命令信息
	CommandQueue* NetCommandQueue;

	Server* server;
	Client* client;

	FixedCamera cam;

	bool ServerOrClient;

	//控制队列同步的简单的同步锁
	bool queue_pop_lock;
	bool queue_front_lock;//为什么要有这两个锁

public:
	GameWorld();
	~GameWorld();

	int getPlayerNum();
	void setPlayerNum( int n );
	Player* getPlayer( int n );
	RenderModel* getModel( int n );
	bool LoadModel( int i, const char* filename );

	CommandQueue* getMainCommandQueue();//获得指向队列的指针
	CommandQueue* getNetCommandQueue();
	queue<DataPackage*>* getDataPackageQueue(void);
	void InitAsServer(void);
	void InitAsClient(void);//初始化游戏端
	void lock_pop(void);
	void unlock_pop(void);//BOOL型锁的开关
	bool getLock_pop(void);//BOOL型锁的开关
	void lock_front(void);
	void unlock_front(void);
	bool getLock_front(void);

	FixedCamera* getCamera();
public:
	GameWorld(bool server_or_client);
	bool IsServer(void);
	void setServer(bool s);
	Client* getClient(void);
	Server* getServer(void);
};
