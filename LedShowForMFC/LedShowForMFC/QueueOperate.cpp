#include "StdAfx.h"
#include "QueueOperate.h"

#pragma comment(lib, "mqrt.lib")

CQueueOperate::CQueueOperate(void)
{
}


CQueueOperate::~CQueueOperate(void)
{
}
char* CQueueOperate::WcharToChar(const wchar_t* wc)
{
	char* m_char=NULL;
	delete m_char;
	m_char = NULL;
	int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}
wchar_t* CQueueOperate::CharToWchar(char* in_str)
{
	int nLen=::MultiByteToWideChar(CP_ACP,0,in_str,-1,NULL,0);
	if(nLen == 0)
	{
		return NULL;
	}
	wchar_t* out_str = new wchar_t[nLen];
	MultiByteToWideChar(CP_ACP,0,in_str,-1,out_str,nLen);
	return out_str;

}
HRESULT CQueueOperate::ReadingDestQueue(char *szQueueName,char *szComputerName,char* szQueueInfo)
{
	WCHAR* wszQueueName = CharToWchar(szQueueName);
	WCHAR* wszComputerName = CharToWchar(szComputerName);
	WCHAR readInfo[1024] = L"";
	HRESULT hr = ReadingDestQueue(wszQueueName,wszComputerName,readInfo);
	if(readInfo != NULL)
	{
		char* strTemp = WcharToChar(readInfo);
		memcpy(szQueueInfo,strTemp,strlen(strTemp));
		delete [] strTemp;
	}
	delete [] wszQueueName;
	delete [] wszComputerName;
	return hr;
}
HRESULT CQueueOperate::ReadingDestQueue(WCHAR *wszQueueName,WCHAR *wszComputerName,WCHAR* wszQueueInfo)
{
	const int NUMBEROFPROPERTIMES = 5;

	DWORD cPropId = 0;
	HRESULT hr = MQ_OK;
	HANDLE hQueue = NULL;
	ULONG ulBufferSize = 1024;

	MQMSGPROPS msgProps;
	MQPROPVARIANT aMsgPropVar[NUMBEROFPROPERTIMES];
	QUEUEPROPID aMsgPropId[NUMBEROFPROPERTIMES];
	HRESULT aMsgStatus[NUMBEROFPROPERTIMES];

	aMsgPropId[cPropId] = PROPID_M_LABEL_LEN;
	aMsgPropVar[cPropId].vt = VT_UI4;
	aMsgPropVar[cPropId].ulVal = MQ_MAX_MSG_LABEL_LEN;
	cPropId ++;

	WCHAR wszLabelBuffer[PROPID_M_LABEL_LEN];// = L"The_message_name";
	aMsgPropId[cPropId] = PROPID_M_LABEL_LEN;
	aMsgPropVar[cPropId].vt = VT_LPWSTR;
	aMsgPropVar[cPropId].pwszVal = wszLabelBuffer;
	cPropId ++;

	UCHAR * pucBodyBuffer = NULL;
	pucBodyBuffer = (UCHAR *)malloc(ulBufferSize);
	if(pucBodyBuffer == NULL)
	{
		return MQ_ERROR_INSUFFICIENT_RESOURCES;
	}
	memset(pucBodyBuffer,0,ulBufferSize);
	aMsgPropId[cPropId] = PROPID_M_BODY_SIZE;
	aMsgPropVar[cPropId].vt = VT_NULL;
	cPropId++ ;

	aMsgPropId[cPropId] = PROPID_M_BODY;
	aMsgPropVar[cPropId].vt = VT_VECTOR | VT_UI1;
	aMsgPropVar[cPropId].caub.pElems = (UCHAR *)pucBodyBuffer;
	aMsgPropVar[cPropId].caub.cElems = ulBufferSize;
	cPropId++;

	aMsgPropId[cPropId] = PROPID_M_BODY_TYPE;
	aMsgPropVar[cPropId].vt = VT_NULL;
	cPropId++;

	//Init
	msgProps.cProp = cPropId;
	msgProps.aPropID = aMsgPropId;
	msgProps.aPropVar = aMsgPropVar;
	msgProps.aStatus = aMsgStatus;


	WCHAR * wszFormatName = NULL;
	DWORD dwFormatNameLength = 0;
	//WCHAR *wszQueueName = L"..\\private$\\CdmLedC";
	//WCHAR *wszComputerName = L"WQ-20170413VKEC";
	dwFormatNameLength = wcslen(wszQueueName) + wcslen(wszComputerName) + 13;
	wszFormatName = new WCHAR[dwFormatNameLength];
	if(wszFormatName == NULL)
	{
		return MQ_ERROR_INSUFFICIENT_RESOURCES;
	}
	memset(wszFormatName,0,dwFormatNameLength);
	if(_snwprintf(wszFormatName,dwFormatNameLength - 1,	L"DIRECT=OS:%s\\%s",wszComputerName,wszQueueName) < 0)
	{
		//名称太长或有特殊字符
		return FALSE;
	}
	else
	{
		wszFormatName[dwFormatNameLength - 1] = L'\0';
	}
	hr = MQOpenQueue(wszFormatName,MQ_RECEIVE_ACCESS,MQ_DENY_NONE,&hQueue);
	if(wszFormatName)
	{
		delete [] wszFormatName;
	}
	if(FAILED(hr))
	{
		//读消息队列错误
		return hr;
	}
	for(;;)
	{
		aMsgPropVar[0].ulVal = MQ_MAX_MSG_LABEL_LEN;
		hr = MQReceiveMessage(hQueue,1000,MQ_ACTION_RECEIVE,&msgProps,NULL,NULL,NULL,MQ_NO_TRANSACTION);
		if(hr == MQ_ERROR_BUFFER_OVERFLOW)
		{
			ulBufferSize = aMsgPropVar[2].ulVal*sizeof(UCHAR);
			pucBodyBuffer = (UCHAR *)realloc(pucBodyBuffer,ulBufferSize);
			if(pucBodyBuffer == NULL)
			{
				return MQ_ERROR_INSUFFICIENT_RESOURCES;
			}
			memset(pucBodyBuffer,0,ulBufferSize);
			aMsgPropVar[3].caub.pElems = (UCHAR *)pucBodyBuffer;
			aMsgPropVar[3].caub.cElems = ulBufferSize;
			continue;
		}
		if(FAILED(hr))
		{
			//接收消息错误或没有消息,关闭队列
			wprintf(L"No messages. Closing queue\n");
			break;
		}
		if(msgProps.aPropVar[0].ulVal == 0)
		{
			//消息从队列里移除
			wprintf(L"Removed message from queue.\n");
		}
		else
		{
			wprintf(L"Removed message '%s' from queue.\n",wszLabelBuffer);
		}
		if(msgProps.aPropVar[4].ulVal == VT_BSTR)
		{
			wcsncpy(wszQueueInfo,(WCHAR*)pucBodyBuffer,1024);
			wprintf(L"Body:%s ",(WCHAR*)pucBodyBuffer);
			wprintf(L"\n");
			break;
		}
	}
	// Close the queue and free the memory allocated for the body buffer.  
	hr = MQCloseQueue(hQueue);  
	free(pucBodyBuffer);
	return hr;
}

