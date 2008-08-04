#pragma once

#include "CollisionObject.h"
#include "GameWorld.h"
#include <vector>

using namespace std;

const float MARGIN = 0.05f;
const float CHECK_MARGIN = 0.1f;
const float CLIMBABLE_HEIGHT = 2.5f;	//可直接跨过的高度

//------------------------------------------------------------
class CollisionSystem
{
	GameWorld* world;

	vector<CollisionObject> objects;

	float roll_rad;	//角色半径

public:
	CollisionSystem( GameWorld* w, float rad = 5.0f );
	~CollisionSystem();

	bool LoadScene( const char* filename );
	void CD_Hor( Player* p, const Vector2D& d );	//水平碰撞检测
	void CD_Hor_Obejct( Player* p, CollisionObject* o, Vector2D& d, Vector2D& n, Vector2D& slide );
	void CD_Hor_Player( Player* p, Player* p2, Vector2D& d );	//角色水平碰撞检测
	void CD_Ver( Player* p, float dz );	//垂直碰撞检测
	void Update( float dt );
};
