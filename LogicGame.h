#pragma once

#include "GameWorld.h"
#include "CollisionSystem.h"
#include "RenderSystem.h"
#include "Controllor.h"

class LogicGame
{
	GameWorld* world;
	RenderSystem* render;
	CollisionSystem* physics;
	Controllor* controllor;

public:
	LogicGame( GameWorld* w );
	~LogicGame();

	void UpdateGame( float dt );
	void CollideDetection( float dt );
	void UpdateMain( float dt );
	void UpdateNet( float dt );
	void DetectAnimationEnd();

	GameWorld* getWorld();
	RenderSystem* getRenderSys();
	CollisionSystem* getPhySys();
	Controllor* getControllor();
};