HRESULT CQueueOperate::CreateMSMQQueue(LPWSTR wszPathName,PSECURITY_DESCRIPTOR pSecurityDescriptor,LPWSTR wszOutFormatName, DWORD *pdwOutFormatNameLength)
{
	// 定义队列最大属性. 
	const int NUMBEROFPROPERTIES = 2; 
	// 定义属性 
	MQQUEUEPROPS QueueProps; 
	MQPROPVARIANT aQueuePropVar[NUMBEROFPROPERTIES]; 
	QUEUEPROPID aQueuePropId[NUMBEROFPROPERTIES]; 
	HRESULT aQueueStatus[NUMBEROFPROPERTIES]; 
	HRESULT hr = MQ_OK; 
	// Validate the input parameters. 
	if (wszPathName == NULL || wszOutFormatName == NULL || pdwOutFormatNameLength == NULL)
	{
		return MQ_ERROR_INVALID_PARAMETER;

	} 
	// 设置属性 
	DWORD cPropId = 0; 
	aQueuePropId[cPropId] = PROPID_Q_PATHNAME; 
	aQueuePropVar[cPropId].vt = VT_LPWSTR; 
	aQueuePropVar[cPropId].pwszVal = wszPathName;

	cPropId++; 
	WCHAR wszLabel[MQ_MAX_Q_LABEL_LEN] = L"Have a Test Queue"; 
	aQueuePropId[cPropId] = PROPID_Q_LABEL; 
	aQueuePropVar[cPropId].vt = VT_LPWSTR; 
	aQueuePropVar[cPropId].pwszVal = wszLabel; 
	cPropId++; 
	// 初始化MQQUEUEPROPS 结构体. 
	QueueProps.cProp = cPropId; 	// Number of properties
	QueueProps.aPropID = aQueuePropId; // IDs of the queue properties 
	QueueProps.aPropVar = aQueuePropVar; // Values of the queue properties 
	QueueProps.aStatus = aQueueStatus; // Pointer to the return status 
	// 调用MQCreateQueue创建队列 
	WCHAR wszFormatNameBuffer[256]; 
	DWORD dwFormatNameBufferLength = sizeof(wszFormatNameBuffer)/sizeof(wszFormatNameBuffer[0]);
	hr = MQCreateQueue(pSecurityDescriptor, // Security descriptor 
		&QueueProps, // Address of queue property structure 
		wszFormatNameBuffer, // Pointer to format name buffer 
		&dwFormatNameBufferLength); // Pointer to receive the queue's format name length  
	if ( FAILED(hr) ) 
	{
		//fprintf( stderr, "Failed MQCreateQueue, error = 0x%x\n", hr ); 
		wprintf( L"MQCreateQueue error ret = 0x%x.\n", hr);
	} 
	// Return the format name if the queue is created successfully. 
	if (hr == MQ_OK || hr == MQ_INFORMATION_PROPERTY)
	{ 
		wprintf(L"The queue was created.\n"); 
		if (*pdwOutFormatNameLength >= dwFormatNameBufferLength) 
		{ 
			wcsncpy(wszOutFormatName, wszFormatNameBuffer, *pdwOutFormatNameLength - 1);
			wszOutFormatName[*pdwOutFormatNameLength - 1] = L'\0';
			*pdwOutFormatNameLength = dwFormatNameBufferLength;
		}  
		else 
		{ 
			wprintf(L"But its format name cannot be returned.\n");
		}
	}
	return hr;
}

