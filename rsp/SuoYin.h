#pragma once
extern int SIndex;

// SuoYin 对话框

class SuoYin : public CDialogEx
{
	DECLARE_DYNAMIC(SuoYin)

public:
	SuoYin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SuoYin();

// 对话框数据
	enum { IDD = IDD_DIALOG_SuoYin };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListColortable();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
