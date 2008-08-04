#include "RenderScene.h"

#pragma warning(disable:4996)

//------------------------------------------------------------

RenderScene::RenderScene()
{
	num_vertices = 0;
	vertex = NULL;

	num_indices = 0;
	index = NULL;
}
//------------------------------------------------------------

RenderScene::~RenderScene()
{
	ReleaseAll();
}
//------------------------------------------------------------

void RenderScene::ReleaseAll()
{
	if ( num_vertices != 0 )
	{
		num_vertices = 0;
		delete [] vertex;
		delete [] normal;
		delete [] uv;
	}

	if ( num_indices != 0 )
	{
		num_indices = 0;
		delete [] index;	//Ī����������
	}
}
//------------------------------------------------------------

bool RenderScene::LoadScene( const char* filename )
{
	char oneline[255];
	int num1, num2;

	FILE* filein = fopen( filename, "rt" );
	if ( filein == NULL )
		return false;

	//1.m�ĵ�һ�л�ó�����ĸ���Ȼ����������vertex��normal
	ReadStr( filein, oneline );
	sscanf( oneline, "%d %d", &num1, &num2 );

	ReleaseAll();

	num_vertices = num1;
	vertex = new Vector3D[num1];
	normal = new Vector3D[num1];
	uv = new Vector2D[num1];

	num_indices = num2;
	index = new unsigned short[num2];

	for ( int i = 0; i < num1; ++ i )
	{
		//Ϊ������Ч�ʵĸ�ֵ
		Vector3D& v = vertex[i];
		Vector3D& n = normal[i];

		//��ȡÿһ�У��õ���ͷ�������ֵ
		ReadStr( filein, oneline );
		sscanf( oneline, "V %f %f %f %f %f %f %f %f", &v.x, &v.y, &v.z, &n.x, &n.y, &n.z, &uv[i].x, &uv[i].y );
	}

	for ( int i = 0; i < num2; ++ i )
	{
		ReadStr( filein, oneline );
		sscanf( oneline, "I %d", &index[i] );
	}

	return true;
}