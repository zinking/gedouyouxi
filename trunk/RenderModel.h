#pragma once

#include "StudioModel.h"

class RenderModel
{
	StudioModel model;	//Ä£ÐÍ

public:
	RenderModel();
	~RenderModel();
	bool LoadModel( const char* filename );
	float CalcStature();
	bool AABBTest( RenderModel& m, float v1, float v2, float v3 );
	int GetSequence();
	int GetFrame();
	void SetSequence( int s, int frame = 0 );
	bool IsAniEnd();
	void Update( float dt, float speed = 1.0f );
	void Draw();
	StudioModel* getStudioModel();
};
