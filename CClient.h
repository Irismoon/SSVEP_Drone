#include "Head.h"
#include "SVEP.h"


#include "EmoStateDLL.h"
#include "edk.h"
#include "edkErrorCode.h"
#pragma comment(lib,"edk.lib")

EE_DataChannel_t targetChannelList[] = {
	ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7,
	ED_P7, ED_O1, ED_O2, ED_P8, ED_T8,
	ED_FC6, ED_F4, ED_F8, ED_AF4
};




static DWORD WINAPI RecvThreadEntry(PVOID arg);
class CClient
{
public:
	CSVEP*				data;
	HANDLE				m_hThread;			//收包线程句柄
	bool				is_TransportActive;	//是否在收包

	EmoEngineEventHandle mEvent;
	EmoStateHandle mState;
	DataHandle mDataHandle;
	double	m_pBody[150];

	CClient()
	{

		is_TransportActive = false;
		mEvent = EE_EmoEngineEventCreate();
		mState = EE_EmoStateCreate();
		for(int i=0;i<150;i++)
			m_pBody[i]=0;
	}
	~CClient()
	{
		if( mDataHandle )
		{
			EE_DataFree( mDataHandle );
			mDataHandle = NULL;
		}
		is_TransportActive=false;
		Sleep(100);
		if(m_hThread!=NULL) 
		{
			CloseHandle(m_hThread);
			m_hThread=NULL;
		}

		// Disconnect from the Emotiv Engine
		EE_EngineDisconnect();
		EE_EmoStateFree(mState);
		EE_EmoEngineEventFree(mEvent);
	}

	bool	Connect()
	{
		EE_EngineConnect();

		// Create a data handle for this session
		mDataHandle = EE_DataCreate();

		// Create a one second buffer (Should be enough)
		float time = 1;
		EE_DataSetBufferSizeInSec( time );


		// Connect to the first user
		int state	= 0;
		unsigned int userID = 0;
		bool readyToCollect = false;
		while ( !readyToCollect )
		{
			state = EE_EngineGetNextEvent( mEvent );

			if( state == EDK_OK )
			{
				EE_Event_t eventType = EE_EmoEngineEventGetType( mEvent );
				EE_EmoEngineEventGetUserId( mEvent, &userID );

				// See if we have a user connected or not
				if( eventType == EE_UserAdded )
					EE_DataAcquisitionEnable( userID, true );

				// See if we've removed a user or not
				if( eventType == EE_UserRemoved )
					EE_DataAcquisitionEnable( userID, false );

				// There is a user connected, we need to check the state
				if (eventType == EE_EmoStateUpdated)
				{
					EE_EmoEngineEventGetEmoState( mEvent, mState );

					// If we have a headset on, we're ready to go
					if( ES_GetHeadsetOn( mState ) )
						readyToCollect = true;
				}
			}
		}
		m_hThread=CreateThread(NULL,0,RecvThreadEntry,(PVOID)this,0,NULL);
		return true;
	}

	bool	Dis_Connect()
	{

		return true;
	}

