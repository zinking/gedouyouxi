#include "RenderModel.h"
#include "ViewerSettings.h"

//------------------------------------------------------------

RenderModel::RenderModel()
{
}
//------------------------------------------------------------

RenderModel::~RenderModel()
{
}
//------------------------------------------------------------

bool RenderModel::LoadModel( const char* filename )
{
	model.FreeModel();
	if ( model.LoadModel( (char*) filename) )
	{
		if ( model.PostLoadModel( (char*) filename) )
		{
			model.SetSkin (0);
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------
//计算高度
float RenderModel::CalcStature()
{
	float min[3], max[3];//含义很不清楚
	model.ExtractBbox (min, max);

	return max[2] - min[2];
}
//------------------------------------------------------------

//需要修改成细节包围盒的检测
bool RenderModel::AABBTest( RenderModel& m, float v1, float v2, float v3 )
{
	float min[3], max[3], min2[3], max2[3];
	model.ExtractBbox (min, max);
	m.model.ExtractBbox (min2, max2);

	min2[0] -= v1;	//世界坐标差
	min2[1] -= v2;
	min2[2] -= v3;
	max2[0] -= v1;
	max2[1] -= v2;
	max2[2] -= v3;

	for ( int i = 0; i < 3; ++ i )
	{
		//表示如果一个方向上两个盒子最小值和最大值的比有以下关系，不碰撞返回false，如果3个方向都不满足则说明有摩擦
		if ( min[i] > max2[i] || min2[i] > max[i] )
			return false;
	}
	return true;
}
//------------------------------------------------------------

int RenderModel::GetSequence()
{
	return model.GetSequence();
}
//------------------------------------------------------------

int RenderModel::GetFrame()
{
	return model.SetFrame( -1 );
}
//------------------------------------------------------------

void RenderModel::SetSequence( int s, int frame )
{
	if ( model.GetSequence() != s )
		model.SetSequence( s );
	if ( frame != 0 )
		model.SetFrame( frame );
}
//------------------------------------------------------------

bool RenderModel::IsAniEnd()
{
	return model.IsFrameEnd();
}
//------------------------------------------------------------

void RenderModel::Update( float dt, float speed )
{
	model.AdvanceFrame( dt * speed );
}
//------------------------------------------------------------
//人物模型绘画
void RenderModel::Draw()
{
	model.DrawModel();	//studiomodel
}

StudioModel* RenderModel::getStudioModel()
{
	return &model;
}
