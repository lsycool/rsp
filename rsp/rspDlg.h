
// rspDlg.h : 头文件
//
#include "ImageShow.h"
#include "LinearStepParamDlg.h"//分段线性拉伸参数设置
#include "DlgLinerPara.h"
#include "cDlgMorphErosion.h"
#include "cDlgMorphClose.h"
#include "cDlgMorphDilation.h"
#include "cDlgMorphOpen.h"
#include "kernel.h"

extern ImageShow* imageS;
extern CString pathname;
extern CString path;
extern int flag1;
extern BOOL selnum;
extern LinearStepParamDlg linesdlgPara;
extern CDlgLinerPara dlgPara;
extern cDlgMorphErosion dlgPara1;
extern cDlgMorphDilation dlgPara2;
extern cDlgMorphOpen dlgPara3;
extern cDlgMorphClose dlgPara4;
//extern CString savePath;
//extern CString FilePath;
#pragma once

using namespace MyEngine;
// CrspDlg 对话框
class CrspDlg : public CDialogEx
{
// 构造
public:
	CrspDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RSP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
		Kernel k1;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDigopen();
	afx_msg void OnDlgCancel();
	afx_msg void OnColorsy();
	CString FilePath;
	afx_msg void On32787();
	afx_msg void OnRGBtoGray();
	afx_msg void OnSuoYin();
	afx_msg void OnJunHen();
	afx_msg void OnContrastEnhancement();
	afx_msg void OnERZ();
	afx_msg void OnISODATA();
	afx_msg void OnChangeDetect();
	afx_msg void OnGCP();
	afx_msg void OnSave();
	afx_msg void OnSaveA();
	afx_msg void OnKmeans();
	afx_msg void OnHcm();
	afx_msg void OnMinMax();
	afx_msg void OnSimilar();
	afx_msg void OnBp();
	afx_msg void OnMoravec();
	afx_msg void OnImageMatch();
	afx_msg void OnPHOTOGRAM();
	afx_msg void OnAbout();
	afx_msg void OnGanzhiqi();
	afx_msg void OnIhs();
	afx_msg void OnIhsf();
	afx_msg void OnIhsmerge();
	afx_msg void OnQfft();
	afx_msg void OnQfftn();
	afx_msg void OnFft();
	afx_msg void OnFftn();
	afx_msg void OnOpenfiledialog();
	afx_msg void OnKl();
	afx_msg void OnProtran();
	afx_msg void OnUnweighting();
	afx_msg void OnWeighting();
	afx_msg void OnAddgaussnoise();
	afx_msg void OnAddpeppersaltnoise();
	afx_msg void OnConvolution();
	afx_msg void OnRobert();
	afx_msg void OnSobel();
	afx_msg void OnPrewitt();
	afx_msg void OnLaplacian();
	afx_msg void OnEdgetrace();
	afx_msg void OnRegiongrow();
	afx_msg void OnSubsetimg();
	afx_msg void OnRoi();
	afx_msg void OnSlope();
	afx_msg void OnAspect();
	afx_msg void OnTPI();
	afx_msg void OnHillShade();
	afx_msg void OnTRI();
	afx_msg void OnRoughness();
	afx_msg void OnMultiScaleSegment();
	afx_msg void OnWatershed();
	afx_msg void OnLinearStrecth();
	afx_msg void OnPointLiner();
	afx_msg void OnMorphErosion();
	afx_msg void OnMorphDilation();
	afx_msg void OnMorphOpen();
	afx_msg void OnMorphClose();
	afx_msg void OnMorphThining();
	afx_msg void OnHitAndMiss();
	afx_msg void OnAboutAuthor();
	afx_msg void OnMiDuSeg();
	afx_msg void OnMenuLoadPlug();
	afx_msg void OnMenuDeal(UINT nID);
public:
	int FindMenuItem(CMenu* Menu, CString MenuString);
	CString GetPluginPath();
	void InitDll();
	afx_msg void OnDigitRecog();
};
