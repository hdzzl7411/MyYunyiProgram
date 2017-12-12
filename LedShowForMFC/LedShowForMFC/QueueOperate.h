#pragma once
#include <mq.h>

class CQueueOperate
{
public:
	CQueueOperate(void);
	~CQueueOperate(void);
public:
	HRESULT CreateMSMQQueue(LPWSTR wszPathName,	PSECURITY_DESCRIPTOR pSecurityDescriptor,LPWSTR wszOutFormatName, DWORD *pdwOutFormatNameLength);
	int OpenMyQueue(LPWSTR wszPathName,DWORD dwAccess,DWORD dwShareMode,QUEUEHANDLE *phQueue);
	HRESULT QueueSendMessage( WCHAR* wszQueueName, WCHAR* wszComputerName,WCHAR* wszMsg);
	HRESULT QueueSendMessage( char* szQueueName, char* szComputerName,char* strMsg);
	HRESULT ReadingDestQueue(WCHAR *wszQueueName,WCHAR *wszComputerName,WCHAR* wszQueueInfo);
	HRESULT ReadingDestQueue(char *szQueueName,char *szComputerName,char* szQueueInfo);
	char* WcharToChar(const wchar_t* wc);
	wchar_t* CharToWchar(char* in_str);
};

