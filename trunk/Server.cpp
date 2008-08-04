#include "Server.h"
#include <iostream>

using namespace std;

Server::Server(void)
{
}

Server::~Server(void)
{
}

Server::Server(int p)
{
	this->sendScene();//未实现
	PORT = p;
	this->client_socket = NULL;
	this->client_socket2 = NULL;

	try
	{
		this->server_socket = new MyCSocket(PORT);
	}
	catch( MyCSocketException se)
	{
		cout << " : Exception => " << se.getText() << "\n";
		return;
	}

	cout << "Receiving Client at PORT: " << p << endl;

	while( !client_socket )
	{
		client_socket = server_socket->Accept();
	}

	cout << "Client Connected." << endl;	
	
	try
	{
		this->server_socket2 = new MyCSocket(PORT + 1);
	}
	catch( MyCSocketException se)
	{
		cout << " : Exception => " << se.getText() << "\n";
		return;
	}

	cout << "Receiving Client at PORT: " << p + 1 << endl;

	while( !client_socket2 )
	{
		client_socket2 = server_socket2->Accept();
	}

	cout << "Client Connected." << endl;
}


void Server::write(char* str)
{
	this->client_socket->Write(str, strlen( str ));
}

void Server::read(char* str)
{
	memset( str, 0, 20);
	client_socket->Read(str, 20);
}

void Server::write2(char* str)
{
	this->client_socket2->Write(str, strlen( str ));
}

void Server::read2(char* str)
{
	memset( str, 0, 100);
	client_socket2->Read(str, 100);
}

void Server::sendScene(void)
{

}

void Server::sendDataPack(DataPackage &d)
{
	char* str = new char[100];
	strcpy(str, "a");

	write2(strcat(str, d.castBoolToCharPtr(d.IsServer)));
	read2(str);//发送数据包的时候同时读是什么意思
	strcpy(str, "b");
	write2(strcat(str, d.castFloatToCharPtr(d.pos.x)));
	read2(str);
	strcpy(str, "c");
	write2(strcat(str, d.castFloatToCharPtr(d.pos.y)));
	read2(str);
	strcpy(str, "d");
	write2(strcat(str, d.castFloatToCharPtr(d.height)));
	read2(str);
	strcpy(str, "e");
	write2(strcat(str, d.castIntToCharPtr(d.HorState)));
	read2(str);
	strcpy(str, "f");
	write2(strcat(str, d.castIntToCharPtr(d.VerState)));
	read2(str);
	strcpy(str, "g");
	write2(strcat(str, d.castFloatToCharPtr(d.rotation)));
	read2(str);
	strcpy(str, "h");
	write2(strcat(str, d.castIntToCharPtr(d.frame)));
	read2(str);
	strcpy(str, "i");
	write2(strcat(str, d.castIntToCharPtr(d.sequence)));
	read2(str);


	delete str;
}