	void	ReceiveData()
	{
		unsigned int userID = 0;
		// Check for updated states
		int state;
		is_TransportActive = true;

		// While both sockets are open
		while(is_TransportActive)
		{
			state= EE_EngineGetNextEvent( mEvent );

			// Verify that the updated states still allow us to collect data
			if( state == EDK_OK )
			{
				EE_Event_t eventType = EE_EmoEngineEventGetType( mEvent );
				EE_EmoEngineEventGetUserId( mEvent, &userID );



				// Check for User Removal
				if( eventType == EE_UserRemoved )
				{
					EE_DataAcquisitionEnable( userID, false );
					//cout << "Current Emotiv User removed!" << endl;
					return;
				}

				// User is connected, but state has been updated
				if (eventType == EE_EmoStateUpdated)
				{
					EE_EmoEngineEventGetEmoState( mEvent, mState );				
				}
			}
			// Update the system
			EE_DataUpdateHandle( 0, mDataHandle );

			// Find out how many samples have accrued
			unsigned int nSamplesTaken=0;
			EE_DataGetNumberOfSample( mDataHandle, &nSamplesTaken );

			if( nSamplesTaken != 0 )
			{
				double* data1_buff = new double[ nSamplesTaken ];
				double* data2_buff = new double[ nSamplesTaken ];
				double* data3_buff = new double[ nSamplesTaken ];
				double* data4_buff = new double[ nSamplesTaken ];
				double* data5_buff = new double[ nSamplesTaken ];
				double* data6_buff = new double[ nSamplesTaken ];
				double* data7_buff = new double[ nSamplesTaken ];
				double* data8_buff = new double[ nSamplesTaken ];
				// Allocate space and grab the samples for each channel对每一个通道分别获取采样
				/*EE_DataGet( mDataHandle, ED_O1, data1_buff, nSamplesTaken );
				EE_DataGet( mDataHandle, ED_O2, data2_buff, nSamplesTaken );*/
				EE_DataGet( mDataHandle, ED_AF3, data1_buff, nSamplesTaken );
				EE_DataGet( mDataHandle, ED_F7, data2_buff, nSamplesTaken );
				EE_DataGet( mDataHandle, ED_F3, data3_buff, nSamplesTaken );
				EE_DataGet( mDataHandle, ED_FC5, data4_buff, nSamplesTaken );

				EE_DataGet( mDataHandle, ED_AF4, data5_buff, nSamplesTaken );
				EE_DataGet( mDataHandle, ED_F8, data6_buff, nSamplesTaken );
				EE_DataGet( mDataHandle, ED_F4, data7_buff, nSamplesTaken );
				EE_DataGet( mDataHandle, ED_FC6, data8_buff, nSamplesTaken );
				for ( unsigned int sampleIdx = 0; sampleIdx < nSamplesTaken; sampleIdx++ )
				{
					m_pBody[9*sampleIdx+0]=data1_buff[ sampleIdx ];
					m_pBody[9*sampleIdx+1]=data2_buff[ sampleIdx ];
					m_pBody[9*sampleIdx+2]=data3_buff[ sampleIdx ];
					m_pBody[9*sampleIdx+3]=data4_buff[ sampleIdx ];
					m_pBody[9*sampleIdx+4]=data5_buff[ sampleIdx ];
					m_pBody[9*sampleIdx+5]=data6_buff[ sampleIdx ];
					m_pBody[9*sampleIdx+6]=data7_buff[ sampleIdx ];
					m_pBody[9*sampleIdx+7]=data8_buff[ sampleIdx ];
					m_pBody[9*sampleIdx+8]=0;
					
				}
				data->SetData(m_pBody,nSamplesTaken);

				// Clean up the data structure.
				unsigned long long* cq1 = reinterpret_cast< unsigned long long* >( data1_buff );			
				delete [] data1_buff;
				unsigned long long* cq2 = reinterpret_cast< unsigned long long* >( data2_buff );			
				delete [] data2_buff;
				unsigned long long* cq3 = reinterpret_cast< unsigned long long* >( data3_buff );			
				delete [] data3_buff;
				unsigned long long* cq4 = reinterpret_cast< unsigned long long* >( data4_buff );			
				delete [] data4_buff;
				unsigned long long* cq5 = reinterpret_cast< unsigned long long* >( data5_buff );			
				delete [] data5_buff;
				unsigned long long* cq6 = reinterpret_cast< unsigned long long* >( data6_buff );			
				delete [] data6_buff;
				unsigned long long* cq7 = reinterpret_cast< unsigned long long* >( data7_buff );			
				delete [] data7_buff;
				unsigned long long* cq8 = reinterpret_cast< unsigned long long* >( data8_buff );			
				delete [] data8_buff;
			}
			Sleep( 3 );//ms
		}
	}
};

static DWORD WINAPI RecvThreadEntry(PVOID arg)
{		
	srand( (unsigned)time( NULL ) );

	((CClient *)arg)->ReceiveData();
	return 0;
}