#pragma  once

#include <queue>
#include "Player.h"
#include "Toolkit.h"

using namespace std;



class Command
{
public:
	Player*  ActivePlayer;
	Player*  TargetPlayer;
	

	CommandCategory CommandName;

	virtual void execute()=0;
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
	queue<Command*> Qc;
	int size;
public:
	CommandQ();
	~CommandQ();
	int push( Command* com );
	int pop();
	Command* front();

};