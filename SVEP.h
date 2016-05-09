#pragma once
#include "Head.h"

static DWORD WINAPI FileThreadEntry(PVOID arg);
static DWORD WINAPI MatlabThreadEntry(PVOID arg);
static DWORD WINAPI PlaySoundEntry(PVOID arg);

class CSVEP
{
	//����SSVEP������
public:
	int		data[DATA_LENGTH][DATA_CHANNEL];
	int		data_index;
	//matlab��C++����ʱ��������,0-��û������1-C++�ڲ�����2-matlab�ڲ���
	int		critical;

	//����C++��matlab֮�䴫����,
	//C++->matlab:��ʱ���ǩ�ͱ��
	//matlab->C++:��ʵʱ���ؽ��
	double	signal;				
	HANDLE	m_hThread_file;							//�����߳̿���			
	HANDLE	m_hThread_mat;							//�����߳̿���

	int		mark_mat;
	char	file_mat[100];
	int		state;                                 //SVEP����״̬��play/stop/end/rest��ͨ���ո�������л�
	int		mark[DATA_TYPE];						//���ڱ�ǿ���״̬���жϵķ���
	int		trigger[DATA_LENGTH];
	ofstream	evafile;
	LARGE_INTEGER tstart, tend, fequency;//64λ�з������� 

	CSVEP()
	{		
		data_index=-1;
		mark_mat=-1;
		signal=-1;
		state=STATE_STOP;
		if (Evaluation) evafile.open("evaluation.txt");
		QueryPerformanceFrequency(&fequency);
		for(int i=0;i<DATA_TYPE;i++)
			mark[i]=0;
		m_hThread_file=CreateThread(NULL,0,FileThreadEntry,(PVOID)this,0,NULL);	
		m_hThread_mat=CreateThread(NULL,0,MatlabThreadEntry,(PVOID)this,0,NULL);		
	}

	~CSVEP()
	{
		state=STATE_END;
		Sleep(1000);
		if (DEBUG) evafile.close();
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
	}

	void SetData(double *p,int nBlockPnts)
	{//�����ݽ�data
		for (int i=0;i<nBlockPnts;i++)
		{
			if(state==STATE_PLAY)
			{
				data_index++;
				trigger[data_index%DATA_LENGTH]=trigger[(data_index-1+DATA_LENGTH)%DATA_LENGTH];
				if(IS_TRAIN)
				{
					if (data_index%TRIAL == 0)//��ǩ����TRIALΪ��λ�仯��
					{
						trigger[data_index%DATA_LENGTH] = rand() % DATA_TYPE;
					}
					///���һ��trial�����ˣ�trigger���ֵ�ı䣬����������һ�еı��ֵ
					if((data_index+1)%(SEGMENT)==0)//һ��ѵ�����������˳������ˣ�����Ҳ��ʧ
						state=STATE_REST;
				}
				if (!IS_TRAIN & Evaluation) {
					if (data_index%TRIAL == 0)//��ǩ����TRIALΪ��λ�仯��
					{
						trigger[data_index%DATA_LENGTH] = rand() % DATA_TYPE;
						evafile << '\n' << trigger[data_index%DATA_LENGTH] << '\t';
						switch (trigger[data_index%DATA_LENGTH]) {
						case 0:
						PlaySound("up.wav", NULL, SND_ASYNC);
						QueryPerformanceCounter(&tstart);
						evafile << ((double)tstart.QuadPart) * 1000 / (double)fequency.QuadPart << '\t';
						break;
						case 1:
						PlaySound("right.wav", NULL, SND_ASYNC);
						QueryPerformanceCounter(&tstart);
						evafile << ((double)tstart.QuadPart) * 1000 / (double)fequency.QuadPart << '\t';
						break;
						case 2:
						PlaySound("left.wav", NULL, SND_ASYNC);
						QueryPerformanceCounter(&tstart);
						evafile << ((double)tstart.QuadPart)*1000/(double)fequency.QuadPart << '\t';
						break;
						case 3:
						PlaySound("down.wav", NULL, SND_ASYNC);
						QueryPerformanceCounter(&tstart);
						evafile << ((double)tstart.QuadPart) * 1000 / (double)fequency.QuadPart << '\t';
						break;
						}
					}
					if ((data_index + 1) % (SEGMENT) == 0)
						state = STATE_REST;
				}
				for (int j=0;j<DATA_CHANNEL;j++)
				{
					data[data_index%DATA_LENGTH][j]=*p;
					p++;//һ����һ��һ���ش�
				}
				p++;//����һ��

			}
		}
	}

	void Write()
	{//�����ݱ������ļ�
		char	file_date[100];
		char	file_cnt[100];
		char	file_mark[100];

		//�ļ���¼
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
		sprintf(fn,"md data-%s",file_date);system(fn);//��fn���ļ���
		sprintf(file_cnt,"data-%s\\data_cnt-%s.txt",file_date,file_date);//�ļ�
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
 
	//�������ݣ��ж�Ŀǰʵ�鴦����һ���裬��������Ӧ����
	void MatlabEng()
	{	
		int mat_index=-1;
		critical=2;		
		Engine *ep; 
		ep=engOpen(NULL);
		engSetVisible(ep,true); 

		TCHAR	mat_path[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, mat_path);
		char buff[500];
		sprintf(buff,"cd('%s\\mat_file');",mat_path);
		engEvalString(ep,buff);//����ִ���ַ���buff�еı��ʽ�������ļ���
		engEvalString(ep,"head");//ִ��head�ļ������ǳ�ʼ�������ʴ���
		if(!IS_TRAIN)
			engEvalString(ep,"playing");//ִ��playing������ѵ���õ�ϵ��

		double *s;
		mxArray *xx = mxCreateDoubleMatrix(1,DATA_CHANNEL+1, mxREAL); //������СΪ1x(DATA_CHANNEL+1)��˫���Ⱦ��󣬶�Ϊʵ��
		mxArray *ss= mxCreateDoubleMatrix(1,1, mxREAL);		
		critical=0;

		while(state!=STATE_END)
		{
			if(data_index>mat_index)
			{	
				critical=2;
				mat_index++;
				s=mxGetPr(xx);//�õ�ʵ�����ݵ�ָ�룬�ı�s�൱�ڸı�xx��ֵ
				for(int i=0;i<DATA_CHANNEL;i++)
					s[i]=data[mat_index%DATA_LENGTH][i];//ȡdata��һ�е�����
				if(mat_index<SEGMENT)
					s[DATA_CHANNEL]=trigger[mat_index%DATA_LENGTH];
				else
					s[DATA_CHANNEL]=-1;
				engPutVariable(ep, "x",xx);//��xxָ���������x�ı�����д��Matlab����ռ�
				engEvalString(ep,"data_add");
				ss=engGetVariable(ep, "signal");//�������л�ȡ����
				s=mxGetPr(ss);//
				signal=s[0];//һ��trial����һ��signal��signal��0��1��2��3
				if(signal>=0&&signal<4&&!IS_TRAIN)
				{
					trigger[0]=int(signal);//������
					mark[int(signal)]+=VALUE_POSITIVE;	
					for(int i=0;i<4;i++)
						if(mark[i]>=VALUE_NEGITIVE)
						mark[i]-=VALUE_NEGITIVE;
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