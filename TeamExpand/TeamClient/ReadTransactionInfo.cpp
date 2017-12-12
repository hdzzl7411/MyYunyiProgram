#include "stdafx.h"
#include "ReadTransactionInfo.h"

CReadTransactionInfo::CReadTransactionInfo ( )
{}


CReadTransactionInfo::~CReadTransactionInfo ( )
{}

void CReadTransactionInfo::Initialize ( string strPath )
{
	if ( !m_markup.Load ( strPath.c_str() ) )
	{
		//MessageBox ( NULL, _T("读取信息文件失败，请检查信息文件是否存在"), _T("提示"), MB_OK );
		return;
	}
	// 查找根节点
	if ( !m_markup.FindElem ( ) )
	{
		//MessageBox ( NULL, _T("解析配置文件根节点失败"), _T("提示"), MB_OK );
		return;
	}
	if ( !m_markup.IntoElem ( ) )
	{
		//MessageBox ( NULL, _T("进入配置文件根节点失败"), _T("提示"), MB_OK );
		return;
	}
	return;
}

void CReadTransactionInfo::InitEvaluation ( )
{
	CGuard a ( m_EvaInfoLock );
	m_EvaInfo.clear ( );

	string strTagName = _T("");
	while ( m_markup.FindElem ( ) )
	{
		strTagName = m_markup.GetTagName ( );
		if ( "TransactionTable" == strTagName )
		{
			if ( m_markup.IntoElem ( ) )
			{
				string strTagNameTmp = _T ( "" );
				string strCounter = _T ( "" );
				string strStaff = _T ( "" );
				string strEva = _T ( "" );
				string strTime = _T ( "" );
				while ( m_markup.FindElem ( ) )
				{
					strTagNameTmp = m_markup.GetTagName ( );
					if ( "CounterIndex" == strTagNameTmp )
					{
						strCounter = m_markup.GetData ( );
					}
					else if ( "StaffId" == strTagNameTmp )
					{
						strStaff = m_markup.GetData ( );
					}
					else if ( "EvaluationType" == strTagNameTmp )
					{
						strEva = m_markup.GetData ( );
					}
					else if ( "GetNumberTime" == strTagNameTmp )
					{
						CString strTmp = m_markup.GetData ( );
						strTime = strTmp.Left ( 10 );
					}
					
				}
				CTime t = CTime::GetCurrentTime ( ); //获取系统日期
				int d = t.GetDay ( ); //获得几号
				int y = t.GetYear ( ); //获取年份
				int m = t.GetMonth ( ); //获取当前月份

				char szTemp[25] = { 0 };
				sprintf_s ( szTemp, 11, "%4d-%02d-%02d", y,m,d );
				if ( strTime == szTemp )
				{
					CSEvaluation csEva;
					memset(csEva.nCounterIndex, 0, MAXCOUNTER_LEN);
					memset(csEva.nStaffId, 0, MAXSTAFFID_LEN);
					memcpy(csEva.nCounterIndex, strCounter.c_str(),strCounter.length());
					memcpy(csEva.nStaffId , strStaff.c_str(),strStaff.length());
					csEva.nEvaType = atoi (strEva.c_str() );
					csEva.nCount = 0;
					csEva.nDegree = 0;
					csEva.nVerysatisfy = 0;
					csEva.nDiscontentCount = 0;
					//memcpy ( csEva.strDate, strTime.c_str(), strTime.length() );
					AddEvaluationToList ( csEva );
				}
				m_markup.OutOfElem ( );
			}
		}
	}

}
void CReadTransactionInfo::InitRuningInfo ( )
{
	CGuard a ( m_RunInfoLock );
	m_RunInfo.clear ( );

	string strTagName = _T("");
	while ( m_markup.FindElem ( ) )
	{
		strTagName = m_markup.GetTagName ( );
		if ( "TransactionTable" == strTagName )
		{
			if ( m_markup.IntoElem ( ) )
			{
				string strTagNameTmp = _T("");
				string strCounter = _T("");
				string strTickNum = _T("");
				string strTransactionStatus = _T("");
				string strTime = _T("");
				while ( m_markup.FindElem ( ) )
				{
					strTagNameTmp = m_markup.GetTagName ( );
					if ( "CounterIndex" == strTagNameTmp )
					{
						strCounter = m_markup.GetData ( );
					}
					else if ( "TicketNumber" == strTagNameTmp )
					{
						strTickNum = m_markup.GetData ( );
					}
					else if ( "TransactionStatus" == strTagNameTmp )
					{
						strTransactionStatus = m_markup.GetData ( );
					}
					else if ( "GetNumberTime" == strTagNameTmp )
					{
						CString strTmp = m_markup.GetData ( );
						strTime = strTmp.Left ( 10 );
					}
					
				}
				CTime t = CTime::GetCurrentTime ( ); //获取系统日期
				int d = t.GetDay ( ); //获得几号
				int y = t.GetYear ( ); //获取年份
				int m = t.GetMonth ( ); //获取当前月份

				char szTemp[25] = { 0 };
				sprintf_s ( szTemp, 11, "%4d-%02d-%02d", y,m,d );
				if ( strTime == szTemp )
				{
					CSRuningInfo csRun;
					memset(csRun.nCounterIndex, 0, MAXCOUNTER_LEN);
					memset(csRun.nTicketNumber, 0, MAXSTAFFID_LEN);
					memcpy(csRun.nCounterIndex ,  strCounter.c_str(),strCounter.length());
					memcpy(csRun.nTicketNumber , strTickNum.c_str(),strTickNum.length());
					csRun.nTransactionStatus = atoi (strTransactionStatus.c_str() );
					AddRunInfoToList ( csRun );
				}
				m_markup.OutOfElem ( );
			}
		}
	}
}

