#pragma once

#ifndef STD_API  
#define STD_API __stdcall  
#endif 

class CProcessBase  
{
public:
	CProcessBase() 
	{
		m_dPosition = 0.0;
		m_iStepCount = 100;
		m_iCurStep = 0;
		m_bIsContinue = true;
	}
	virtual ~CProcessBase() {}
	virtual void SetMessage(const char* pszMsg) = 0;
	virtual bool SetPosition(double dPosition) = 0;
	virtual bool StepIt() = 0;

	virtual void SetStepCount(int iStepCount)
	{
		ReSetProcess();	
		m_iStepCount = iStepCount;
	}
	CString GetMessage()
	{
		return m_strMessage;
	}
	double GetPosition()
	{
		return m_dPosition;
	}
	void ReSetProcess()
	{
		m_dPosition = 0.0;
		m_iStepCount = 100;
		m_iCurStep = 0;
		m_bIsContinue = true;
	}
public:
	CString m_strMessage;/*! ������Ϣ */
	double m_dPosition;/*! ����ֵ */
	int m_iStepCount;/*! ���ȸ��� */
	int m_iCurStep;/*! ���ȵ�ǰ���� */
	bool m_bIsContinue;/*! �Ƿ�ȡ����ֵΪfalseʱ��ʾ����ȡ�� */
};



// CProgressBar �Ի���

class CProgressBar : public CDialogEx,public CProcessBase
{
	DECLARE_DYNAMIC(CProgressBar)

public:
	CProgressBar(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProgressBar();
	void SetMessage(const char* pszMsg);
	bool SetPosition(double dPosition);
	bool StepIt();
	void updateProgress(int);
// �Ի�������
	enum { IDD = IDD_DIALOG_JDT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	CProgressCtrl* m_progress;
	DECLARE_MESSAGE_MAP()
public:
	CStatic* cs;
	virtual BOOL OnInitDialog();
	//dfComplete ��ɽ���ֵ����ȡֵΪ 0.0 �� 1.0 ֮�� ;pszMessage ������Ϣ ;pProgressArg   CProcessBase��ָ�� 
};

int STD_API ALGTermProgress( double dfComplete, const char *pszMessage, void * pProgressArg );
