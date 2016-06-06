#pragma once
#include "Head.h"

static DWORD WINAPI CaptureThreadEntry(PVOID arg);

class CVideo
{
public:
	//…„œÒÕ∑œ‡πÿ
	struct  SimpleCapParams capture_temp,capture;
	int		device;
	bool	is_capture;
	bool    is_updated;
	bool	is_flash;

	CVideo()
	{
		is_updated=true;
		is_capture=true;
		device = setupESCAPI();
		if (device < 1)is_capture=false;
		if(is_capture)
		{
			if (device > 1)	
				device = 0;
			else
				device = 0;
			capture_temp.mWidth = 1200;
			capture_temp.mHeight = 576;
			capture_temp.mTargetBuf = new int[capture_temp.mWidth*capture_temp.mHeight];

			capture.mWidth = 1200;
			capture.mHeight = 576;
			capture.mTargetBuf = new int[capture.mWidth*capture.mHeight];
			initCapture(device,&capture_temp);
			doCapture(device);
			CreateThread(NULL,0,CaptureThreadEntry,(PVOID)this,0,NULL);
		}	
	}

	~CVideo()
	{
		deinitCapture(0);
	}

	void Capture()
	{
		while(is_capture)
		{
			if (isCaptureDone(device))
			{
				for (int i = 0; i < capture_temp.mWidth*capture_temp.mHeight; i++)
					capture_temp.mTargetBuf[i] = (capture_temp.mTargetBuf[i] & 0xff00ff00) |
					((capture_temp.mTargetBuf[i] & 0xff) << 16) |
					((capture_temp.mTargetBuf[i] & 0xff0000) >> 16);
			memcpy(capture.mTargetBuf , capture_temp.mTargetBuf, capture_temp.mWidth*capture_temp.mHeight*sizeof(int));	
			//Sleep(50);
			is_updated=true;
			}		
			doCapture(device);
			Sleep(40);			
		}
	}
};


static DWORD WINAPI CaptureThreadEntry(PVOID arg)
{
	if(DIS_MODE==1)
		((CVideo *)arg)->Capture();
	return 0;
}