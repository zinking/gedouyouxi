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
	vector<Player> players;//�������֧�ֶ���ҵ�
	vector<RenderModel> models;//֧�ֶ���Ⱦģ��
	map<Player*,int> IndexTable;

	CommandQueue* MainCommandQueue;
	CommandQueue* NetCommandQueue;//����˿ͻ��˷���������������������

	Server* server;
	Client* client;

	FixedCamera cam;

	bool ServerOrClient;
	bool queue_pop_lock;
	bool queue_front_lock;//ΪʲôҪ����������

public:
	GameWorld();
	~GameWorld();

	int getPlayerNum();
	void setPlayerNum( int n );
	Player* getPlayer( int n );
	RenderModel* getModel( int n );
	bool LoadModel( int i, const char* filename );

	CommandQueue* getMainCommandQueue();//���ָ����е�ָ��
	CommandQueue* getNetCommandQueue();
	queue<DataPackage*>* getDataPackageQueue(void);
	void InitAsServer(void);
	void InitAsClient(void);//��ʼ����Ϸ��
	void lock_pop(void);
	void unlock_pop(void);//BOOL�����Ŀ���
	bool getLock_pop(void);//BOOL�����Ŀ���
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
