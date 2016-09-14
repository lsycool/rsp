#pragma once


// ChangeDetect 对话框

class ChangeDetect : public CDialogEx
{
	DECLARE_DYNAMIC(ChangeDetect)

public:
	ChangeDetect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ChangeDetect();

// 对话框数据
	enum { IDD = IDD_DIALOG_CHANGED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonFormer();
	afx_msg void OnBnClickedButtonLatter();
	afx_msg void OnBnClickedButtonResult();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
