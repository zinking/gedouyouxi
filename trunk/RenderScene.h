#pragma once

#include "Toolkit.h"

class RenderScene
{
public:
	int num_vertices;
	Vector3D* vertex;
	Vector3D* normal;
	Vector2D* uv;

	int num_indices;
	unsigned short* index;

	RenderScene();
	~RenderScene();
	void ReleaseAll();
	bool LoadScene( const char* filename );
};