void CReadTransactionInfo::AddEvaluationToList ( CSEvaluation csEva )
{
	BOOL bExist = FALSE;
	for ( list<CSEvaluation>::iterator EvaIt = m_EvaInfo.begin ( ); EvaIt != m_EvaInfo.end ( ); EvaIt++ )
	{
		if ( atoi(EvaIt->nCounterIndex) == atoi(csEva.nCounterIndex) )
		{
			EvaIt->nCount++;
			if ( csEva.nEvaType == 1 )
			{
				EvaIt->nDiscontentCount++;
			}
			else if ( csEva.nEvaType == 2 || csEva.nEvaType == 3 )
			{
				EvaIt->nDegree++;
			}
			else if ( csEva.nEvaType == 4 )
			{
				EvaIt->nVerysatisfy++;
			}
			bExist = TRUE;
		}
	}
	if ( !bExist )
	{
		csEva.nCount  = 1;
		if ( csEva.nEvaType == 1 )
		{
			csEva.nDiscontentCount++;
		}
		else if ( csEva.nEvaType == 2 || csEva.nEvaType == 3 )
		{
			csEva.nDegree++;
		}
		else if ( csEva.nEvaType == 4 )
		{
			csEva.nVerysatisfy++;
		}
		m_EvaInfo.push_back ( csEva );
	}
}

void CReadTransactionInfo::AddRunInfoToList ( CSRuningInfo csRun )
{
	BOOL bExist = FALSE;
	for ( list<CSRuningInfo>::iterator RunIt = m_RunInfo.begin ( ); RunIt != m_RunInfo.end ( ); RunIt++ )
	{
		if ( atoi(RunIt->nCounterIndex) == atoi(csRun.nCounterIndex) )
		{
			if ( csRun.nTransactionStatus == 1 )         //处于呼叫办理状态，更新票号
			{
				memcpy(RunIt->nCounterIndex, csRun.nCounterIndex, MAXCOUNTER_LEN);
				memcpy(RunIt->nTicketNumber, csRun.nTicketNumber, MAXSTAFFID_LEN);
			}
			bExist = TRUE;
		}
	}
	if ( !bExist )
	{
		if ( m_RunInfo.size ( ) > 8 )
		{
			m_RunInfo.pop_front ( );
		}
		if (atoi(csRun.nCounterIndex) > 0 || csRun.nTransactionStatus == 1)
			m_RunInfo.push_back ( csRun );
	}
}
