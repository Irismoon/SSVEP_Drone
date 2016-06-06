#pragma once
#include "Head.h"

static DWORD WINAPI FileThreadEntry(PVOID arg);
static DWORD WINAPI MatlabThreadEntry(PVOID arg);
static DWORD WINAPI SerialportThreadEntry(PVOID arg);

class CSVEP
{
	//处理SSVEP的数据
public:
	int		data[DATA_LENGTH][DATA_CHANNEL];
	int		data_index;
	//matlab和C++运行时互不操作,0-都没操作，1-C++在操作，2-matlab在操作
	int		critical;
	int     serialnum;

	//用于C++和matlab之间传变量,
	//C++->matlab:传时间标签和标记
	//matlab->C++:传实时返回结果
	double	signal;		
	double  velocity;
	double  variance;
	int		take_var;
	int     land_var;
	HANDLE	m_hThread_file;							//用于线程控制			
	HANDLE	m_hThread_mat;							//用于线程控制
	HANDLE  m_hThread_port;

	char	file_mat[100];
	int		state;                                 //SVEP程序状态，play/stop/end/rest，通过空格键来回切换
	int		mark[DATA_TYPE];						//用于标记控制状态下判断的方向
	int		trigger[DATA_LENGTH];
	//ofstream	evafile;
	//LARGE_INTEGER tstart, tend, fequency;//64位有符号整数 

	CSVEP()
	{		
		data_index=-1;
		take_var=0;
		land_var = 0;
		signal=-1;
		velocity = 0;
		variance = 0;
		state=STATE_STOP;
		serialnum = 0;
		//if (Evaluation) evafile.open("evaluation.txt");
		//QueryPerformanceFrequency(&fequency);
		for(int i=0;i<DATA_TYPE;i++)
			mark[i]=0;
		m_hThread_file=CreateThread(NULL,0,FileThreadEntry,(PVOID)this,0,NULL);	
		m_hThread_mat=CreateThread(NULL,0,MatlabThreadEntry,(PVOID)this,0,NULL);
		m_hThread_port = CreateThread(NULL, 0, SerialportThreadEntry, (PVOID)this, 0, NULL);
	}

	~CSVEP()
	{
		state=STATE_END;
		Sleep(1000);
		//if (DEBUG) evafile.close();
		if(m_hThread_file!=NULL) 
		{
			CloseHandle(m_hThread_file);
			m_hThread_file=NULL;
		}
		if(m_hThread_mat!=NULL) 
		{
			CloseHandle(m_hThread_mat);
			m_hThread_mat=NULL;
		}
		if (m_hThread_port != NULL)
		{
			CloseHandle(m_hThread_port);
			m_hThread_mat = NULL;
		}
	}

	void SetData(double *p,int nBlockPnts)
	{//存数据进data
		for (int i=0;i<nBlockPnts;i++)
		{
			if(state==STATE_PLAY)
			{
				data_index++;
				trigger[data_index%DATA_LENGTH]=trigger[(data_index-1+DATA_LENGTH)%DATA_LENGTH];
				if (Evaluation) {
					if (data_index%TRIAL == 0)//标签是以TRIAL为单位变化的
					{
						trigger[data_index%DATA_LENGTH] = rand() % DATA_TYPE+1;
						switch (trigger[data_index%DATA_LENGTH]) {
						case 1:
						PlaySound("up.wav", NULL, SND_ASYNC);
						break;
						case 4:
						PlaySound("right.wav", NULL, SND_ASYNC);
						break;
						case 3:
						PlaySound("left.wav", NULL, SND_ASYNC);
						break;
						case 2:
						PlaySound("down.wav", NULL, SND_ASYNC);
						break;
						}
					}
					if ((data_index + 1) % (SEGMENT) == 0)
						state = STATE_REST;
				}
				for (int j=0;j<DATA_CHANNEL;j++)
				{
					data[data_index%DATA_LENGTH][j]=*p;
					p++;//一行里一个一个地存
				}
				p++;//换下一行

			}
		}
	}

