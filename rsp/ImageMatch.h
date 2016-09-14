#pragma once
#include "Morac.h"
#include <math.h>


struct MATCHEDPOINTS 
{
	int x1;
	int y1;
	int x2;
	int y2;				//����
	float coefficient;	//���ϵ��
};


// CImageMatch �Ի���

class CImageMatch : public CDialogEx
{
	DECLARE_DYNAMIC(CImageMatch)

public:
	CImageMatch(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImageMatch();

// �Ի�������
	enum { IDD = IDD_DIALOG_IMGMATCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	// �洢Ӱ��
	void SaveImage(BYTE* Outdata,int nImageSizeX,int nImageSizeY,CString OutPath, CString ImageType);
	// �洢ƥ���
	void SaveToTxt(vector<MATCHEDPOINTS> MatchedPoints,float thre,int Wsize,CString OutFile);
	// //����Ī��ά������
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
