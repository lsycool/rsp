#pragma once
extern int BN[];

// BandSythesis 对话框

class BandSythesis : public CDialogEx
{
	DECLARE_DYNAMIC(BandSythesis)

public:
	BandSythesis(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~BandSythesis();

// 对话框数据
	enum { IDD = IDD_DIALOG_BandSy };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListBand();
	afx_msg void OnBnClickedOk();

	CListBox * ListBox;
	afx_msg void OnBnClickedCancel();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};
