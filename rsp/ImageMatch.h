#pragma once
#include "Morac.h"
#include <math.h>


struct MATCHEDPOINTS 
{
	int x1;
	int y1;
	int x2;
	int y2;				//坐标
	float coefficient;	//相关系数
};


// CImageMatch 对话框

class CImageMatch : public CDialogEx
{
	DECLARE_DYNAMIC(CImageMatch)

public:
	CImageMatch(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImageMatch();

// 对话框数据
	enum { IDD = IDD_DIALOG_IMGMATCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMoin();
	CString MH_inL;
	CString MH_inR;
	CString MH_outL;
	CString MH_outR;
	CString MH_data;
	afx_msg void OnBnClickedButtonMoin2();
	afx_msg void OnBnClickedButtonMout();
	afx_msg void OnBnClickedButtonMout2();
	afx_msg void OnBnClickedButtonCodat();
	void ReadImage(BYTE* &Data,int &nImageSizeX,int &nImageSizeY,CString FilePath);
	// 存储影像
	void SaveImage(BYTE* Outdata,int nImageSizeX,int nImageSizeY,CString OutPath, CString ImageType);
	// 存储匹配点
	void SaveToTxt(vector<MATCHEDPOINTS> MatchedPoints,float thre,int Wsize,CString OutFile);
	// //计算莫罗维克算子
	void Movarec(BYTE* Indata1,vector<FEATUREPOINT> &FeaturePoint,int ImageSizeX,int ImageSizeY,int win1,int win2,double thre);
	void DrawCross(BYTE* pBits1, BYTE* pBits2, vector<MATCHEDPOINTS> MatchedPoints,int _width1, int _width2, int _height1, int _height2);

	afx_msg void OnBnClickedOk();
	int m_threL;
	int m_threR;
	int m_winL1;
	int m_winL2;
	int m_winR1;
	int m_winR2;
	int m_scare;
	float m_cort;
	virtual BOOL OnInitDialog();
};
