#pragma once



// Kmeans �Ի���

class Kmeans : public CDialogEx
{
	DECLARE_DYNAMIC(Kmeans)

public:
	Kmeans(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Kmeans();

// �Ի�������
	enum { IDD = IDD_DIALOG_Kmeans };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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


//K��ֵ�����㷨
#ifndef _CLUSTERING_H_
#define _CLUSTERING_H_
struct VQ_VECTOR 
{
	BYTE* Data;//��������
	int nDimension;//����������ά��
	int nCluster;//����������������������
};

struct VQ_CENTER
{
	BYTE* Data;//��������
	int nDimension;//����������ά��
	int Num;//���ڴ������������
};


int LBGCluster(VQ_VECTOR* X,int n,VQ_CENTER* y,int m);//k��ֵ�����㷨
#endif
