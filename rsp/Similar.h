#pragma once
#include <vector>
using namespace std;


// CSimilar 对话框
struct Sim_center
{
	vector<BYTE>data;//存储聚类中心
};

struct Sim_sample
{
	BYTE data;
	int mark;
};

class CSimilar : public CDialogEx
{
	DECLARE_DYNAMIC(CSimilar)

public:
	CSimilar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSimilar();

// 对话框数据
	enum { IDD = IDD_DIALOG_Similar };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_T;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CString Similar_IN;//输入文件路径
	CString Similar_OUT;//输出文件路径

	Sim_center center;
	Sim_sample* sample;
	virtual BOOL OnInitDialog();
};
