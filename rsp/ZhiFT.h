#pragma once
extern int nScreenWidthZ;
extern int nScreenHeightZ;

// ZhiFT �Ի���

class ZhiFT : public CDialogEx
{
	DECLARE_DYNAMIC(ZhiFT)

public:
	ZhiFT(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ZhiFT();

// �Ի�������
	enum { IDD = IDD_DIALOG_ZhiFangTu };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	bool ZhiFangTu(CDC* pDC);
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
