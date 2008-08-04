#pragma once

class FixedCamera
{
public:
	float x, y, z;
	float dx, dy, dz;
	float ux, uy, uz;

	FixedCamera();
	~FixedCamera();
};
