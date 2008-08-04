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

	Vector2D expect = d;	//期望位移
	Vector2D available = d;	//有效位移
	Vector2D slide = d;	//滑动方向
	Vector2D normal;	//碰撞法向量
	int repeat = 5;	//迭代上限

	do
	{
		for ( int i = 0; i < objects.size(); ++ i )	//场景碰撞检测
		{
			CD_Hor_Obejct( p, &objects[i], available, normal, slide );
		}
		available += normal * MARGIN;	//避免穿透

		for ( int i = 0; i < world->getPlayerNum(); ++ i )	//角色碰撞检测
		{
			CD_Hor_Player( p, world->getPlayer(i), available );
		}

		p->setPosition( p->getPosition() + available );	//应用位移
		expect = expect - available;	//剩余期望位移
		-- repeat;

		if ( expect.Length() > CHECK_MARGIN )	//迭代准备
		{
			slide.Normalize();
			available = expect = slide * expect.Dot( slide );	//滑动
		}
		else break;
	}
	while ( repeat > 0 );
}
//------------------------------------------------------------

void CollisionSystem::CD_Hor_Obejct( Player* p, CollisionObject* o, Vector2D& d, Vector2D& n, Vector2D& slide )
{
	if ( p->getHeight() - o->height > -CLIMBABLE_HEIGHT )	//角色比物体高或在可攀登高度范围内
		return;

	for ( int i = 0; i < 8; ++ i )
	{
		float dis = - o->normal[i].Dot( d );	//期望运动的法向距离

		if ( dis < ZERO )	//平行或反向
			continue;

		float l = o->normal[i].Dot( p->getPosition() - o->edge[i] );	//起点到边距离

		if ( l < -ZERO )	//起点在背面
			continue;

		if ( l - dis > ZERO )	//运动距离不够
			continue;

		Vector2D dEdge = o->edge[i] - o->edge[(i + 1) % 8];
		float c1 = dEdge.Dot( p->getPosition() - o->edge[i] );
		float c2 = dEdge.Dot( p->getPosition() - o->edge[(i + 1) % 8] );

		if ( c1 * c2 > ZERO )	//交点不在边上
			continue;

		d *= l / dis;	//改写有效运动距离
		n = o->normal[i];	//改写碰撞法向量
		slide = dEdge;	//改写滑动方向
	}
}
//------------------------------------------------------------

void CollisionSystem::CD_Hor_Player( Player* p, Player* p2, Vector2D& d )
{
	if ( p == p2 )
		return;

	p->setCollPlayer( NULL );
	p2->setCollPlayer( NULL );

	if ( p->getHeight() - p2->getHeight() - p2->getStature() > CHECK_MARGIN )	//角色高度差大于身高
		return;

	if ( p2->getHeight() - p->getHeight() - p->getStature() > CHECK_MARGIN )	//角色高度差大于身高
		return;

	Vector2D v = p2->getPosition() - p->getPosition();	//圆心连线向量
	float l2 = d.Dot( v );	//运动方向投影

	if ( l2 < ZERO )	//反向运动
		return;

	float l = v.Length();
	float dis2 = l * l - l2 * l2;	//圆心2与运动线段距离

	if ( sqrt( dis2 ) - roll_rad * 2 > -ZERO )	//圆心2到运动线段距离大于半径和
		return;

	float a = sqrt( l * l - dis2 ) - sqrt ( roll_rad * roll_rad * 4 - dis2 );	//交点与圆心1距离
	a = ( a - MARGIN ) / d.Length();

	if ( a > 1 )	//运动距离未到相交点
		return;

	p->setCollPlayer( p2 );
	p2->setCollPlayer( p );
	d *= a;
}
//------------------------------------------------------------

void CollisionSystem::CD_Ver( Player* p, float dz )	//垂直碰撞检测
{
	float h = 0;	//所在位置最高物体的高度
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

	if ( p->getHeight() + dz - h <= CHECK_MARGIN )	//着地
	{
		p->setHeight( h + MARGIN );
		p->setSpeedVer( 0 );
		p->setStateVer( STATEV_STAND );
	}
	else	//坠落
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
