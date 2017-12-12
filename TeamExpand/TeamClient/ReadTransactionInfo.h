#include "Markup.h"
#include <string>
#include <list>
#define  MAXSTAFFID_LEN		6
#define  MAXCOUNTER_LEN     3
using namespace std;
class CNetLock
{
public:
	CNetLock ( )
	{
		InitializeCriticalSection ( &m_CritSect );
	}

	~CNetLock ( )
	{
		DeleteCriticalSection ( &m_CritSect );
	}

	void Lock ( )
	{
		EnterCriticalSection ( &m_CritSect );
	}

	void Unlock ( )
	{
		LeaveCriticalSection ( &m_CritSect );
	}

private:
	CRITICAL_SECTION m_CritSect;
};
//�ٽ���Դ����
class CGuard
{
private:
	CNetLock& oracle;

public:
	CGuard ( CNetLock& lock ) : oracle ( lock )
	{
		oracle.Lock ( );
	}
	~CGuard ( )
	{
		oracle.Unlock ( );
	}
};
#pragma pack(1)
typedef struct tagEvaluation
{
	char							nCounterIndex[MAXCOUNTER_LEN];                             //���ں�
	char							nStaffId[MAXSTAFFID_LEN];										//Ա�����
	int							nCount;										//ͳ�ư���ҵ��������
	int							nEvaType;									//��������
	int							nDiscontentCount;						//������ͳ����
	int							nDegree;										//����ͳ����
	int							nVerysatisfy;								//�ǳ�����ͳ����
	//unsigned char			strDate[11];									//ҵ��������
	tagEvaluation()
	{
		memcpy(nCounterIndex, "", MAXCOUNTER_LEN);
		memcpy(nStaffId, "", MAXSTAFFID_LEN);
		nCount = 0;
		nEvaType = -1;
		nDiscontentCount = 0;
		nDegree = 0;
		nVerysatisfy = 0;
	};
}CSEvaluation, *LPCSEvaluation;
typedef struct tagRuningInfo 
{
	char							nTicketNumber[MAXSTAFFID_LEN];							//Ʊ��
	char							nCounterIndex[MAXCOUNTER_LEN];                             //���ں�
	int							nTransactionStatus;						//ҵ��״̬
	tagRuningInfo()
	{
		memcpy(nCounterIndex, "", MAXCOUNTER_LEN);
		memcpy(nTicketNumber, "", MAXSTAFFID_LEN);
		nTransactionStatus = 0;
	};
}CSRuningInfo,*LPCSRuningInfo;
#pragma pack()
#pragma once
class CReadTransactionInfo
{
public:
	CReadTransactionInfo ( );
	virtual ~CReadTransactionInfo ( );
	static CReadTransactionInfo& GetInstance ( )
	{
		static CReadTransactionInfo obj;
		return obj;
	}
public:
	void Initialize ( string strPath );
	void InitEvaluation ();
	void InitRuningInfo ();
	void AddEvaluationToList ( CSEvaluation csEva );
	void AddRunInfoToList ( CSRuningInfo csRun );
	std::list<CSEvaluation> GetEvaluationList ( )
	{
		return m_EvaInfo; 
	}
	std::list<CSRuningInfo> GetRunInfoList ( )
	{
		return m_RunInfo;
	}
	void ClearListInfo()
	{
		m_EvaInfo.clear();
		m_RunInfo.clear();
	}
private:
	CMarkup                       m_markup;
	list<CSEvaluation>			m_EvaInfo;
	CNetLock							m_EvaInfoLock;
	list<CSRuningInfo>		m_RunInfo;
	CNetLock							m_RunInfoLock;
};
