#pragma once


// ERZH �Ի���

class ERZH : public CDialogEx
{
	DECLARE_DYNAMIC(ERZH)

public:
	ERZH(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ERZH();

// �Ի�������
	enum { IDD = IDD_DIALOG_ERZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMCustomdrawSliderErzh(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
