#include "LogicGame.h"

LogicGame::LogicGame( GameWorld* w )
{
	world = w;
	render = new RenderSystem( w );
	physics = new CollisionSystem( w );
	controllor = new Controllor( w );
}

LogicGame::~LogicGame()
{
	if ( render != NULL )		delete render;
	if ( physics != NULL )		delete physics;
	if ( controllor != NULL )		delete controllor;
}

GameWorld* LogicGame::getWorld()
{
	return world;
}

RenderSystem* LogicGame::getRenderSys()
{
	return render;
}

CollisionSystem* LogicGame::getPhySys()
{
	return physics;
}

Controllor* LogicGame::getControllor()
{
	return controllor;
}



void  LogicGame::UpdateGame( float dt )
{
	//------------------------------------------------------------
	//更改状态，ani代表了动作序列,speed代表动作速度，dt代表走路跳跃跨度
	UpdateMain( dt );
	UpdateNet( dt );
	CollideDetection( dt );
	DetectAnimationEnd();
	this->getPhySys()->Update( dt );

}

void LogicGame::DetectAnimationEnd()
{
	//等待一个动作做完设置为闲置状态
	Player* pi;
	for( int i = 0; i < world->getPlayerNum(); ++ i )
	{
		pi = world->getPlayer(i);
		pi->setSpeedHor( Vector2D() );
		if ( world->getModel(i)->IsAniEnd() )
		{
			pi->AnimationSequence = 1;
			pi->AnimationSpeed = 2.0f;
			pi->isForceAnimation = false;
		}
	}
}

void LogicGame::CollideDetection( float dt )
{
	int ani = 1;
	float speed = 2.0f;
	Player *pi, *pj;
	for( int i = 0; i < world->getPlayerNum(); ++ i )
	{
		pi = world->getPlayer(i);
		for ( int j = 0; j < world->getPlayerNum(); ++ j )	//模型AABB碰撞检测
		{
			if ( i == j ) continue;
			pj = world->getPlayer(j);
			Vector2D v = pj->getPosition() - pi->getPosition();
			if (  world->getModel(i)->AABBTest( *world->getModel(j), v.x, 
				v.y, pj->getHeight() - pi->getHeight() ) )
			{
				pi->setCollPlayer( pj );
				pj->setCollPlayer( pi );
			}
		}
		pj = pi->getCollPlayer();

		if (  pj->currentCommand != NULL )
		{			
			pj->currentCommand->TargetPlayer = pi;
		}

		ani = pi->AnimationSequence;
		speed = pi->AnimationSpeed;

		world->getModel(i)->SetSequence( ani );//根据状态跟新角色模型的这些数据
		world->getModel(i)->Update( dt, speed );

		pi->setSpeedHor( Vector2D() );
		if ( world->getModel(i)->IsAniEnd() )
		{
			pi->AnimationSequence = 1;
			pi->AnimationSpeed = 2.0f;
			pi->isForceAnimation = false;
		}
	}
}


void LogicGame::UpdateMain( float dt )
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
		pCom->ActivePlayer->currentCommand = pCom;
		CollideDetection(dt);
		pCom->execute();

		pComQ->pop();
	}
}


void LogicGame::UpdateNet( float dt ) //将NET COMMAND 中的命令执行完毕 针对PLAYER 1
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
		pCom->ActivePlayer->currentCommand = pCom;
		CollideDetection(dt);
		pCom->execute();

		pComQ->pop();
	}

}

