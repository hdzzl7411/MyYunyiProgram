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
//临界资源防卫
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
	char							nCounterIndex[MAXCOUNTER_LEN];                             //窗口号
	char							nStaffId[MAXSTAFFID_LEN];										//员工编号
	int							nCount;										//统计办理业务总数量
	int							nEvaType;									//评价类型
	int							nDiscontentCount;						//不满意统计数
	int							nDegree;										//满意统计数
	int							nVerysatisfy;								//非常满意统计数
	//unsigned char			strDate[11];									//业务发生日期
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
	char							nTicketNumber[MAXSTAFFID_LEN];							//票号
	char							nCounterIndex[MAXCOUNTER_LEN];                             //窗口号
	int							nTransactionStatus;						//业务状态
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

