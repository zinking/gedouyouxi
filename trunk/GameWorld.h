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
	vector<Player> players;//设计来是支持多玩家的
	vector<RenderModel> models;//支持多渲染模型
	map<Player*,int> IndexTable;

	CommandQueue* MainCommandQueue;
	CommandQueue* NetCommandQueue;//存放了客户端发来的跳，攻击，等命令

	Server* server;
	Client* client;

	FixedCamera cam;

	bool ServerOrClient;
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
