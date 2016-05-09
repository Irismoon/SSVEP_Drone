#pragma once
#include "Head.h"
#define SPEED 80
class CSerial
{
public:
	//����ͨ�����
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

		// only for R2 car
		p[0][0] = '1';
		p[1][0] = '2';
		p[2][0] = '3';
		p[3][0] = '4';
		p[4][0] = '5';
		p[5][0] = '6';
		p[6][0] = '7';

		serial_state=5;//��ʼ��Ϊ5��ֻ��Ϊ��������0��1��2��3���ɿ���5Ϊң�����ֶ�����
	}
	~CSerial()
	{
		if(is_serial)
		{
			SentSerial(5);
			::CloseHandle(hCom);//�رմ���
		}
	}
	bool CheckSerial()
	{

	}

	void SentSerial(int num)
	{
		serial_state=num;//num���崮��״̬
		if(is_serial)
		{
			//::WriteFile(hCom,s[num],11,&dw,NULL);
			//char buff[1];
			int k=1;       // each command repeat for k times
			switch(num)
			{
			case 0://ǰ
				for(int i=0;i<k;i++)
				{
					::WriteFile(hCom,p[2],1,&dw,NULL);
				}
				break;
			case 1://��
				for(int i=0;i<k;i++)
				{
					::WriteFile(hCom,p[0],1,&dw,NULL);
				}
				break;
			case 2://��
				for(int i=0;i<k;i++)
				{
					::WriteFile(hCom,p[1],1,&dw,NULL);
				}
				break;
			case 3://��
				for(int i=0;i<k;i++)
				{
					::WriteFile(hCom,p[3],1,&dw,NULL);
				}
				break;
			case 5://�����ֶ�����
				for (int i = 0; i<k; i++)
				{
					::WriteFile(hCom, p[4], 1, &dw, NULL);
				}
				break;
			case 6://���
				for (int i = 0; i<k; i++)
				{
					::WriteFile(hCom, p[5], 1, &dw, NULL);
				}
				break;
			case 7://����
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
