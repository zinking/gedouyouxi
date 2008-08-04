#include "Command.h"

/* enum CommandCategory{ RUNBACK,RUN,IDLE,LEFT,RIGHT,JUMP,WEAKFIGHT,
STRONGFIGHT,QIGONG,WEAKFOOT,STRONGFOOT,}; */

//////////////////////////////////////////////////////////////////////////
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
	return Qc.front();
}

int CommandQ::push(Command *com)
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

