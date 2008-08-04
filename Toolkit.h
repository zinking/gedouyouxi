#pragma once

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


const float PI = 3.141592653589793f;
const float ZERO = 0.00001f;

//------------------------------------------------------------
enum CommandCategory
{ 
	RUNBACK = 3,//真是什么错都有，0的整数不能传
	RUN,
	IDLE,
	LEFT,
	RIGHT,
	JUMP,
	WEAKFIGHT,
	STRONGFIGHT,
	QIGONG,
	WEAKFOOT,
	STRONGFOOT,
};

enum	STATEH//水平运动状态
{
	STATEH_IDLE=0,
	STATEH_DEEPIDLE,
	STATEH_RUN,
	STATEH_RUNBACK,

	STATEH_LEFT,
	STATEH_RIGHT,
	STATEH_JUMP,

	STATEH_WEAKFIGHT=100,
	STATEH_STRONGFIGHT,
	STATEH_QIGONG,
	STATEH_WEAKFOOT,
	STATEH_STRONGFOOT,


	STATEH_WEAKFIGHTHIT=200,
	STATEH_STRONGFIGHTHIT,
	STATEH_QIGONGHIT,
	STATEH_WEAKFOOTHIT,
	STATEH_STRONGFOOTHIT,
};

enum	//垂直运动状态
{
	STATEV_STAND,
	STATEV_JUMP
};
//------------------------------------------------------------
struct Vector2D
{
	float x, y;

	Vector2D( float X = 0, float Y = 0 )
	{
		x = X;
		y = Y;
	}

	Vector2D& operator += ( const Vector2D& v )
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vector2D operator + ( const Vector2D& v ) const
	{
		return Vector2D( x + v.x, y + v.y );
	}

	Vector2D& operator -= ( const Vector2D& v )
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vector2D operator - ( const Vector2D& v ) const
	{
		return Vector2D( x - v.x, y - v.y );
	}

	Vector2D operator - () const
	{
		return Vector2D( - x, - y );
	}

	Vector2D& operator *= ( float f )
	{
		x *= f;
		y *= f;
		return *this;
	}

	Vector2D operator * ( float f ) const
	{
		return Vector2D( x * f, y * f );
	}

	Vector2D& operator /= ( float f )
	{
		x /= f;
		y /= f;
		return *this;
	}

	Vector2D operator / ( float f ) const
	{
		return Vector2D( x / f, y / f );
	}

	float Dot( const Vector2D& v ) const
	{
		return x * v.x + y * v.y;
	}

	float Length() const
	{
		return sqrt( x * x + y * y );
	}

	void Normalize()
	{
		float l = Length();
		x /= l;
		y /= l;
	}
};
//------------------------------------------------------------
struct Vector3D
{
	float x, y, z;

	Vector3D( float X = 0, float Y = 0, float Z = 0 )
	{
		x = X;
		y = Y;
		z = Z;
	}
};
//------------------------------------------------------------
void ReadStr( FILE* f, char* string );


//数据包的用途没看出来
struct DataPackage
{
	bool IsServer;
	Vector2D pos;
	float height;
	int HorState;
	int VerState;
	float rotation;
	int frame;
	int sequence;

	DataPackage(bool i = false, Vector2D p = Vector2D(0, 0), 
		float h = 0.0f, int hs = 0, int vs = 0, float r = 0.0f,
		int f = 0, int s = 0)
	{
		IsServer = i;
		pos = p;
		height = h;
		HorState = hs;
		VerState = vs;
		rotation = r;
		frame = f;
		sequence = s;
	}

	char* castBoolToCharPtr(bool tmp)
	{
		char* t = "TRUE";
		char* f = "FALSE";
		if(tmp)
			return t;
		else
			return f;
	}

	bool castCharPtrToBool(char* tmp)
	{
		if(!strcmp(tmp, "TRUE"))
			return true;
		else
			return false;
	}

	char* castFloatToCharPtr(float tmp)
	{
		//潜在的内存泄漏
		char* str = new char[100];
		sprintf(str,"%f",tmp);
		return str;
	}

	float castCharPtrToFloat(char* tmp)
	{
		double f;
		f = atof ( tmp );
		return (float)f;
	}

	char* castIntToCharPtr(int i)
	{
		//潜在的内存泄漏
		char* str = new char[100];
		itoa (i,str,10);
		return str;
	}

	int castCharPtrToInt(char* tmp)
	{  
		int i;
		i = atoi (tmp);
		return i;
	}
};