#include "Client.h"
#include <iostream>

using namespace std;

Client::Client(void)
{
	socket = NULL;

	IP = new char[100];
	cout << "input the target IP address: " << endl;
	cin  >> IP;

	cout << "input the target PORT: " << endl;
	cin >> PORT;

	/*IP = "127.0.0.1";
	PORT = 1001;*/

	//create the socket and socket2 through the IP and PORT
	while( !socket )
	{
		try
		{
			socket = new MyCSocket(IP, PORT);
		}
		catch( MyCSocketException se)
		{
			cout << " : Exception => " << se.getText() << "\n";
			return;
		}
	}

	cout << "Server Port " << PORT << " Connected." << endl;

	while( !socket2 )
	{
		try
		{
			socket2 = new MyCSocket(IP, PORT + 1);
		}
		catch( MyCSocketException se)
		{
			cout << " : Exception => " << se.getText() << "\n";
			return;
		}
	}

	cout << "Server Port " << PORT + 1 << " Connected." << endl;

}

//through the param to create the socket, socket2
Client::Client(char* IP, int PORT)
{
	//this->IP = new char[100];
	this->IP = IP;
	this->PORT = PORT;
	socket = NULL;
	socket2 = NULL;

	while( !socket )
	{
		try
		{
			socket = new MyCSocket(this->IP, this->PORT);
		}
		catch( MyCSocketException se)
		{
			cout << " : Exception => " << se.getText() << "\n";
			return;
		}
	}

	cout << "Server Port " << PORT << " Connected." << endl;

	while( !socket2 )
	{
		try
		{
			socket2 = new MyCSocket(this->IP, this->PORT + 1);
		}
		catch( MyCSocketException se)
		{
			cout << " : Exception => " << se.getText() << "\n";
			return;
		}
	}

	cout << "Server Port " << PORT + 1 << " Connected." << endl;
}

Client::~Client(void)
{
}

void Client::write(char* str)
{
	socket->Write(str, strlen(str));
}

//void Client::write( void* stateCommand )
//{
//	//char strCommand[10];
//	//itoa(stateCommand,strCommand,10);
//	socket->Write( strCommand, strlen( strCommand ));
//}

//void Client::read( int stateCommand )
//{
//	char strCommand[10];
//	memset(strCommand,0,10);
//	socket->Read(strCommand
//}

void Client::read(char * str)
{
	memset( str, 0, 100);
	socket->Read(str, 100);
}

//从SOCKET读出数据放入STR
void Client::write2(char* str)
{
	socket2->Write(str, strlen(str));
}

//从SOCKET读出数据放入STR
void Client::read2(char* str)
{
	memset( str, 0, 100);
	socket2->Read(str, 100);
}

DataPackage Client::readDataPack(DataPackage* d)
{
	char* str = new char[100];

	for(int i = 0 ; i < 9 ; i++)
	{
		read2(str);//读出一个数据

		if(!str)
			break;

		if(!strcmp(str, "EXIT"))
			exit(0);

		if(str[0] == 'a')
		{
			d->IsServer = d->castCharPtrToBool(str + 1);
		}
		else if(str[0] == 'b')
		{
			d->pos.x = d->castCharPtrToFloat(str + 1);
		}
		else if(str[0] == 'c')
		{
			d->pos.y = d->castCharPtrToFloat(str + 1);
		}
		else if(str[0] == 'd')
		{
			d->height = d->castCharPtrToFloat(str + 1);
		}
		else if(str[0] == 'e')
		{
			d->HorState = d->castCharPtrToInt(str + 1);
		}
		else if(str[0] == 'f')
		{
			d->VerState = d->castCharPtrToInt(str + 1);
		}
		else if(str[0] == 'g')
		{
			d->rotation = d->castCharPtrToFloat(str + 1);
		}
		else if(str[0] == 'h')
		{
			d->frame = d->castCharPtrToInt(str + 1);
		}
		else if(str[0] == 'i')
		{
			d->sequence = d->castCharPtrToInt(str + 1);//效率是相当....完全可以用HASHMAP
		}
		write2(str);//这个读出来，然后又写回去，是怎么一回事呢？STR并没有被修改
	}

	delete str;//做什么 似乎是解析了一个Socket命令然后封回了DP
	//从设计上来讲这不应该是Client的方法，应该是DP的方法，DP去解析一个数据字节流么

	return d;//居然没报错，和返回类型不匹配啊
}