//打开队列

int CQueueOperate::OpenMyQueue(LPWSTR wszPathName,	DWORD dwAccess,	DWORD dwShareMode,QUEUEHANDLE *phQueue)
{
	HRESULT hr = MQ_OK;
	//输入的变量参数无效
	if((wszPathName == NULL) || (phQueue == NULL))
	{
		return MQ_ERROR_INVALID_PARAMETER;
	}

	// Call MQPathNameToFormatName to obtain the format name required

	// to open the queue.

	DWORD dwFormatNameBufferLength = 256; // Length of the format name buffer

	WCHAR wszFormatNameBuffer[256]; // Format name buffer 
	hr = MQPathNameToFormatName(wszPathName, wszFormatNameBuffer, &dwFormatNameBufferLength); 
	if (FAILED(hr)) 
	{
		fprintf(stderr, "An error occurred in MQPathNameToFormatName (error: 0x%x).\n", hr);
		return hr;
	}
	// Call MQOpenQueue to open the queue with the access and 
	// share mode provided by the caller. 
	hr = MQOpenQueue( wszFormatNameBuffer, // Format name of the queue 
		dwAccess, // Access mode 
		dwShareMode, // Share mode 
		phQueue // OUT: Queue handle
		);
	if(FAILED(hr))
	{
		fprintf(stderr,"An error occurred in MQOpenQueue(error:0x%x.)\n",hr);
		return hr;
	}
	return hr;
}


