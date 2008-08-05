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

	//���ý�ɫģ�Ͷ���������Ļص�����
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
////��Ϸ�߼�����ʱ��Ƭ��ˢ��
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
///TODO:�Ľ����
///ͨ����TIMER���治�ϵĲ��Խ�ɫ��ǰ����Ϸ״̬�Ƿ��Ѿ����
///������Ϸ��ɫ�Ŀ���״̬
//////////////////////////////////////////////////////////////////////////
void LogicGame::DetectAnimationEnd()
{
	//�ȴ�һ��������������Ϊ����״̬
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
//��Ϸ��ɫ֮�����ײ��⣬ʵ����������������֣�
//��ײ��⣬�ͽ�ɫģ����Ϣ�ĸ���
//////////////////////////////////////////////////////////////////////////
void LogicGame::CollideDetection( float dt )
{
	int ani = 1;
	float speed = 2.0f;
	Player *pi, *pj;
	for( int i = 0; i < world->getPlayerNum(); ++ i )
	{
		pi = world->getPlayer(i);
		for ( int j = 0; j < world->getPlayerNum(); ++ j )	//ģ��AABB��ײ���
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
			//�������ײ����ô������Ϸ�������ö���
			pj->currentCommand->TargetPlayer = pi;
		}

		//////////////////////////////////////////////////////////////////////////
		//ani�����˶�������,speed�������ٶ�
		ani = pi->AnimationSequence;
		speed = pi->AnimationSpeed;
		
		//����״̬���½�ɫģ�͵���Щ����
		world->getModel(i)->SetSequence( ani );
		world->getModel(i)->Update( dt, speed );

		pi->setSpeedHor( Vector2D() );
		//�þ�ֻ���ý�ɫ���ٶ�Ϊ0 Ҳ����ɫ��������ʱ�򲻻ᷢ��λ��
		//if ( world->getModel(i)->IsAniEnd() )
		//{
		//	pi->AnimationSequence = 1;
		//	pi->AnimationSpeed = 2.0f;
		//	pi->isForceAnimation = false;
		//}
	}
}

//////////////////////////////////////////////////////////////////////////
////ȡ��������Ϸ����������е����ִ�У���ϸע��ͬUPDATE_NET)
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
			//����������Ŀǰ����ǿ�ƶ���״̬����ô���Ե�ǰ����
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
////ȡ��Զ�̻������͹��������ִ��
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
			//����������Ŀǰ����ǿ�ƶ���״̬����ô���Ե�ǰ����
			pComQ->pop();
			continue;
		}
		//��Ϸ�����߻�õ�ǰ����
		pCom->ActivePlayer->currentCommand = pCom;
		//��ײ���
		CollideDetection(dt);
		//ִ�е�ǰ����
		pCom->execute();
		//����ִ����ϵ�����
		pComQ->pop();
	}

}

