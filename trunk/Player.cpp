#include "Player.h"
#include "Toolkit.h"

Player::Player( float speedh, float speedv, float speedr )
{
	state_hor = STATEH_IDLE;
	state_ver = STATEV_STAND;

	speed_hor_value = speedh;
	speed_ver_value = speedv;
	speed_rot_value = speedr;

	speed_ver = 0;
	z = 0;
	rot = 0;

	AnimationSequence = 1;
	AnimationSpeed = 2;
	isForceAnimation = false;

	coll = NULL;
	currentCommand = NULL;
}

Player::~Player()
{
	if ( coll ) delete coll;
	if ( currentCommand ) delete currentCommand;
}

bool Player::isForceAni()
{
	//攻击或受到攻击 则强制动画
	//return state_hor == STATEH_WEAKFIGHT || state_hor == STATEH_WEAKFIGHTHIT || 
	//	state_hor == STATEH_STRONGFIGHT || state_hor == STATEH_STRONGFIGHTHIT || 
	//	state_hor == STATEH_WEAKFOOT || state_hor == STATEH_WEAKFOOTHIT || 
	//	state_hor == STATEH_STRONGFOOT || state_hor == STATEH_STRONGFOOTHIT;
	return isForceAnimation;
}

//得到方向用来乘上速度得到速度矢量
Vector2D Player::getDir() const
{
	return Vector2D( cosf(rot / 180 * PI), sinf(rot / 180 * PI) );//为什么要用SIN,COS
}

//一堆的GETTER SETTER，不理

void Player::setStateHor( int value )
{
	this->state_hor = value;
}

int Player::getStateHor() const
{
	return this->state_hor;
}

void Player::setStateVer( int value )
{
	this->state_ver = value;
}

int Player::getStateVer() const
{
	return this->state_ver;
}

void Player::setSpeedHorValue(float value)
{
	this->speed_hor_value = value;
}

float Player::getSpeedHorValue() const
{
	return this->speed_hor_value;
}

void Player::setSpeedVerValue(float value)
{
	this->speed_ver_value = value;
}

float Player::getSpeedVerValue() const
{
	return this->speed_hor_value;
}

void Player::setSpeedRotValue( float s )
{
	speed_rot_value = s;
}

float Player::getSpeedRotValue() const
{
	return speed_rot_value;
}

void Player::setSpeedHor(const Vector2D& v)
{
	this->speed_hor = v;
}

const Vector2D& Player::getSpeedHor() const
{
	return this->speed_hor;
}

void Player::setSpeedVer(float v)
{
	this->speed_ver = v;
}

float Player::getSpeedVer() const
{
	return this->speed_ver;
}

void Player::setStature( float s )
{
	stature = s;
}

float Player::getStature()
{
	return stature;
}

void Player::setPosition(const Vector2D& p)
{
	this->pos = p;
}

const Vector2D& Player::getPosition() const
{
	return this->pos;
}

void Player::setHeight(float z)
{
	this->z = z;
}

float Player::getHeight() const
{
	return z;
}

void Player::setRotation(float r)
{
	if(r >= 360)
		r -= 360;
	else if(r <= -360)
		r += 360;
	rot = r;

	if(getSpeedHor().Length() != 0)
		speed_hor = getDir() * speed_hor_value;
}

float Player::getRotation() const
{
	return rot;
}


Player* Player::getCollPlayer()
{
	return coll;
}

