#include "CollisionObject.h"

CollisionObject::CollisionObject()
{
}

CollisionObject::~CollisionObject()
{
}

void CollisionObject::SetupCollisionPolygen( float rad )
{
	float dx, dy, tmp;

	for ( int i = 0; i < 4; ++ i )
	{
		edge[i * 2].x = vertex[i].x;
		edge[i * 2].y = vertex[i].y;
		edge[i * 2 + 1].x = vertex[(i + 1) % 4].x;
		edge[i * 2 + 1].y = vertex[(i + 1) % 4].y;

		//������
		dx = edge[i * 2].x - edge[i * 2 + 1].x;
		dy = edge[i * 2].y - edge[i * 2 + 1].y;
		tmp = sqrt( dx * dx + dy * dy );
		dx /= tmp;
		dy /= tmp;

		tmp = -dx;	//��ֱ����������һ��ʼ����
		dx = dy;
		dy = tmp;
		normal[i * 2].x = dx;
		normal[i * 2].y = dy;
		//�������������


		//��չ�ߣ�������������Ӧ����rad
		dx *= rad;
		dy *= rad;

		//�ڷ��������������ߵ�ƽ�Ƶõ�������
		edge[i * 2].x += dx;
		edge[i * 2].y += dy;
		edge[i * 2 + 1].x += dx;
		edge[i * 2 + 1].y += dy;
	}
	for ( int i = 0; i < 4; ++ i )
	{
		//�����߷�����
		dx = edge[i * 2 + 1].y - edge[(i * 2 + 2) % 8].y;
		dy = - ( edge[i * 2 + 1].x - edge[(i * 2 + 2) % 8].x );
		tmp = sqrt( dx * dx + dy * dy );
		normal[i * 2 + 1].x = dx / tmp;
		normal[i * 2 + 1].y = dy / tmp;
	}
	height = 0;
	for ( int i = 0; i < 4; ++ i )
	{
		height += vertex[i].z;
	}
	height /= 4;
}