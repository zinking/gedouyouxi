#pragma once
#include "MyCSocket.h"
#include "Toolkit.h"

class Server
{
public:
	Server(void);
	Server(int p);
	void write(char*);
	void read(char*);
	void sendDataPack(DataPackage &d);
	void sendScene(void);
	void write2(char* str);
	void read2(char* str);
public:
	~Server(void);

private:
	int PORT;
	MyCSocket* server_socket;
	MyCSocket* client_socket;
	MyCSocket* server_socket2;
	MyCSocket* client_socket2;

};
