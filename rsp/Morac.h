#pragma once

#include <vector>
using namespace std;

// CMorac 对话框

//特征点
struct FEATUREPOINT
{
	int x;
	int y;		//坐标
	float IV;	//兴趣值
};

class CMorac : public CDialogEx
{
	DECLARE_DYNAMIC(CMorac)

public:
	CMorac(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMorac();

// 对话框数据
	enum { IDD = IDD_DIALOG_PHOTOGRAMMETRY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_thre;
	double m_win1;
	double m_win2;

	CString M_in;//输入影像
	CString M_out;//输出影像
	CString M_data;//输出兴趣点
	afx_msg void OnBnClickedButtonMoin();
	afx_msg void OnBnClickedButtonMout();
	afx_msg void OnBnClickedButtonCodat();
	afx_msg void OnBnClickedOk();
	// 计算莫罗维克
	void CaculMoc(BYTE* Indata1, BYTE* outdata,int ImageSizeX,int ImageSizeY,int win1,int win2,double thre,CString file,CString TxtFile);
	// 保存结果影像
	void SaveImage(BYTE* OutData,int ImageSizeX,int ImageSizeY, CString saveFile, CString ImageType);
	// //保存特征点坐标
	void SaveToText(CString file,int Count1,vector<FEATUREPOINT> FeaturePoint2);
//	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	virtual BOOL OnInitDialog();
};
