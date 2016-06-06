#pragma once
#include "Head.h"
class CSerial
{
public:
	//无线通信相关
	HANDLE	hCom;
	DWORD	dw;
	bool	is_serial;	 
	char    send_buf[50];
	
	CSerial()
	{
		is_serial=true;
		memset(send_buf, 0, sizeof(send_buf));
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
	}
	~CSerial()
	{
		if(is_serial)
		{
			SentSerial('0',0.0000);
			::CloseHandle(hCom);//关闭串口
		}
	}

	void SentSerial(char num,double velocity)
	{
		sprintf(send_buf, "M%c%.1f\n", num, velocity);//velocity的小数点后两位存入，不用把四位全存入，这样可以使ARDRONE飞得更光滑稳定
		WriteFile(hCom, send_buf, 6, &dw, NULL);
	}
};
