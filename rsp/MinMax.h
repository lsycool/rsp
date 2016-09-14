#pragma once
#include <vector>
using namespace std;

// MinMax 对话框
struct MaSample
{
	BYTE data;//每个样本元素
	int mark;//所属类号
};

struct MaClass//聚类中心
{
	vector<MaSample*>sample;//类中的元素
	BYTE data;//类中心
};

class MinMax : public CDialogEx
{
	DECLARE_DYNAMIC(MinMax)

public:
	MinMax(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MinMax();

// 对话框数据
	enum { IDD = IDD_DIALOG_MinMAX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CProgressCtrl* m_progress;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	MaClass* mclass;
	MaSample* msample;
//	float m_sida;
	float m_sida;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	int nImageSizeX;
	int nImageSizeY;
};
int LinearRegression(BYTE *data1,BYTE* data2,int rows, double *a, double *b/*, double *SquarePoor*/);