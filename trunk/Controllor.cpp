#include "Controllor.h"
#include <iostream>
#include <process.h>

using namespace std;

Controllor::Controllor(void)
{
}

Controllor::~Controllor(void)
{
}

Controllor::Controllor(GameWorld* w)
{
	this->world = w;
}

void Controllor::update(void)
{
//	if(/*world->IsServer()*/true)
//	{
		updateMain();
		updateNet();
//	}
	//else
	//{
	//	ClientUpdate();
	//}
}

//���ƶ����ķ���

void Controllor::updateMain(void)
{
	Command* pCom = NULL;
	CommandQ* pComQ = world->getMainCommandQueue();
	while( ! pComQ->empty() )//����������е���������ִ����� ���PLAYER 0
	{//���UpdateMain��������Ҫ��Commandģʽ�޸�
		pCom = pComQ->front();
		if ( pCom->ActivePlayer->isForceAni() )
		{
			pComQ->pop();
			continue;
		}
		pCom->execute();
		
		pComQ->pop();
	}
}


void Controllor::updateNet(void) //��NET COMMAND �е�����ִ����� ���PLAYER 1
{
	Command* pCom = NULL;
	CommandQ* pComQ = world->getNetCommandQueue();
	while( ! pComQ->empty() )//����������е���������ִ����� ���PLAYER 0
	{//���UpdateMain��������Ҫ��Commandģʽ�޸�
		pCom = pComQ->front();
		if ( pCom->ActivePlayer->isForceAni() )
		{
			pComQ->pop();
			continue;
		}
		pCom->execute();

		pComQ->pop();
	}


	
}

