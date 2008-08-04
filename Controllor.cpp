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

//控制动作的方法

void Controllor::updateMain(void)
{
	Command* pCom = NULL;
	CommandQ* pComQ = world->getMainCommandQueue();
	while( ! pComQ->empty() )//将命令队列中的所有命令执行完毕 针对PLAYER 0
	{//这个UpdateMain方法绝对要用Command模式修改
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


void Controllor::updateNet(void) //将NET COMMAND 中的命令执行完毕 针对PLAYER 1
{
	Command* pCom = NULL;
	CommandQ* pComQ = world->getNetCommandQueue();
	while( ! pComQ->empty() )//将命令队列中的所有命令执行完毕 针对PLAYER 0
	{//这个UpdateMain方法绝对要用Command模式修改
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

