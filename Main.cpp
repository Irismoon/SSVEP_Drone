#pragma once
#include "GL.h"
CGL vr;

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			//消息// Message For This Window
							WPARAM	wParam,			// 虚拟标识码Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{//每个窗口会有一个称为窗口过程的回调函数(WndProc)
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?屏保休息
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{////当用户关闭窗口，窗口销毁，程序需结束，发退出消息，以退出消息循环
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:				//某个键被按下			// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;		//wParam标识的虚拟键盘值为1			// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			vr.ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc,//其他消息交给由系统提供的缺省处理函数
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	vr.client.Connect();	                        //先与电极帽连接
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Lopo
	// 创建OpenGL窗口
	if (!vr.CreateGLWindow("BCI SYSTEM",WIN_WIDE,WIN_HEIGHT,32,!vr.fullscreen))
		return 0;											// 失败退出

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{//收到退出的消息
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if (active)										// 程序激活的么?
			{
				if (keys[VK_ESCAPE])						// ESC 按下了么?
				{
					done=TRUE;								// ESC 发出退出信号，没有消息按下ESC后退出循环
				}
				else										// 不是退出的时候，刷新屏幕
				{
					vr.DrawGLScene();//没有消息的时候就一直刷新屏幕重新画
											// 交换缓存 (双缓存)
				}
			}
			if(!vr.key_press())            //执行press函数
				return 0;                  //如果切换全屏重建窗口失败程序结束
		}
	}

	// 关闭程序
	

	return (msg.wParam);							// Exit The Program
}
