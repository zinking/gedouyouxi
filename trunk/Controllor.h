#pragma once
#include "GameWorld.h"

class Controllor
{
public:
	Controllor(void);
	Controllor(GameWorld* w);
	void update(void);
	void updateMain(void);
	void updateNet(void);
	void ClientUpdate(void);
	void updateClient(void);
	~Controllor(void);
	GameWorld *world;
};
