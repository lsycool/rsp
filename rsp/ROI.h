#pragma once
#include "ColorSelectComboBox.h"
#include "ListCtrlEx.h"


// CROI �Ի���

class CROI : public CDialogEx
{
	DECLARE_DYNAMIC(CROI)

public:
	CROI(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CROI();

// �Ի�������
	enum { IDD = IDD_DIALOG_ROI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	LRESULT OnItemChanged(WPARAM wParam,LPARAM lParam);
//	CVSListBox m_List_ctlMain;
	virtual BOOL OnInitDialog();
	CListCtrlEx m_list;
	afx_msg void OnBnClickedButton2();
};