	void Write()
	{//将数据保存至文件
		char	file_date[100];
		char	file_cnt[100];
		char	file_mark[100];

		//文件记录
		sprintf(file_date,"2008.12.26_09.32");
		char buff[9];
		_strdate_s( buff, 9 );//Copy the current system date to a buffer. A pointer to a buffer which will be filled in with the formatted date string,Size of the buffer
		file_date[2]=buff[6];
		file_date[3]=buff[7];
		file_date[5]=buff[0];
		file_date[6]=buff[1];
		file_date[8]=buff[3];
		file_date[9]=buff[4];

		_strtime_s( buff, 9 );
		file_date[11]=buff[0];
		file_date[12]=buff[1];
		file_date[14]=buff[3];
		file_date[15]=buff[4];


		char fn[60];
		sprintf(fn,"md data-%s",file_date);system(fn);//打开fn，文件夹
		sprintf(file_cnt,"data-%s\\data_cnt-%s.txt",file_date,file_date);//文件
		sprintf(file_mark,"data-%s\\data_mark-%s.txt",file_date,file_date);
		sprintf(file_mat,"data=data(1:data_index,:);save(\'..\\\\data-%s\\\\data_mark-%s.mat\', \'data\')",file_date,file_date);


		ofstream file;
		file.open(file_cnt);

		int write_index=-1;
		while(state!=STATE_END)
		{			
			if(write_index<data_index)
			{
				write_index++;
				for(int j=0;j<DATA_CHANNEL;j++)
					file<<data[write_index%DATA_LENGTH][j]<<'\t';
				if(write_index<SEGMENT)
					file<<trigger[write_index%DATA_LENGTH];
				else
					file<<-1;
				file<<'\n';
			}
			else
				Sleep(5);
		}
		file.close();
	}

	void Serial()
	{
		CSerial serial;
		while (state != STATE_END) {
			if (!Evaluation) {
				if (!TAKEOFF&&(take_var>=TAKE_THLD))//if copter has not taken off from land and variance>0.01 comes up to 10 times
				{
					TAKEOFF = true;//take off
					LAND = false;
					serial.SentSerial('t',0.0000);
					serial.SentSerial('t', 0.0000);
					serial.SentSerial('t', 0.0000);
					serial.SentSerial('t', 0.0000);
					serial.SentSerial('t', 0.0000);
					serial.SentSerial('t', 0.0000);
					serial.SentSerial('t', 0.0000);
					take_var = 0;
					serialnum = 0;
				}
				else if (TAKEOFF&&land_var>=LAND_THLD)//if copter has taken off and  variance<0.005 comes up to 6 times
				{
					//serial.SentSerial(0,0.0000);
                    serial.SentSerial('l',0.0000);//sent land command 
					serial.SentSerial('l', 0.0000);//sent land command 
					serial.SentSerial('l', 0.0000);//sent land command 
					serial.SentSerial('l', 0.0000);//sent land command 
					serial.SentSerial('l', 0.0000);//sent land command 
					LAND = true;
					TAKEOFF = false;//ready for the next takeoff
					land_var = 0;
					serialnum = 0;
				}
				else if(TAKEOFF&&!LAND)//if the copter has taken off and not landed
				{
					if (mark[0] >= VALUE_THRESHOLD)//
					{
						mark[0] -= VALUE_MINUS;
						//evafile << '1' << '\t';
						serial.SentSerial('1',velocity);
						serialnum = 1;
					}//向上
					else if (mark[1] >= VALUE_THRESHOLD)
					{
						mark[1] -= VALUE_MINUS;
						//evafile << '2' << '\t';
						serial.SentSerial('2',velocity);
						serialnum = 2;
					}//向后
					else if (mark[2] >= VALUE_THRESHOLD)
					{
						mark[2] -= VALUE_MINUS;
						//evafile << '3' << '\t';
						serial.SentSerial('3',velocity);
						serialnum = 3;
					}//向左
					else if (mark[3] >= VALUE_THRESHOLD)
					{
						mark[3] -= VALUE_MINUS;
						//evafile << '4' << '\t';
						serial.SentSerial('4',velocity);
						serialnum = 4;
					}//向右
				}
				
			}
			else {
				if (mark[0] >= VALUE_THRESHOLD)//阈值策略，同一值连续出现三次及以上才发送指令
				{
					mark[0] -= VALUE_MINUS;
					serial.SentSerial('1',velocity);
					serialnum = 1;
				}//向上
				else if (mark[1] >= VALUE_THRESHOLD)
				{
					mark[1] -= VALUE_MINUS;
					serial.SentSerial('2',velocity);
					serialnum = 2;
				}//向右
				else if (mark[2] >= VALUE_THRESHOLD)
				{
					mark[2] -= VALUE_MINUS;
					serial.SentSerial('3',velocity);
					serialnum = 3;
				}//向左
				else if (mark[3] >= VALUE_THRESHOLD)
				{
					mark[3] -= VALUE_MINUS;
					serial.SentSerial('4',velocity);
					serialnum = 4;
				}//向后
			}
		}
	}
 
