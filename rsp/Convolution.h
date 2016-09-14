#pragma once


// CConvolution 对话框

class CConvolution : public CDialogEx
{
	DECLARE_DYNAMIC(CConvolution)

public:
	CConvolution(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConvolution();
	double temp[9];

// 对话框数据
	enum { IDD = IDD_DIALOG_JUANJI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedOk();
};
