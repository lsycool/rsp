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
	CString m_strMessage;/*! 进度信息 */
	double m_dPosition;/*! 进度值 */
	int m_iStepCount;/*! 进度个数 */
	int m_iCurStep;/*! 进度当前个数 */
	bool m_bIsContinue;/*! 是否取消，值为false时表示计算取消 */
};



// CProgressBar 对话框

class CProgressBar : public CDialogEx,public CProcessBase
{
	DECLARE_DYNAMIC(CProgressBar)

public:
	CProgressBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProgressBar();
	void SetMessage(const char* pszMsg);
	bool SetPosition(double dPosition);
	bool StepIt();
	void updateProgress(int);
// 对话框数据
	enum { IDD = IDD_DIALOG_JDT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CProgressCtrl* m_progress;
	DECLARE_MESSAGE_MAP()
public:
	CStatic* cs;
	virtual BOOL OnInitDialog();
	//dfComplete 完成进度值，其取值为 0.0 到 1.0 之间 ;pszMessage 进度信息 ;pProgressArg   CProcessBase的指针 
};

int STD_API ALGTermProgress( double dfComplete, const char *pszMessage, void * pProgressArg );
