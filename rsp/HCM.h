#pragma once
#include <vector>
using namespace std;

struct Hclass//每个元素
{
	BYTE data;//输入样本
	int mark;
};

struct Hsamp//类
{
	vector<Hclass*>sample;//合并类
};
// HCM 对话框

class HCM : public CDialogEx
{
	DECLARE_DYNAMIC(HCM)

public:
	HCM(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~HCM();
	virtual void OnFinalRelease();

// 对话框数据
	enum { IDD = IDD_DIALOG_HCM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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