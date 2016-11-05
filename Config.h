#pragma once
//Windows设置
#define     RefreshHz           75                  //屏幕刷新率，Hz
#define		DATA_CHANNEL		8					//通道
#define		DATA_TYPE			4                  //判断出的方向
#define		Fs					128
#define		DATA_LENGTH			int(Fs*3)			//缓冲池大小，SVEP每次存数据存3s的数据来分析
#define		SEGMENT				(Fs*160)		    //每次时长
#define		TRIAL				(Fs*5)				//每个trial的时长，一个TRIAL指的是某一闪烁方块闪烁的时长
#define		FRE_SEGMENT			int(Fs*0.9)			//每个trial的时长

/* */
#define		DIS_MODE			2					//1、显示视频;3、黑屏训练，这种情况只要直接画flash就好了，屏幕自动被填充为黑色
#define		WIN_WIDE			640			//窗口宽度
#define		WIN_HEIGHT			480 				//窗口高度

//#define		DIS_MODE			2					//2、虚拟环境
//#define		WIN_WIDE			1280				//窗口宽度
//#define		WIN_HEIGHT			1024				//窗口高度


//#define     IS_TRAIN			false				//true训练，false演示
#define     Evaluation          true                 //evaluation
#define		TRAIN_MOVE			false               //指训练的时候是否要对应实际的小车移动

#define		STATE_STOP			0
#define		STATE_PLAY			1
#define		STATE_REST			2
#define		STATE_END			3
#define		DEBUG				0 

#define KEYMODE false

//起飞设置
bool TAKEOFF = false;
#define     TAKEVAR       0.01
#define     TAKE_THLD     80//连续满足条件十次才起飞

//降落设置
bool LAND = false;
#define     LANDVAR       0.002
#define     LAND_THLD     100

#define		VALUE_POSITIVE		20
#define		VALUE_NEGITIVE		10
#define		VALUE_THRESHOLD		30
#define		VALUE_MINUS			10




//const int ch[8]={9,45,53,57,58,59,60,61};
const double freq[4]=			{12.5,9.37,8.33,6.8182};//刺激频率，显示器刷新频率是75HZ，75/6，/8,/9，/11
//const double freq[3]=			{37.5,25,18.75};

//const double period[3]=			{80,120,200};
const double period[4]=			{80,106.7236,120.048,146.666};//period=1/freq，闪烁一次耗时ms
//const double period[3]=			{26.6667,40,53.3333};
//const double c[3][15]={{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//					   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//					   {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	
const double c[4][15]={{1,1,1,0,0,0,0,0,0,0,0,0,0,0},
					   {1,1,1,1,0,0,0,0,0,0,0,0,0,0},
					   {1,1,1,1,1,0,0,0,0,0,0,0,0,0},
					   {1,1,1,1,1,1,0,0,0,0,0,0,0,0}};


#define		PI					3.1415926535
#define		PI_OVER				0.0174532925//角度转换为弧度的系数
#define		COM_PORT			"COM7"