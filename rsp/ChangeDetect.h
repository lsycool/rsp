#pragma once


// ChangeDetect �Ի���

class ChangeDetect : public CDialogEx
{
	DECLARE_DYNAMIC(ChangeDetect)

public:
	ChangeDetect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ChangeDetect();

// �Ի�������
	enum { IDD = IDD_DIALOG_CHANGED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonFormer();
	afx_msg void OnBnClickedButtonLatter();
	afx_msg void OnBnClickedButtonResult();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
