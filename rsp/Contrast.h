#pragma once


// Contrast �Ի���

class Contrast : public CDialogEx
{
	DECLARE_DYNAMIC(Contrast)

public:
	Contrast(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Contrast();

// �Ի�������
	enum { IDD = IDD_DIALOG_CONSTRAST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
