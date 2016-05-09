#pragma once
#include "Head.h"
#define SPEED 80
class CSerial
{
public:
	//无线通信相关
	HANDLE	hCom;
	DWORD	dw;
	bool	is_serial;	 
	int		serial_state;
	char    p[7][1];	//   only for R2 car
	
	CSerial()
	{
		is_serial=true;
		hCom = ::CreateFile(COM_PORT,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,// FILE_FLAG_OVERLAPPED,//非阻塞通信，
			NULL
			);//创建串口句柄
		if(hCom ==INVALID_HANDLE_VALUE)is_serial=false;
		//以上为打开串口
		DCB dcb={0};
		if(is_serial)
		{
			dcb.DCBlength = sizeof(DCB);
			if(!::GetCommState(hCom,&dcb))is_serial=false;//检测串口设置
		}
		if(is_serial)
		{//设置DCB初值，配置串口
			dcb.BaudRate = 57600;    
			dcb.ByteSize = 8;
			dcb.Parity = NOPARITY;
			dcb.StopBits =ONESTOPBIT;
			if(!::SetCommState(hCom,&dcb))is_serial=false;//设置串口
		}
		//读写串口前，先清空缓冲区
		if( is_serial&&!::PurgeComm( hCom, PURGE_RXCLEAR ) )is_serial=false;

		// only for R2 car
		p[0][0] = '1';
		p[1][0] = '2';
		p[2][0] = '3';
		p[3][0] = '4';
		p[4][0] = '5';
		p[5][0] = '6';
		p[6][0] = '7';

		serial_state=5;//初始化为5，只是为了区别于0，1，2，3，飞控里5为遥控器手动控制
	}
	~CSerial()
	{
		if(is_serial)
		{
			SentSerial(5);
			::CloseHandle(hCom);//关闭串口
		}
	}
	bool CheckSerial()
	{

	}

	void SentSerial(int num)
	{
		serial_state=num;//num定义串口状态
		if(is_serial)
		{
			//::WriteFile(hCom,s[num],11,&dw,NULL);
			//char buff[1];
			int k=1;       // each command repeat for k times
			switch(num)
			{
			case 0://前
				for(int i=0;i<k;i++)
				{
					::WriteFile(hCom,p[2],1,&dw,NULL);
				}
				break;
			case 1://右
				for(int i=0;i<k;i++)
				{
					::WriteFile(hCom,p[0],1,&dw,NULL);
				}
				break;
			case 2://左
				for(int i=0;i<k;i++)
				{
					::WriteFile(hCom,p[1],1,&dw,NULL);
				}
				break;
			case 3://后
				for(int i=0;i<k;i++)
				{
					::WriteFile(hCom,p[3],1,&dw,NULL);
				}
				break;
			case 5://紧急手动控制
				for (int i = 0; i<k; i++)
				{
					::WriteFile(hCom, p[4], 1, &dw, NULL);
				}
				break;
			case 6://起飞
				for (int i = 0; i<k; i++)
				{
					::WriteFile(hCom, p[5], 1, &dw, NULL);
				}
				break;
			case 7://降落
				for (int i = 0; i<k; i++)
				{
					::WriteFile(hCom, p[6], 1, &dw, NULL);
				}
				break;
			}
			//buff[0]=char(num);
			//::WriteFile(hCom,buff,1,&dw,NULL);
		}
	}
};
