#include "CollisionSystem.h"

//------------------------------------------------------------

CollisionSystem::CollisionSystem( GameWorld* w, float rad )
{
	world = w;
	roll_rad = rad;
}
//------------------------------------------------------------

CollisionSystem::~CollisionSystem()
{
}
//------------------------------------------------------------

bool CollisionSystem::LoadScene( const char* filename )
{
	char oneline[255];
	int num;

	FILE* filein = fopen( filename, "rt" );
	if ( filein == NULL )
		return false;

	ReadStr( filein, oneline );
	sscanf( oneline, "%d\n", &num );
	objects.resize( num );

	for ( int i = 0; i < num; ++ i )
	{
		for ( int j = 0; j < 4; ++ j )
		{
			Vector3D& v = objects[i].vertex[j];

			ReadStr( filein, oneline );
			sscanf( oneline, "%f %f %f", &v.x, &v.y, &v.z );
		}
		objects[i].SetupCollisionPolygen( roll_rad );
	}

	return true;
}
//------------------------------------------------------------

void CollisionSystem::CD_Hor( Player* p, const Vector2D& d )
{
	if ( d.Length() < CHECK_MARGIN )
		return;

	Vector2D expect = d;	//����λ��
	Vector2D available = d;	//��Чλ��
	Vector2D slide = d;	//��������
	Vector2D normal;	//��ײ������
	int repeat = 5;	//��������

	do
	{
		for ( int i = 0; i < objects.size(); ++ i )	//������ײ���
		{
			CD_Hor_Obejct( p, &objects[i], available, normal, slide );
		}
		available += normal * MARGIN;	//���⴩͸

		for ( int i = 0; i < world->getPlayerNum(); ++ i )	//��ɫ��ײ���
		{
			CD_Hor_Player( p, world->getPlayer(i), available );
		}

		p->setPosition( p->getPosition() + available );	//Ӧ��λ��
		expect = expect - available;	//ʣ������λ��
		-- repeat;

		if ( expect.Length() > CHECK_MARGIN )	//����׼��
		{
			slide.Normalize();
			available = expect = slide * expect.Dot( slide );	//����
		}
		else break;
	}
	while ( repeat > 0 );
}
//------------------------------------------------------------

void CollisionSystem::CD_Hor_Obejct( Player* p, CollisionObject* o, Vector2D& d, Vector2D& n, Vector2D& slide )
{
	if ( p->getHeight() - o->height > -CLIMBABLE_HEIGHT )	//��ɫ������߻��ڿ��ʵǸ߶ȷ�Χ��
		return;

	for ( int i = 0; i < 8; ++ i )
	{
		float dis = - o->normal[i].Dot( d );	//�����˶��ķ������

		if ( dis < ZERO )	//ƽ�л���
			continue;

		float l = o->normal[i].Dot( p->getPosition() - o->edge[i] );	//��㵽�߾���

		if ( l < -ZERO )	//����ڱ���
			continue;

		if ( l - dis > ZERO )	//�˶����벻��
			continue;

		Vector2D dEdge = o->edge[i] - o->edge[(i + 1) % 8];
		float c1 = dEdge.Dot( p->getPosition() - o->edge[i] );
		float c2 = dEdge.Dot( p->getPosition() - o->edge[(i + 1) % 8] );

		if ( c1 * c2 > ZERO )	//���㲻�ڱ���
			continue;

		d *= l / dis;	//��д��Ч�˶�����
		n = o->normal[i];	//��д��ײ������
		slide = dEdge;	//��д��������
	}
}
//------------------------------------------------------------

void CollisionSystem::CD_Hor_Player( Player* p, Player* p2, Vector2D& d )
{
	if ( p == p2 )
		return;

	p->setCollPlayer( NULL );
	p2->setCollPlayer( NULL );

	if ( p->getHeight() - p2->getHeight() - p2->getStature() > CHECK_MARGIN )	//��ɫ�߶Ȳ�������
		return;

	if ( p2->getHeight() - p->getHeight() - p->getStature() > CHECK_MARGIN )	//��ɫ�߶Ȳ�������
		return;

	Vector2D v = p2->getPosition() - p->getPosition();	//Բ����������
	float l2 = d.Dot( v );	//�˶�����ͶӰ

	if ( l2 < ZERO )	//�����˶�
		return;

	float l = v.Length();
	float dis2 = l * l - l2 * l2;	//Բ��2���˶��߶ξ���

	if ( sqrt( dis2 ) - roll_rad * 2 > -ZERO )	//Բ��2���˶��߶ξ�����ڰ뾶��
		return;

	float a = sqrt( l * l - dis2 ) - sqrt ( roll_rad * roll_rad * 4 - dis2 );	//������Բ��1����
	a = ( a - MARGIN ) / d.Length();

	if ( a > 1 )	//�˶�����δ���ཻ��
		return;

	p->setCollPlayer( p2 );
	p2->setCollPlayer( p );
	d *= a;
}
//------------------------------------------------------------

void CollisionSystem::CD_Ver( Player* p, float dz )	//��ֱ��ײ���
{
	float h = 0;	//����λ���������ĸ߶�
	bool inside;

	for ( int i = 0; i < objects.size(); ++ i )
	{
		inside = true;
		for ( int j = 0; j < 8; ++ j )
		{
			if ( objects[i].normal[j].Dot( p->getPosition() - objects[i].edge[j] ) > ZERO )
			{
				inside = false;
				break;
			}
		}
		if ( inside && objects[i].height > h )
			h = objects[i].height;
	}

	for ( int i = 0; i < world->getPlayerNum(); ++ i )
	{
		Player* p2 = world->getPlayer(i);

		if ( p != p2 &&
			p->getHeight() > p2->getHeight() &&
			( p->getPosition() - p2->getPosition() ).Length() < roll_rad * 2 )
		{
			if ( p2->getHeight() + p2->getStature() > h )
				h = p2->getHeight() + p2->getStature();
		}
	}

	if ( p->getHeight() + dz - h <= CHECK_MARGIN )	//�ŵ�
	{
		p->setHeight( h + MARGIN );
		p->setSpeedVer( 0 );
		p->setStateVer( STATEV_STAND );
	}
	else	//׹��
	{
		p->setHeight( p->getHeight() + dz );
		p->setStateVer( STATEH_JUMP );
	}
}
//------------------------------------------------------------

void CollisionSystem::Update( float dt )
{
	Player* p;

	for ( int i = 0; i < world->getPlayerNum(); ++ i )
	{
		p = world->getPlayer(i);
		CD_Hor( p, p->getSpeedHor() * dt );
		CD_Ver( p, p->getSpeedVer() * dt );
		if ( p->getStateVer() == STATEH_JUMP )
		{
			p->setSpeedVer( p->getSpeedVer() - 3.0f );
		}
	}
}
