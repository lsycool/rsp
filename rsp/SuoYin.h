#pragma once
extern int SIndex;

// SuoYin �Ի���

class SuoYin : public CDialogEx
{
	DECLARE_DYNAMIC(SuoYin)

public:
	SuoYin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SuoYin();

// �Ի�������
	enum { IDD = IDD_DIALOG_SuoYin };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListColortable();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
