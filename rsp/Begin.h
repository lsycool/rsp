#pragma once


// Begin �Ի���

class Begin : public CDialogEx
{
	DECLARE_DYNAMIC(Begin)

public:
	Begin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Begin();
	UINT m_nTimerID;//��¼��������
	UINT m_nTimerID1;//�ı䴰��͸����

// �Ի�������
	enum { IDD = IDD_DIALOG_BEGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(UINT lpszTemplateName, CWnd* pParentWnd = NULL);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
