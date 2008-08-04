#include "FixedCamera.h"

FixedCamera::FixedCamera()
{
	x = y = -100;
	z = 100;
	dx = dy = 50;
	dz = 0;
	ux = uy = 0;
	uz = 1;
}

FixedCamera::~FixedCamera()
{
}
