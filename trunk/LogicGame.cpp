#include "LogicGame.h"

void AnimationEndCallback( void* targetPlayer);

LogicGame::LogicGame( GameWorld* w )
{
	world = w;
	render = new RenderSystem( world );
	physics = new CollisionSystem( world );
	controllor = new Controllor( world );

	if ( !physics->LoadScene( "2.s" ) )	return;
	if ( !render->LoadScene( "1.m" ) ) 	return;

	world->setPlayerNum( 2 );

	if ( !world->LoadModel( 0, "goku.mdl" ) ) 	return;
	if ( !world->LoadModel( 1, "goku.mdl" ) ) return;

	//设置角色模型动画结束后的回调函数
	StudioModel* pStu;
	for ( int i=0;i< world->getPlayerNum(); i++)
	{
		pStu = world->getModel(i)->getStudioModel();
		pStu->animationCall = AnimationEndCallback;
		pStu->data = (void*) world->getPlayer(i);

	}
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

//////////////////////////////////////////////////////////////////////////
////游戏逻辑基于时间片的刷新
//////////////////////////////////////////////////////////////////////////
void  LogicGame::UpdateGame( float dt )
{

	UpdateMain( dt );
	UpdateNet( dt );
	getPhySys()->Update( dt );
	CollideDetection( dt );
	//DetectAnimationEnd();


}


//////////////////////////////////////////////////////////////////////////
///TODO:改进这点
///通过在TIMER里面不断的测试角色当前的游戏状态是否已经完成
///设置游戏角色的空闲状态
//////////////////////////////////////////////////////////////////////////
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

void AnimationEndCallback( void* targetPlayer)
{
	Player* player = (Player* ) targetPlayer;

	player->AnimationSequence = 1;
	player->AnimationSpeed = 2.0f;
	player->isForceAnimation = false;

}


//////////////////////////////////////////////////////////////////////////
//游戏角色之间的碰撞检测，实际上耦合了两个部分：
//碰撞检测，和角色模型信息的更新
//////////////////////////////////////////////////////////////////////////
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

		if ( pj  && pj->currentCommand != NULL )
		{
			//如果有碰撞，那么设置游戏命令作用对象
			pj->currentCommand->TargetPlayer = pi;
		}

		//////////////////////////////////////////////////////////////////////////
		//ani代表了动作序列,speed代表动作速度
		ani = pi->AnimationSequence;
		speed = pi->AnimationSpeed;
		
		//根据状态跟新角色模型的这些数据
		world->getModel(i)->SetSequence( ani );
		world->getModel(i)->Update( dt, speed );

		pi->setSpeedHor( Vector2D() );
		//该句只是让角色的速度为0 也即角色做动作的时候不会发生位移
		//if ( world->getModel(i)->IsAniEnd() )
		//{
		//	pi->AnimationSequence = 1;
		//	pi->AnimationSpeed = 2.0f;
		//	pi->isForceAnimation = false;
		//}
	}
}

//////////////////////////////////////////////////////////////////////////
////取出本机游戏的命令队列中的命令并执行（详细注释同UPDATE_NET)
//////////////////////////////////////////////////////////////////////////
void LogicGame::UpdateMain( float dt )
{
	Command* pCom = NULL;
	CommandQ* pComQ = world->getMainCommandQueue();
	while( ! pComQ->empty() )
	{
		pCom = pComQ->front();
		if ( pCom->ActivePlayer->isForceAni() )
		{
			//如果命令发起者目前处于强制动画状态，那么忽略当前命令
			pComQ->pop();
			continue;
		}
		pCom->ActivePlayer->currentCommand = pCom;
		CollideDetection(dt);
		pCom->execute();

		pComQ->pop();
	}
}

//////////////////////////////////////////////////////////////////////////
////取出远程机器发送过来的命令并执行
//////////////////////////////////////////////////////////////////////////
void LogicGame::UpdateNet( float dt ) 
{
	Command* pCom = NULL;
	CommandQ* pComQ = world->getNetCommandQueue();
	while( ! pComQ->empty() )
	{
		pCom = pComQ->front();
		if ( pCom->ActivePlayer->isForceAni() )
		{
			//如果命令发起者目前处于强制动画状态，那么忽略当前命令
			pComQ->pop();
			continue;
		}
		//游戏发起者获得当前命令
		pCom->ActivePlayer->currentCommand = pCom;
		//碰撞检测
		CollideDetection(dt);
		//执行当前命令
		pCom->execute();
		//弹出执行完毕的命令
		pComQ->pop();
	}

}

