#pragma once


// Begin 对话框

class Begin : public CDialogEx
{
	DECLARE_DYNAMIC(Begin)

public:
	Begin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Begin();
	UINT m_nTimerID;//记录窗口析构
	UINT m_nTimerID1;//改变窗口透明度

// 对话框数据
	enum { IDD = IDD_DIALOG_BEGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(UINT lpszTemplateName, CWnd* pParentWnd = NULL);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
