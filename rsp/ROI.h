#pragma once
#include "ColorSelectComboBox.h"
#include "ListCtrlEx.h"


// CROI 对话框

class CROI : public CDialogEx
{
	DECLARE_DYNAMIC(CROI)

public:
	CROI(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CROI();

// 对话框数据
	enum { IDD = IDD_DIALOG_ROI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	LRESULT OnItemChanged(WPARAM wParam,LPARAM lParam);
//	CVSListBox m_List_ctlMain;
	virtual BOOL OnInitDialog();
	CListCtrlEx m_list;
	afx_msg void OnBnClickedButton2();
};