	//处理数据，判断目前实验处于哪一步骤，并给出相应处理
	void MatlabEng()
	{	
		//CSerial serial;
		int mat_index=-1;//放在函数里，函数的私有变量，防止其他程序修改此值
		critical=2;		
		Engine *ep; 
		ep=engOpen(NULL);
		engSetVisible(ep,true); 

		TCHAR	mat_path[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, mat_path);
		char buff[500];
		sprintf(buff,"cd('%s\\mat_file');",mat_path);
		engEvalString(ep,buff);//引擎执行字符串buff中的表达式，进入文件夹
		engEvalString(ep,"head");//执行head文件，都是初始定义性质处理
		//if(!IS_TRAIN)
			//engEvalString(ep,"playing");//执行playing，载入训练好的系数

		double *s; 
		mxArray *xx = mxCreateDoubleMatrix(1,DATA_CHANNEL+1, mxREAL); //创建大小为1x(DATA_CHANNEL+1)的双精度矩阵，都为实数
		mxArray *ss= mxCreateDoubleMatrix(1,3, mxREAL);		
		critical=0;

		while(state!=STATE_END)
		{
			if(data_index>mat_index)
			{	
				critical=2;
				mat_index++;
				s=mxGetPr(xx);//得到实部数据的指针，改变s相当于改变xx的值
				for(int i=0;i<DATA_CHANNEL;i++)
					s[i]=data[mat_index%DATA_LENGTH][i];//取data中一行的数据
				if(mat_index<SEGMENT)
					s[DATA_CHANNEL]=trigger[mat_index%DATA_LENGTH];
				else
					s[DATA_CHANNEL]=-1;
				engPutVariable(ep, "x",xx);//把xx指向的数据以x的变量名写入Matlab引擎空间
				engEvalString(ep,"data_add");
				ss=engGetVariable(ep, "output");//从引擎中获取变量
				s = mxGetPr(ss);
				signal = s[0]; velocity = s[1]; variance = s[2];
				//一个trial产生一个signal，signal是0，1，2，3
				if (Evaluation) {
					if (signal > 0 && signal < 5)
					{
						//trigger[0]=int(signal);//调试用
						mark[int(signal) - 1] += VALUE_POSITIVE;
						for (int i = 0; i < 4; i++) {
							if (mark[i] >= VALUE_NEGITIVE)
							{
								mark[i] -= VALUE_NEGITIVE;
							}
						}
					}
				}
				else {
						if (!TAKEOFF) {
							if (variance >= TAKEVAR) {
								take_var += VALUE_POSITIVE;
							}
							if (signal > 0 && take_var > 0)
							{
								take_var -= VALUE_NEGITIVE;
							}
						}
						else if (TAKEOFF&&!LAND) {
							if (signal > 0 && signal < 5)
							{
								//trigger[0]=int(signal);//调试用
								mark[int(signal) - 1] += VALUE_POSITIVE;
								for (int i = 0; i < 4; i++) {
									if (mark[i] >= VALUE_NEGITIVE)
									{
										mark[i] -= VALUE_NEGITIVE;
									}
								}
								if (variance < LANDVAR)
									land_var += VALUE_POSITIVE;
								else if (land_var>0)
									land_var -= VALUE_NEGITIVE;

							}
						}
				}
				critical=0;
				
			}
			if(data_index==mat_index)
				Sleep(10);//10ms
		}
		critical=2;
		mxDestroyArray(ss);
		mxDestroyArray(xx);

		engClose(ep);
		critical=0;
	}

};

static DWORD WINAPI FileThreadEntry(PVOID arg)
{		
	((CSVEP *)arg)->Write();
	return 0;
}

static DWORD WINAPI MatlabThreadEntry(PVOID arg)
{		
	((CSVEP *)arg)->MatlabEng();
	return 0;
}
 static DWORD WINAPI SerialportThreadEntry(PVOID arg)
{
	((CSVEP *)arg)->Serial();
	return 0;
}