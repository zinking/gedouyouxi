#pragma once

#include "Toolkit.h"

class CollisionObject
{
public:
	Vector3D vertex[4];
	Vector2D edge[8];
	Vector2D normal[8];
	float height;

	CollisionObject();
	~CollisionObject();
	void SetupCollisionPolygen( float rad );
};
