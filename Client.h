#pragma once
#include "MyCSocket.h"
#include "Toolkit.h"

class Client
{
public:
	Client(void);
	Client(char* IP, int PORT);
	void write2(char* str);
	void read2(char* str);
	void write(char*);
	//void Client::write( int stateCommand );
	void read(char*);
	DataPackage readDataPack(DataPackage* d);
public:
	~Client(void);

private:
	char* IP;
	int PORT;
	MyCSocket* socket;
	MyCSocket* socket2;
};
