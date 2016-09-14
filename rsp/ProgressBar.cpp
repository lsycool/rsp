// ProgressBar.cpp : 实现文件
//

#include "stdafx.h"
#include "rsp.h"
#include "ProgressBar.h"
#include "afxdialogex.h"


// CProgressBar 对话框

IMPLEMENT_DYNAMIC(CProgressBar, CDialogEx)

CProgressBar::CProgressBar(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProgressBar::IDD, pParent)
{
	m_dPosition = 0.0;
	m_iStepCount = 100;
	m_iCurStep = 0;
}

CProgressBar::~CProgressBar()
{
	
}

void CProgressBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProgressBar, CDialogEx)
END_MESSAGE_MAP()


// CProgressBar 消息处理程序


BOOL CProgressBar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	cs=(CStatic*)GetDlgItem(IDC_STATIC_JDT);

	m_progress=(CProgressCtrl*)GetDlgItem(IDC_PROGRESS_JDT);
	m_progress->SendMessage(PBM_SETBARCOLOR,0,RGB(49,108,38));
	m_progress->ModifyStyleEx(WS_EX_STATICEDGE,0);
	m_progress->Invalidate(FALSE);
	m_progress->SetRange(0,100);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void  CProgressBar::SetMessage(const char* pszMsg)
{
	if (pszMsg != NULL)
	{
		m_strMessage = pszMsg;
		cs->SetWindowText(m_strMessage);
	}

}

//GDAL调用函数
bool  CProgressBar::SetPosition(double dPosition)
{
	m_dPosition = dPosition;
	m_progress->SetPos(min( 100u, (UINT)( m_dPosition*100.0 )));

	return true;
}

//自己所写程序调用函数
bool  CProgressBar::StepIt()
{
	m_iCurStep ++;
	m_dPosition = m_iCurStep*1.0 / m_iStepCount;
	m_progress->SetPos(min( 100u, (UINT)( m_dPosition*100.0 ) ) );

	return true;
}

void  CProgressBar::updateProgress(int step)
{
	m_progress->SetPos(step);
}

// 调用GDAL进度条接口
int STD_API ALGTermProgress( double dfComplete, const char *pszMessage, void * pProgressArg )
{
	if(pProgressArg != NULL)
	{
		CProgressBar * pProcess = (CProgressBar*) pProgressArg;
		pProcess->m_bIsContinue = pProcess->SetPosition(dfComplete);

		TRY {
			pProcess->SetMessage(pszMessage);
		}
		CATCH (CMemoryException, e)
		{
			
		}
		END_CATCH

		if(pProcess->m_bIsContinue)
			return TRUE;
		else
			return FALSE;
	}
	else
		return false;
}