void Player::setCollPlayer( Player* p )
{
	coll = p;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////


CommandQ::CommandQ()
{
	size = 20;
}

CommandQ::~CommandQ()
{
	Command* pCom = NULL;
	while ( Qc.size() > 0 )
	{
		pCom = Qc.front();
		delete pCom;
		Qc.pop();
	}
}

Command* CommandQ::front()
{
	if ( Qc.size() > 0 )	return Qc.front();
	else return NULL;
}

bool CommandQ::empty()
{
	return Qc.size() == 0;
}

int CommandQ::push( Command* com )
{
	if ( Qc.size() < size )
	{
		Qc.push(com);
		return 1;
	}
	else return 0;
}

int CommandQ::pop()
{
	Command* pCom = NULL;
	if ( Qc.size() > 0 )
	{
		pCom = Qc.front();
		Qc.pop();
		pCom->ActivePlayer->currentCommand = NULL;
		delete pCom;		
		return 1;
	}
	else return 0;
}

//////////////////////////////////////////////////////////////////////////
Command* CommandLookUpTable::getNewCommand( CommandCategory cc ,Player* aPlayer )
{
	switch ( cc )
	{
	case RUNBACK:
		return new RunBackCommand( aPlayer );
	case RUN:
		return new RunCommand( aPlayer );
	case IDLE:
		return new IDLECommand( aPlayer );
	case LEFT:
		return new MoveLeftCommand( aPlayer );
	case RIGHT:
		return new MoveRightCommand( aPlayer );
	case JUMP:
		return new JumpCommand( aPlayer );
	case WEAKFIGHT:
		return new WeakFightCommand( aPlayer );
	case STRONGFIGHT:
		return new StrongFightCommand( aPlayer );
	case QIGONG:
		return new QiGongCommand( aPlayer );
	case WEAKFOOT:
		return new WeakFootCommand( aPlayer );
	case STRONGFOOT:
		return new StrongFootCommand( aPlayer );
		//出错处理

	}
}


//////////////////////////////////////////////////////////////////////////
RunBackCommand::RunBackCommand( Player* aPlayer )
{
	this->ActivePlayer = aPlayer;
	this->TargetPlayer = NULL ;

	this->CommandName = RUNBACK;
}

void RunBackCommand::execute()
{
	ActivePlayer->AnimationSequence = 9;
	ActivePlayer->AnimationSpeed = 3;
	ActivePlayer->isForceAnimation = false;

	ActivePlayer->currentCommand = this; 
	//这里应该注意到有两个指针同时指向了一个堆上的位置，造成了后来潜在的堆腐烂
	//Command* pCom = ActivePlayer->currentCommand;
	//if ( pCom ) delete pCom;
	//pCom = new RunBackCommand( ActivePlayer );

	ActivePlayer->setSpeedHor(
		(-( ActivePlayer->getDir() ) ) *
		ActivePlayer->getSpeedHorValue()
		);

}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
RunCommand::RunCommand( Player* aPlayer )
{
	this->ActivePlayer = aPlayer;
	this->TargetPlayer = NULL ;

	this->CommandName = RUN;
}

void RunCommand::execute()
{
	ActivePlayer->AnimationSequence = 4;
	ActivePlayer->AnimationSpeed = 3;
	ActivePlayer->isForceAnimation = false;

	ActivePlayer->currentCommand = this;

	//Command* pCom = ActivePlayer->currentCommand;
	//if ( pCom ) delete pCom;
	//pCom = new RunCommand( ActivePlayer );

	ActivePlayer->setSpeedHor(
		ActivePlayer->getDir()  *
		ActivePlayer->getSpeedHorValue()
		);

}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
IDLECommand::IDLECommand( Player* aPlayer )
{
	this->ActivePlayer = aPlayer;
	this->TargetPlayer = NULL ;

	this->CommandName = IDLE;
}

void IDLECommand::execute()
{
	ActivePlayer->AnimationSequence = 1;
	ActivePlayer->AnimationSpeed = 2;
	ActivePlayer->isForceAnimation = false;

	ActivePlayer->currentCommand = this;

	//Command* pCom = ActivePlayer->currentCommand;
	//if ( pCom ) delete pCom;
	//pCom = new IDLECommand( ActivePlayer );

	Vector2D v;
	ActivePlayer->setSpeedHor(v);


}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
MoveLeftCommand::MoveLeftCommand( Player* aPlayer )
{
	this->ActivePlayer = aPlayer;
	this->TargetPlayer = NULL ;

	this->CommandName = LEFT;
}

void MoveLeftCommand::execute()
{
	ActivePlayer->AnimationSequence = 1;
	ActivePlayer->AnimationSpeed = 2;
	ActivePlayer->isForceAnimation = false;

	ActivePlayer->currentCommand = this;

	//Command* pCom = ActivePlayer->currentCommand;
	//if ( pCom ) delete pCom;
	//pCom = new MoveLeftCommand( ActivePlayer );

	ActivePlayer->setRotation(
		ActivePlayer->getRotation() + ActivePlayer->getSpeedRotValue()
		);

}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
MoveRightCommand::MoveRightCommand( Player* aPlayer )
{
	this->ActivePlayer = aPlayer;
	this->TargetPlayer = NULL ;

	this->CommandName = RIGHT;
}

void MoveRightCommand::execute()
{
	ActivePlayer->AnimationSequence = 1;
	ActivePlayer->AnimationSpeed = 2;
	ActivePlayer->isForceAnimation = false;

	ActivePlayer->currentCommand = this;

	//Command* pCom = ActivePlayer->currentCommand;
	//if ( pCom ) delete pCom;
	//pCom = new MoveRightCommand( ActivePlayer );

	ActivePlayer->setRotation(
		ActivePlayer->getRotation() - ActivePlayer->getSpeedRotValue()
		);

}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
JumpCommand::JumpCommand( Player* aPlayer )
{
	this->ActivePlayer = aPlayer;
	this->TargetPlayer = NULL ;

	this->CommandName = JUMP;

}

void JumpCommand::execute()
{
	ActivePlayer->AnimationSequence = 10;
	ActivePlayer->AnimationSpeed = 2;
	ActivePlayer->isForceAnimation = false;

	ActivePlayer->currentCommand = this;

	//Command* pCom = ActivePlayer->currentCommand;
	//if ( pCom ) delete pCom;
	//pCom = new JumpCommand( ActivePlayer );

	ActivePlayer->setSpeedVer(
		ActivePlayer->getSpeedVerValue()
		);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
WeakFightCommand::WeakFightCommand( Player* aPlayer )
{
	this->ActivePlayer = aPlayer;
	this->TargetPlayer = NULL ;

	this->CommandName = WEAKFIGHT;
}

void WeakFightCommand::execute()
{
	ActivePlayer->AnimationSequence = 136;
	ActivePlayer->AnimationSpeed = 4;
	ActivePlayer->isForceAnimation = true;

	ActivePlayer->currentCommand = this;

	//Command* pCom = ActivePlayer->currentCommand;
	//if ( pCom ) delete pCom;
	//pCom = new WeakFightCommand( ActivePlayer );

	if (!ActivePlayer->isForceAni())
	{
		ActivePlayer->setSpeedHor(Vector2D(0, 0));
	}

	if ( TargetPlayer ) 
	{
		TargetPlayer->AnimationSequence = 160;
		TargetPlayer->AnimationSpeed = 4;
		TargetPlayer->isForceAnimation = true;
	}

}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
StrongFightCommand::StrongFightCommand( Player* aPlayer )
{
	this->ActivePlayer = aPlayer;
	this->TargetPlayer = NULL ;

	this->CommandName = STRONGFIGHT;
}

void StrongFightCommand::execute()
{
	ActivePlayer->AnimationSequence = 137;
	ActivePlayer->AnimationSpeed = 4;
	ActivePlayer->isForceAnimation = true;

	ActivePlayer->currentCommand = this;

	//Command* pCom = ActivePlayer->currentCommand;
	//if ( pCom ) delete pCom;
	//pCom = new StrongFightCommand( ActivePlayer );

	if (!ActivePlayer->isForceAni())
	{
		ActivePlayer->setSpeedHor(Vector2D(0, 0));
	}

	if ( TargetPlayer ) 
	{
		TargetPlayer->AnimationSequence = 161;
		TargetPlayer->AnimationSpeed = 4;
		TargetPlayer->isForceAnimation = true;
	}

}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
QiGongCommand::QiGongCommand( Player* aPlayer )
{
	this->ActivePlayer = aPlayer;
	this->TargetPlayer = NULL ;

	this->CommandName = QIGONG;
}

void QiGongCommand::execute()
{
	ActivePlayer->AnimationSequence = 90;
	ActivePlayer->AnimationSpeed = 3;
	ActivePlayer->isForceAnimation = true;

	ActivePlayer->currentCommand = this;

	//Command* pCom = ActivePlayer->currentCommand;
	//if ( pCom ) delete pCom;
	//pCom = new QiGongCommand( ActivePlayer );

	if ( !ActivePlayer->isForceAni() )
	{
		ActivePlayer->setSpeedHor(Vector2D(0, 0));
	}

	//if ( TargetPlayer ) //气功击中动画似乎未定义
	//{
	//	TargetPlayer->AnimationSequence = 160;
	//	TargetPlayer->AnimationSpeed = 4;
	//	TargetPlayer->isForceAnimation = true;
	//}

}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
WeakFootCommand::WeakFootCommand( Player* aPlayer )
{
	this->ActivePlayer = aPlayer;
	this->TargetPlayer = NULL ;

	this->CommandName = WEAKFOOT;
}

void WeakFootCommand::execute()
{
	ActivePlayer->AnimationSequence = 139;
	ActivePlayer->AnimationSpeed = 5;
	ActivePlayer->isForceAnimation = true;

	ActivePlayer->currentCommand = this;

	//Command* pCom = ActivePlayer->currentCommand;
	//if ( pCom ) delete pCom;
	//pCom = new WeakFootCommand( ActivePlayer );

	if (!ActivePlayer->isForceAni())
	{
		ActivePlayer->setSpeedHor(Vector2D(0, 0));
	}

	if ( TargetPlayer ) 
	{
		TargetPlayer->AnimationSequence = 164;
		TargetPlayer->AnimationSpeed = 4;
		TargetPlayer->isForceAnimation = true;
	}

}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
StrongFootCommand::StrongFootCommand( Player* aPlayer )
{
	this->ActivePlayer = aPlayer;
	this->TargetPlayer = NULL ;

	this->CommandName = STRONGFOOT;
}

void StrongFootCommand::execute()
{
	ActivePlayer->AnimationSequence = 140;
	ActivePlayer->AnimationSpeed = 4;
	ActivePlayer->isForceAnimation = true;

	ActivePlayer->currentCommand = this;

	//Command* pCom = ActivePlayer->currentCommand;
	//if ( pCom ) delete pCom;
	//pCom = new StrongFootCommand( ActivePlayer );

	if (!ActivePlayer->isForceAni())
	{
		ActivePlayer->setSpeedHor(Vector2D(0, 0));
	}

	if ( TargetPlayer ) 
	{
		TargetPlayer->AnimationSequence = 164;
		TargetPlayer->AnimationSpeed = 4;
		TargetPlayer->isForceAnimation = true;
	}

}
//////////////////////////////////////////////////////////////////////////
