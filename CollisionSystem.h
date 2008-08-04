#pragma once

#include "CollisionObject.h"
#include "GameWorld.h"
#include <vector>

using namespace std;

const float MARGIN = 0.05f;
const float CHECK_MARGIN = 0.1f;
const float CLIMBABLE_HEIGHT = 2.5f;	//��ֱ�ӿ���ĸ߶�

//------------------------------------------------------------
class CollisionSystem
{
	GameWorld* world;

	vector<CollisionObject> objects;

	float roll_rad;	//��ɫ�뾶

public:
	CollisionSystem( GameWorld* w, float rad = 5.0f );
	~CollisionSystem();

	bool LoadScene( const char* filename );
	void CD_Hor( Player* p, const Vector2D& d );	//ˮƽ��ײ���
	void CD_Hor_Obejct( Player* p, CollisionObject* o, Vector2D& d, Vector2D& n, Vector2D& slide );
	void CD_Hor_Player( Player* p, Player* p2, Vector2D& d );	//��ɫˮƽ��ײ���
	void CD_Ver( Player* p, float dz );	//��ֱ��ײ���
	void Update( float dt );
};