//发消息
HRESULT CQueueOperate::QueueSendMessage( char* szQueueName, char* szComputerName,char* strMsg)
{
	WCHAR* wszQueueName = CharToWchar(szQueueName);
	WCHAR* wszComputerName = CharToWchar(szComputerName);
	WCHAR* wszMsg = CharToWchar(strMsg);
	HRESULT hr = QueueSendMessage(wszQueueName,wszComputerName,wszMsg);
	delete [] wszQueueName;
	delete [] wszComputerName;
	delete [] wszMsg;
	return hr;
}
HRESULT CQueueOperate::QueueSendMessage( WCHAR* wszQueueName, WCHAR* wszComputerName,WCHAR* wszMsg )
{
	if(wszQueueName == NULL || wszComputerName == NULL)
	{
		return MQ_ERROR_INVALID_PARAMETER;
	}
	const int NUMBEROFPROPERTIES = 5;
	DWORD cPropId = 0;
	HRESULT hr = MQ_OK;
	HANDLE hQueue = NULL;
	//定义结构
	MQMSGPROPS msgProps;
	MQPROPVARIANT aMsgPropVar[NUMBEROFPROPERTIES];
	QUEUEPROPID aMsgPropId[NUMBEROFPROPERTIES];
	HRESULT aMsgStatus[NUMBEROFPROPERTIES];
	//
	aMsgPropId[cPropId] = PROPID_M_LABEL; // Property ID //
	aMsgPropVar[cPropId].vt = VT_LPWSTR; // Type indicator //
	aMsgPropVar[cPropId].pwszVal = L" LEDShowInfo"; // The message body //
	cPropId++;

//	WCHAR wszMessageBody[] = L"The Message Body Test";
	aMsgPropId[cPropId] = PROPID_M_BODY; // Property ID 
	aMsgPropVar[cPropId].vt = VT_VECTOR | VT_UI1; // Type indicator 
	aMsgPropVar[cPropId].caub.pElems = (LPBYTE)wszMsg; 
	int nLen = WideCharToMultiByte(CP_ACP, 0, wszMsg, wcslen(wszMsg), NULL, 0, NULL, NULL);
	aMsgPropVar[cPropId].caub.cElems = nLen+4; //strlen(WcharToChar(wszMsg)); 
// 	aMsgPropVar[cPropId].caub.pElems = (LPBYTE)wszMessageBody; 
// 	aMsgPropVar[cPropId].caub.cElems = sizeof(wszMessageBody); 
	cPropId++; 
	DWORD dwBodyType = VT_BSTR;
	aMsgPropId[cPropId] = PROPID_M_BODY_TYPE; // Property ID 
	aMsgPropVar[cPropId].vt = VT_UI4; // Type indicator 
	aMsgPropVar[cPropId].ulVal = dwBodyType; 
	cPropId ++; 

	// Initialize the MQMSGPROPS structure.

	msgProps.cProp = cPropId; 
	msgProps.aPropID = aMsgPropId; 
	msgProps.aPropVar = aMsgPropVar; 
	msgProps.aStatus = aMsgStatus;
	// Create a direct format name for the queue. 
	WCHAR * wszFormatName = NULL; 
	DWORD dwBufferLength = 0; 
	dwBufferLength = wcslen(wszQueueName) + wcslen(wszComputerName) + 12;
	wszFormatName = new WCHAR[dwBufferLength]; 
	if (wszFormatName == NULL) 
	{
		return MQ_ERROR_INSUFFICIENT_RESOURCES; 
	} 
	memset(wszFormatName, 0, dwBufferLength*sizeof(WCHAR)); 
	if (_snwprintf( wszFormatName, 
		dwBufferLength - 1, 
		L"DIRECT=OS:%s\\%s",//原来使用L"DIRECT=OS:%s//%s"
		wszComputerName, 
		wszQueueName ) < 0)
	{
		wprintf(L"The format name is too long for the buffer specified.\n"); 
		return FALSE;
	}  
	else
	{
		wszFormatName[dwBufferLength - 1] = L'\0';
	}


	// Call MQOpenQueue to open the queue with send access. 
	hr = MQOpenQueue( wszFormatName, //原来使用wszFormatName // Format name of the queue
		MQ_SEND_ACCESS, // Access mode  
		MQ_DENY_NONE, // Share mode 
		&hQueue // OUT: Queue handle 
		); 
	delete [] wszFormatName; 
	// Handle any error returned by MQOpenQueue. 
	if (FAILED(hr)) {
		fprintf( stderr, "Failed MQSendMessage, MQOpenQueue ret error = 0x%x\n ", hr ); 
		return hr;
	} 
	// Call MQSendMessage to send the message to the queue. 
	hr = MQSendMessage(hQueue, // Queue handle 
		&msgProps, // Message property structure 
		MQ_NO_TRANSACTION // Not in a transaction 
		);  
	if (FAILED(hr))
	{
		MQCloseQueue(hQueue); 
		fprintf( stderr, "Failed MQSendMessage, error = 0x%x\n", hr );
	}
	printf("MQSendMessage ret = 0x%x\n",hr);
	hr = MQCloseQueue(hQueue); 
	return hr;

}  