#pragma once


// CConvolution �Ի���

class CConvolution : public CDialogEx
{
	DECLARE_DYNAMIC(CConvolution)

public:
	CConvolution(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConvolution();
	double temp[9];

// �Ի�������
	enum { IDD = IDD_DIALOG_JUANJI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedOk();
};
