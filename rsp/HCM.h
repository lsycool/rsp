#pragma once
#include <vector>
using namespace std;

struct Hclass//ÿ��Ԫ��
{
	BYTE data;//��������
	int mark;
};

struct Hsamp//��
{
	vector<Hclass*>sample;//�ϲ���
};
// HCM �Ի���

class HCM : public CDialogEx
{
	DECLARE_DYNAMIC(HCM)

public:
	HCM(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~HCM();
	virtual void OnFinalRelease();

// �Ի�������
	enum { IDD = IDD_DIALOG_HCM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	GDALDataset* poDataset;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
	void hcmCluster(Hclass* hc,int num,Hsamp* hs);
	CProgressCtrl* m_progress;
};