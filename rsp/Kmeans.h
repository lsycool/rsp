#pragma once



// Kmeans 对话框

class Kmeans : public CDialogEx
{
	DECLARE_DYNAMIC(Kmeans)

public:
	Kmeans(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Kmeans();

// 对话框数据
	enum { IDD = IDD_DIALOG_Kmeans };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
//	Byte Kvalue;
//	BYTE m_Kvalue;
	int m_DieD;
	int m_Kvalue;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CProgressCtrl* m_Progress;
	virtual BOOL OnInitDialog();
};


//K均值聚类算法
#ifndef _CLUSTERING_H_
#define _CLUSTERING_H_
struct VQ_VECTOR 
{
	BYTE* Data;//输入向量
	int nDimension;//输入向量的维数
	int nCluster;//聚类过程中向量所属的类号
};

struct VQ_CENTER
{
	BYTE* Data;//聚类中心
	int nDimension;//中心向量的维数
	int Num;//属于此类的向量个数
};


int LBGCluster(VQ_VECTOR* X,int n,VQ_CENTER* y,int m);//k均值聚类算法
#endif
