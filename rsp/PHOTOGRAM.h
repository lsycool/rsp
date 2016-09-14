#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
using namespace std;

const int n=6;

// CPHOTOGRAM 对话框

class CPHOTOGRAM : public CDialogEx
{
	DECLARE_DYNAMIC(CPHOTOGRAM)

public:
	CPHOTOGRAM(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPHOTOGRAM();

// 对话框数据
	enum { IDD = IDD_DIALOG_PHOGMT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString XP_IN;
	CString DM_OUT;
	CString WFW_OUT;
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton2();
	// //解算外方位元素
	void CaculateWFW(double B[4][5], float x0, float y0, float fk, float m, CString FilePath,CString FilePath1);
	float m_X0;
	float m_Y0;
	float m_FK;
	float m_BLC;
	template<typename T1,typename T2>void transpose(T1*mat1,T2*mat2,int a,int b);
	template<typename T1,typename T2>void multi(T1*mat1,T2 * mat2,T2 * result,int a,int b,int c);
	template<typename T>void input (T*mat,int a,int b);
	void inverse (double c[n][n]);
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
