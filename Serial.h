#pragma once
#include "Head.h"
class CSerial
{
public:
	//����ͨ�����
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
			0,// FILE_FLAG_OVERLAPPED,//������ͨ�ţ�
			NULL
			);//�������ھ��
		if(hCom ==INVALID_HANDLE_VALUE)is_serial=false;
		//����Ϊ�򿪴���
		DCB dcb={0};
		if(is_serial)
		{
			dcb.DCBlength = sizeof(DCB);
			if(!::GetCommState(hCom,&dcb))is_serial=false;//��⴮������
		}
		if(is_serial)
		{//����DCB��ֵ�����ô���
			dcb.BaudRate = 57600;    
			dcb.ByteSize = 8;
			dcb.Parity = NOPARITY;
			dcb.StopBits =ONESTOPBIT;
			if(!::SetCommState(hCom,&dcb))is_serial=false;//���ô���
		}
		//��д����ǰ������ջ�����
		if( is_serial&&!::PurgeComm( hCom, PURGE_RXCLEAR ) )is_serial=false;
	}
	~CSerial()
	{
		if(is_serial)
		{
			SentSerial(0,0);
			::CloseHandle(hCom);//�رմ���
		}
	}

	void SentSerial(char num,bool velocity)
	{
		sprintf(send_buf, "%d%d\n", num,velocity);//������ӡ���ַ�����
		WriteFile(hCom, send_buf, 3, &dw, NULL);
	}
};
