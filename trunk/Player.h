#pragma once
#include "Toolkit.h"
#include <queue>

using namespace std;

class Command;

class Player
{
public:
	Player( float speedh = 30.0f, float speedv = 400.0f, float speedr = 20.0f );
	~Player();

	void setStateHor( int value );
	int getStateHor() const;

	void setStateVer( int value );
	int getStateVer() const;

	bool isForceAni();

	void setSpeedHorValue( float value );
	float getSpeedHorValue() const;

	void setSpeedVerValue( float value );
	float getSpeedVerValue() const;

	void setSpeedRotValue( float s );
	float getSpeedRotValue() const;

	void setSpeedHor( const Vector2D& v );
	const Vector2D& getSpeedHor() const;

	void setSpeedVer( float v );
	float getSpeedVer() const;

	void setStature( float s );
	float getStature();

	void setPosition( const Vector2D& p );
	const Vector2D& getPosition() const;

	void setHeight( float z );
	float getHeight() const;

	void setRotation( float r );
	float getRotation() const;
	Vector2D getDir() const;

	Player* getCollPlayer();
	void setCollPlayer( Player* p );

	int AnimationSequence;
	float AnimationSpeed;
	bool isForceAnimation;
	Command* currentCommand;

private:
	int state_hor;
	int state_ver;

	float speed_hor_value;
	float speed_ver_value;
	float speed_rot_value;

	Vector2D speed_hor;
	float speed_ver;

	float stature;	//身高

	Vector2D pos;	//位置 //为什么位置和高度分开来处理（对，高度不经常变化）
	float z;	//高度
	float rot;	//旋转

	Player* coll;	//相碰撞的角色
};


class Command
{
public:
	Player*  ActivePlayer;
	Player*  TargetPlayer;


	CommandCategory CommandName;

	virtual void execute()=0;
	virtual ~Command()
	{
	}
};

class RunBackCommand:public Command
{
public:
	RunBackCommand( Player* aPlayer );
	virtual ~RunBackCommand(){}

	virtual void execute();

};

class RunCommand:public Command
{
public:
	RunCommand( Player* aPlayer );
	virtual ~RunCommand(){}

	virtual void execute();

};

class IDLECommand:public Command
{
public:
	IDLECommand( Player* aPlayer );
	virtual ~IDLECommand(){}

	virtual void execute();

};

class MoveLeftCommand:public Command
{
public:
	MoveLeftCommand( Player* aPlayer );
	virtual ~MoveLeftCommand(){}

	virtual void execute();

};

class MoveRightCommand:public Command
{
public:
	MoveRightCommand( Player* aPlayer );
	virtual ~MoveRightCommand(){}

	virtual void execute();

};
class JumpCommand:public Command
{
public:
	JumpCommand( Player* aPlayer );
	virtual ~JumpCommand(){}

	virtual void execute();

};

class WeakFightCommand:public Command
{
public:
	WeakFightCommand( Player* aPlayer );
	virtual ~WeakFightCommand(){}

	virtual void execute();

};
class StrongFightCommand:public Command
{
public:
	StrongFightCommand( Player* aPlayer );
	virtual ~StrongFightCommand(){}

	virtual void execute();

};

class QiGongCommand:public Command
{
public:
	QiGongCommand( Player* aPlayer );
	virtual ~QiGongCommand(){}

	virtual void execute();

};

class WeakFootCommand:public Command
{
public:
	WeakFootCommand( Player* aPlayer );
	virtual ~WeakFootCommand(){}

	virtual void execute();


};

class StrongFootCommand:public Command
{
public:
	StrongFootCommand( Player* aPlayer );
	virtual ~StrongFootCommand(){}

	virtual void execute();

};


class CommandLookUpTable
{
public:
	static Command*  getNewCommand( CommandCategory cc, Player* aPlayer );

};

class CommandQ
{
	queue<Command* > Qc;
	int size;
public:
	CommandQ();
	~CommandQ();
	int push( Command* com );
	int pop();
	bool empty();
	Command* front();

};
