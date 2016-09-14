// ProgressBar.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rsp.h"
#include "ProgressBar.h"
#include "afxdialogex.h"


// CProgressBar �Ի���

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


// CProgressBar ��Ϣ�������


BOOL CProgressBar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	cs=(CStatic*)GetDlgItem(IDC_STATIC_JDT);

	m_progress=(CProgressCtrl*)GetDlgItem(IDC_PROGRESS_JDT);
	m_progress->SendMessage(PBM_SETBARCOLOR,0,RGB(49,108,38));
	m_progress->ModifyStyleEx(WS_EX_STATICEDGE,0);
	m_progress->Invalidate(FALSE);
	m_progress->SetRange(0,100);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void  CProgressBar::SetMessage(const char* pszMsg)
{
	if (pszMsg != NULL)
	{
		m_strMessage = pszMsg;
		cs->SetWindowText(m_strMessage);
	}

}

//GDAL���ú���
bool  CProgressBar::SetPosition(double dPosition)
{
	m_dPosition = dPosition;
	m_progress->SetPos(min( 100u, (UINT)( m_dPosition*100.0 )));

	return true;
}

//�Լ���д������ú���
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

// ����GDAL�������ӿ�
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