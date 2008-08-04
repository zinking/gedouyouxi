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

//��SOCKET�������ݷ���STR
void Client::write2(char* str)
{
	socket2->Write(str, strlen(str));
}

//��SOCKET�������ݷ���STR
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
		read2(str);//����һ������

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
			d->sequence = d->castCharPtrToInt(str + 1);//Ч�����൱....��ȫ������HASHMAP
		}
		write2(str);//�����������Ȼ����д��ȥ������ôһ�����أ�STR��û�б��޸�
	}

	delete str;//��ʲô �ƺ��ǽ�����һ��Socket����Ȼ������DP
	//������������ⲻӦ����Client�ķ�����Ӧ����DP�ķ�����DPȥ����һ�������ֽ���ô

	return d;//��Ȼû�����ͷ������Ͳ�ƥ�䰡
}

