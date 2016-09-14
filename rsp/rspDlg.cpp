//一般出现无法解析的外部符号的原因是函数只找到声明但没有实现，或者是少了什么链接库。
// rspDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "rsp.h"
#include "rspDlg.h"
#include "afxdialogex.h"
#include "BandSythesis.h"
#include "ZhiFT.h"
#include "SuoYin.h"
#include "Contrast.h"
#include "ERZH.h"
#include "ISODATA.h"
#include "ChangeDetect.h"
#include "GCP.h"
#include "Kmeans.h"
#include "HCM.h"
#include "MinMax.h"
#include "Similar.h"
#include "BP.h"
#include "Morac.h"
#include "ImageMatch.h"
#include "PHOTOGRAM.h"
#include "Arthmetic.h"
#include "FileList.h"
#include "ROI.h"
#include "DigitRecogDlg.h"
#include <shellapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CString path="";
ImageShow* imageS=NULL;
LinearStepParamDlg linesdlgPara;
CDlgLinerPara dlgPara;
cDlgMorphErosion dlgPara1;
cDlgMorphDilation dlgPara2;
cDlgMorphOpen dlgPara3;
cDlgMorphClose dlgPara4;

extern CString BiaoZhi;
extern CFileList* cfiles;
extern CROI* roi;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CrspDlg 对话框




CrspDlg::CrspDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CrspDlg::IDD, pParent)
	, FilePath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CrspDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CrspDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_DigOpen, &CrspDlg::OnDigopen)
	ON_COMMAND(ID_32772, &CrspDlg::OnDlgCancel)
	ON_COMMAND(ID_ColorSy, &CrspDlg::OnColorsy)
	ON_COMMAND(ID_32787, &CrspDlg::On32787)
	ON_COMMAND(ID_32789, &CrspDlg::OnRGBtoGray)
	ON_COMMAND(ID_32788, &CrspDlg::OnSuoYin)
	ON_COMMAND(ID_32791, &CrspDlg::OnJunHen)
	ON_COMMAND(ID_32790, &CrspDlg::OnContrastEnhancement)
	ON_COMMAND(ID_32792, &CrspDlg::OnERZ)
	ON_COMMAND(ID_32794, &CrspDlg::OnISODATA)
	ON_COMMAND(ID_32795, &CrspDlg::OnChangeDetect)
	ON_COMMAND(ID_32793, &CrspDlg::OnGCP)
	ON_COMMAND(ID_32774, &CrspDlg::OnSave)
	ON_COMMAND(ID_Menu, &CrspDlg::OnSaveA)
	ON_COMMAND(ID_32799, &CrspDlg::OnKmeans)
	ON_COMMAND(ID_HCM, &CrspDlg::OnHcm)
	ON_COMMAND(ID_32805, &CrspDlg::OnMinMax)
	ON_COMMAND(ID_32806, &CrspDlg::OnSimilar)
	ON_COMMAND(ID_BP, &CrspDlg::OnBp)
	ON_COMMAND(ID_32810, &CrspDlg::OnMoravec)
	ON_COMMAND(ID_32811, &CrspDlg::OnImageMatch)
	ON_COMMAND(ID_32809, &CrspDlg::OnPHOTOGRAM)
	ON_COMMAND(ID_About, &CrspDlg::OnAbout)
	ON_COMMAND(ID_GanZhiQi, &CrspDlg::OnGanzhiqi)
	ON_COMMAND(ID_IHS, &CrspDlg::OnIhs)
	ON_COMMAND(ID_IHSF, &CrspDlg::OnIhsf)
	ON_COMMAND(ID_IHSMERGE, &CrspDlg::OnIhsmerge)
	ON_COMMAND(ID_QFFT, &CrspDlg::OnQfft)
	ON_COMMAND(ID_QFFTN, &CrspDlg::OnQfftn)
	ON_COMMAND(ID_FFT, &CrspDlg::OnFft)
	ON_COMMAND(ID_FFTN, &CrspDlg::OnFftn)
	ON_COMMAND(ID_OPENFILEDIALOG, &CrspDlg::OnOpenfiledialog)
	ON_COMMAND(ID_KL, &CrspDlg::OnKl)
	ON_COMMAND(ID_PROTRAN, &CrspDlg::OnProtran)
	ON_COMMAND(ID_UNWEIGHTING, &CrspDlg::OnUnweighting)
	ON_COMMAND(ID_WEIGHTING, &CrspDlg::OnWeighting)
	ON_COMMAND(ID_AddGaussNoise, &CrspDlg::OnAddgaussnoise)
	ON_COMMAND(ID_AddPepperSaltNoise, &CrspDlg::OnAddpeppersaltnoise)
	ON_COMMAND(ID_Convolution, &CrspDlg::OnConvolution)
	ON_COMMAND(ID_Robert, &CrspDlg::OnRobert)
	ON_COMMAND(ID_Sobel, &CrspDlg::OnSobel)
	ON_COMMAND(ID_Prewitt, &CrspDlg::OnPrewitt)
	ON_COMMAND(ID_Laplacian, &CrspDlg::OnLaplacian)
	ON_COMMAND(ID_EdgeTrace, &CrspDlg::OnEdgetrace)
	ON_COMMAND(ID_RegionGrow, &CrspDlg::OnRegiongrow)
	ON_COMMAND(ID_SUBSETIMG, &CrspDlg::OnSubsetimg)
	ON_COMMAND(ID_ROI, &CrspDlg::OnRoi)
	ON_COMMAND(ID_Menu32929, &CrspDlg::OnSlope)
	ON_COMMAND(ID_32930, &CrspDlg::OnAspect)
	ON_COMMAND(ID_32932, &CrspDlg::OnTPI)
	ON_COMMAND(ID_32931, &CrspDlg::OnHillShade)
	ON_COMMAND(ID_32933, &CrspDlg::OnTRI)
	ON_COMMAND(ID_32934, &CrspDlg::OnRoughness)
	ON_COMMAND(ID_32903, &CrspDlg::OnMultiScaleSegment)
	ON_COMMAND(ID_WaterShed, &CrspDlg::OnWatershed)
	ON_COMMAND(ID_32812, &CrspDlg::OnLinearStrecth)
	ON_COMMAND(ID_32935, &CrspDlg::OnPointLiner)
	ON_COMMAND(ID_32936, &CrspDlg::OnMorphErosion)
	ON_COMMAND(ID_32937, &CrspDlg::OnMorphDilation)
	ON_COMMAND(ID_32938, &CrspDlg::OnMorphOpen)
	ON_COMMAND(ID_32939, &CrspDlg::OnMorphClose)
	ON_COMMAND(ID_32943, &CrspDlg::OnMorphThining)
	ON_COMMAND(ID_32942, &CrspDlg::OnHitAndMiss)
	ON_COMMAND(ID_32948, &CrspDlg::OnAboutAuthor)
	ON_COMMAND(ID_32856, &CrspDlg::OnMiDuSeg)
	ON_COMMAND(ID_Menu32951, &CrspDlg::OnMenuLoadPlug)
	ON_COMMAND_RANGE(ID_MENUSTART,ID_MENUEND,&CrspDlg::OnMenuDeal)
	ON_COMMAND(ID_32844, &CrspDlg::OnDigitRecog)
END_MESSAGE_MAP()


// CrspDlg 消息处理程序

BOOL CrspDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	MoveWindow(10,10,1046,63);

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	CMenu* menu=new CMenu;//声明菜单对象
	menu->LoadMenu(IDR_MENU1);//加载用户自定义菜单
	this->SetMenu(menu);//设置自定义菜单替换现有菜单 

	ShowWindow(SW_MINIMIZE);

	InitDll();
	// TODO: 在此添加额外的初始化代码
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CrspDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CrspDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else 
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CrspDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CrspDlg::OnDigopen()
{
	// TODO: 在此添加命令处理程序代码
	CString strTemp;
	CFileDialog dlg(
		true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.bmp)|*.bmp|JPG文件(*.jpg)|*.jpg;*.jpeg|tiff文件(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	dlg.m_ofn.lpstrTitle="请输入遥感影像";
	if(dlg.DoModal() == IDOK)
	{
		flag1=0;
		FilePath=dlg.GetPathName();
		path=FilePath;
		pathname=dlg.GetFileTitle();

		if (imageS->GetSafeHwnd()==NULL)
		{
			imageS=new ImageShow;
			imageS->Create(IDD_IMAGE_SHOW);
			selnum=FALSE;
		}
		else
		{
			imageS->ShowWindow(SW_SHOW);
			imageS->Invalidate(TRUE);
		}
		return;	
	}

}


void CrspDlg::OnDlgCancel()
{
	// TODO: 在此添加命令处理程序代码
	CDialogEx::OnCancel();
}


void CrspDlg::OnColorsy()
{
	BandSythesis* bandSy=new BandSythesis;
	bandSy->Create(IDD_DIALOG_BandSy);
	// TODO: 在此添加命令处理程序代码
}

//直方图显示
void CrspDlg::On32787()
{
	// TODO: 在此添加命令处理程序代码
	ZhiFT* zft=new ZhiFT;
	zft->Create(IDD_DIALOG_ZhiFangTu);
	
}


void CrspDlg::OnRGBtoGray()
{
	flag1=1;
	imageS->Invalidate(FALSE);
	// TODO: 在此添加命令处理程序代码
}


void CrspDlg::OnSuoYin()
{
	// TODO: 在此添加命令处理程序代码
	SuoYin* SY=new SuoYin;
	SY->Create(IDD_DIALOG_SuoYin);
}


void CrspDlg::OnJunHen()
{
	// TODO: 在此添加命令处理程序代码
	flag1=4;
//	ImageShow* image=new ImageShow;
//	image->Create(IDD_IMAGE_SHOW);
//	image->ShowWindow(SW_SHOW);
	imageS->Invalidate(FALSE);
}


void CrspDlg::OnContrastEnhancement()
{
	flag1=3;
	Contrast* Con=new Contrast;
	Con->Create(IDD_DIALOG_CONSTRAST);
	// TODO: 在此添加命令处理程序代码
}


void CrspDlg::OnERZ()
{
	flag1=5;
	ERZH* er=new ERZH;
	er->Create(IDD_DIALOG_ERZ);
	// TODO: 在此添加命令处理程序代码
}


void CrspDlg::OnISODATA()
{
	// TODO: 在此添加命令处理程序代码
	ISODATA* iso=new ISODATA;
	iso->Create(IDD_DIALOG_ISOD);
}


void CrspDlg::OnChangeDetect()
{
	// TODO: 在此添加命令处理程序代码
	ChangeDetect* CD=new ChangeDetect;
	CD->Create(IDD_DIALOG_CHANGED);
	CD->ShowWindow(SW_SHOW);
	
}


void CrspDlg::OnGCP()
{
	// TODO: 在此添加命令处理程序代码
	GCP* JZgcp=new GCP;
	JZgcp->Create(IDD_DIALOG_JZ);
	JZgcp->ShowWindow(SW_SHOW);
}


void CrspDlg::OnSave()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(
		false, "BMP", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.bmp)|*.bmp|JPG文件(*.jpg)|*.jpg;*.jpeg|tiff文件(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	dlg.m_ofn.lpstrTitle="请输入存储位置";
	if(dlg.DoModal() == IDOK)
	{
//		savePath=dlg.GetPathName();
	}
}


void CrspDlg::OnSaveA()
{
	//// TODO: 在此添加命令处理程序代码
	//char* pszFormat;
	//if(RPath.Right(4)==".BMP")
	//{
	//	pszFormat = "BMP";
	//}	

	//GDALDriver *poDriver;
	//poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	//GDALDataset *poDstDS;
	//char **papszOptions =poDriver->GetMetadata();
	//poDstDS = poDriver->Create( (char*)(LPCTSTR)RPath,//输出路径
	//	nImageSizeX,nImageSizeY, //影像数据宽高
	//	3, //存或取的波段数
	//	GDT_Byte, 	//缓冲区数据类型
	//	papszOptions );

	//EndWaitCursor();
	//m_Progress->SetPos(100);

	//if(poDstDS->RasterIO(
	//	GF_Write,						//IO方式
	//	0,0,							//影像数据起始位置x,y
	//	nImageSizeX,nImageSizeY,				//影像数据宽高
	//	OutData,						//指向缓冲区数据指针
	//	nImageSizeX,nImageSizeY,				//缓冲区数据宽高
	//	GDT_Byte,						//缓冲区数据类型
	//	3,						//存或取的波段数
	//	NULL,							//存或取的波段列表
	//	0,								//缓冲区数据一个像素占用的字节数
	//	0,								//缓冲区一行数据所占用的字节数
	//	0)==CE_Failure )				//缓冲区数据存储方式 0为RRRRGGGGBBBB-bsp 1为RGBRGBRGB-bip	
	//{
	//	AfxMessageBox("保存文件失败！");
	//	return;
	//}
	//else
	//	AfxMessageBox("分类完成！");

	//GDALClose(poDstDS);
	//delete []OutData;
	//delete []InData;

	//path=RPath;
	//ImageShow* image=new ImageShow;
	//image->Create(IDD_IMAGE_SHOW);
	//image->MoveWindow(690,75,660,660);
	//image->ShowWindow(SW_SHOW);

	CDialogEx::OnOK();
}


void CrspDlg::OnKmeans()
{
	// TODO: 在此添加命令处理程序代码
	Kmeans* KJZ=new Kmeans;
	KJZ->Create(IDD_DIALOG_Kmeans);
	KJZ->ShowWindow(SW_SHOW);

}


void CrspDlg::OnHcm()
{
	// TODO: 在此添加命令处理程序代码
	HCM* hc=new HCM;
	hc->Create(IDD_DIALOG_HCM);
	hc->ShowWindow(SW_SHOW);
}

//最大最小值分类
void CrspDlg::OnMinMax()
{
	// TODO: 在此添加命令处理程序代码
	MinMax* mima=new MinMax;
	mima->Create(IDD_DIALOG_MinMAX);
	mima->ShowWindow(SW_SHOW);
}


void CrspDlg::OnSimilar()
{
	// TODO: 在此添加命令处理程序代码
	CSimilar* sim=new CSimilar;
	sim->Create(IDD_DIALOG_Similar);
	sim->ShowWindow(SW_SHOW);
}

//BP神经网络
void CrspDlg::OnBp()
{
	// TODO: 在此添加命令处理程序代码
	CBP* BP=new CBP;
	BP->Create(IDD_DIALOG_BP);
	BP->ShowWindow(SW_SHOW);
}

//计算莫罗维克算子
void CrspDlg::OnMoravec()
{
	// TODO: 在此添加命令处理程序代码
	CMorac* moc=new CMorac;
	moc->Create(IDD_DIALOG_PHOTOGRAMMETRY);
	moc->ShowWindow(SW_SHOW);
}

//特征点配准
void CrspDlg::OnImageMatch()
{
	// TODO: 在此添加命令处理程序代码
	CImageMatch* match=new CImageMatch;
	match->Create(IDD_DIALOG_IMGMATCH);
	match->ShowWindow(SW_SHOW);
}

//空间后方交会外方位元素解算
void CrspDlg::OnPHOTOGRAM()
{
	// TODO: 在此添加命令处理程序代码
	CPHOTOGRAM* PHG=new CPHOTOGRAM;
	PHG->Create(IDD_DIALOG_PHOGMT);
	PHG->ShowWindow(SW_SHOW);
}


void CrspDlg::OnAbout()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg cabd;
	cabd.DoModal();
}


void CrspDlg::OnGanzhiqi()
{
	// TODO: 在此添加命令处理程序代码
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnIhs()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="IHS";
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnIhsf()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="IHSF";
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnIhsmerge()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="IHSMERGE";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnQfft()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="QFFT";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnQfftn()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="QFFTN";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnFft()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="FFT";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnFftn()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="FFTN";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnOpenfiledialog()
{
	// TODO: 在此添加命令处理程序代码
	cfiles->ShowWindow(SW_SHOW);
}


void CrspDlg::OnKl()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="KL";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnProtran()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="ProTran";	
	CArthmetic math;
	math.DoModal();

}


void CrspDlg::OnUnweighting()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="UnweightingMerge";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnWeighting()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="WeightingMerge";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnAddgaussnoise()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="GAOSI";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnAddpeppersaltnoise()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="SaltNoise";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnConvolution()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="Convolution";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnRobert()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="Robert";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnSobel()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="Sobel";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnPrewitt()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="Prewitt";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnLaplacian()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="Laplacian";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnEdgetrace()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="Edgetrace";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnRegiongrow()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="RegionGrow";	
	CArthmetic math;
	math.DoModal();

}


void CrspDlg::OnSubsetimg()
{
	// TODO: 在此添加命令处理程序代码
		BiaoZhi="OnSubsetimg";
		AfxMessageBox("请在视图内圈定裁剪区..(单击左键开始，单击右键结束)");
}


void CrspDlg::OnRoi()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="ROI";

	roi=new CROI;
	roi->Create(IDD_DIALOG_ROI);
	roi->ShowWindow(SW_SHOW);
}


void CrspDlg::OnSlope()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="Slope";	
	CArthmetic math;
	math.DoModal();

}


void CrspDlg::OnAspect()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="Aspect";	
	CArthmetic math;
	math.DoModal();

}


void CrspDlg::OnTPI()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="TPI";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnHillShade()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="HillShade";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnTRI()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="TRI";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnRoughness()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="Roughness";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnMultiScaleSegment()
{
	// TODO: 在此添加命令处理程序代码
	typedef BOOL (/*WINAPI */* TESTDLL)(CString);
	HINSTANCE hmod;
	hmod = ::LoadLibrary ("TestDll.dll");
	if(hmod==NULL)
	{
		AfxMessageBox("链接DLL失败");
		return;
	}
	TESTDLL lpproc;
	lpproc = (TESTDLL)GetProcAddress (hmod,"MultiScaleSegment");
	if(lpproc!=(TESTDLL)NULL)
	{
		(*lpproc)(path);
	}
	else
	{
		AfxMessageBox("无法加载函数");
	}

}


void CrspDlg::OnWatershed()
{
	// TODO: 在此添加命令处理程序代码
	BiaoZhi="Watershed";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnLinearStrecth()
{
	// TODO: 在此添加命令处理程序代码
	//线性拉伸参数设置对话框
	flag1=6;
	if(linesdlgPara.DoModal()==IDOK)
	{
		//获取用户设置的两个拉伸转折点
		CPoint point1,point2;
		point1.x=linesdlgPara.m_nX1;
		point1.y=linesdlgPara.m_nY1;
		point2.x =linesdlgPara.m_nX2;
		point2.y=linesdlgPara.m_nY2;

		//判断参数的合理性
		if(point1.x<0||point1.x>255||point2.x<0||point2.x>255
			||point1.y<0||point1.y>255||point2.y<0||point2.y>255
			||point1.x>point2.x)
		{
				::MessageBox(0,"参数不合理",MB_OK,0);
				flag1=-1;//退出
				return;
		}
	}
	imageS->Invalidate(FALSE);
}


void CrspDlg::OnPointLiner()
{
	// TODO: 在此添加命令处理程序代
	flag1=7;
	// 初始化变量值
	dlgPara.m_fA = 2.0;
	dlgPara.m_fB = -128.0;

	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	imageS->Invalidate(FALSE);
}


void CrspDlg::OnMorphErosion()
{
	// TODO: 在此添加命令处理程序代码
	flag1=8;
	// 初始化变量值
	dlgPara1.m_nMode = 0;

	if (dlgPara1.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	imageS->Invalidate(FALSE);
}


void CrspDlg::OnMorphDilation()
{
	// TODO: 在此添加命令处理程序代码
	flag1=9;
	// 初始化变量值
	dlgPara2.m_nMode = 0;

	if (dlgPara2.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	imageS->Invalidate(FALSE);

}


void CrspDlg::OnMorphOpen()
{
	// TODO: 在此添加命令处理程序代码
	flag1=10;
	// 初始化变量值
	dlgPara3.m_nMode = 0;

	if (dlgPara3.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	imageS->Invalidate(FALSE);

}


void CrspDlg::OnMorphClose()
{
	// TODO: 在此添加命令处理程序代码
	flag1=11;
	// 初始化变量值
	dlgPara4.m_nMode = 0;

	if (dlgPara4.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	imageS->Invalidate(FALSE);
}


void CrspDlg::OnMorphThining()
{
	// TODO: 在此添加命令处理程序代码
	flag1=12;
	imageS->Invalidate(FALSE);
}


void CrspDlg::OnHitAndMiss()
{
	// TODO: 在此添加命令处理程序代码
	flag1=13;
	imageS->Invalidate(FALSE);
}


void CrspDlg::OnAboutAuthor()
{
	// TODO: 在此添加命令处理程序代码
	ShellExecute(NULL, "open", "http://weibo.com/u/1194046363?wvr=3.6&lf=reg",NULL, NULL, SW_SHOWNORMAL);
}


void CrspDlg::OnMiDuSeg()
{
	// TODO: 在此添加命令处理程序代码
	flag1=14;
	imageS->Invalidate(FALSE);
}

//加载插件
void CrspDlg::OnMenuLoadPlug()
{
	// TODO: 在此添加命令处理程序代码
	CString PlugPath;
	CFileDialog dlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, "插件(*.dll)|*.dll||");
	dlg.m_ofn.lpstrTitle="请添加插件";

	if(dlg.DoModal() == IDOK)
	{
		PlugPath=dlg.GetPathName();
	}
	else
	{
		return;
	}
	k1.loadPlugin(PlugPath.GetBuffer(0));
	CMenu *pMenu = AfxGetMainWnd()->GetMenu();
	int Pos = FindMenuItem(pMenu,_T("插件管理"));
	if (-1 == Pos)
	{
		return;
	}
	CMenu *pSubMenu = pMenu->GetSubMenu(Pos);
	int count=	pSubMenu->GetMenuItemCount();			
	int i=0;
	for (std::map<std::string, Plugin>::iterator it=k1.m_LoadedPlugins.begin(); it != k1.m_LoadedPlugins.end(); it++ )
	{
		//添加子项
		CString Engname=(*it).second.getEngineName();
		if(FindMenuItem(pSubMenu,Engname)>0||FindMenuItem(pSubMenu,Engname)==0)//判断当前插件是否已经加载，如果未加载则插入到当前插件管理中
			return;
		pSubMenu->InsertMenu(i,MF_BYPOSITION,ID_MENUSTART+i,(*it).second.getEngineName());
		i++;
	}
}

int CrspDlg::FindMenuItem(CMenu* Menu, CString MenuString)
{
	ASSERT(Menu);
	ASSERT(::IsMenu(Menu->GetSafeHmenu()));
	int count = Menu->GetMenuItemCount();//返回该菜单的项数
	for (int i = 0; i < count; i++)
	{
		CString str;
		if (Menu->GetMenuString(i, str, MF_BYPOSITION) &&str.Compare(MenuString) == 0)//将指定菜单项的标签拷贝到指定的缓冲区
			return i;
	}
	return -1;
}

void CrspDlg::OnMenuDeal(UINT nID)
{
	CMenu *pMenu = AfxGetMainWnd()->GetMenu();
	int Pos = FindMenuItem(pMenu,_T("插件管理"));
	CMenu *pSubMenu = pMenu->GetSubMenu(Pos);
	CString DllName;
	//根据命令找到子项
	pSubMenu->GetMenuString(nID,DllName,MF_BYCOMMAND);//获取插件名字
	for (std::map<std::string, Plugin>::iterator it=k1.m_LoadedPlugins.begin(); it != k1.m_LoadedPlugins.end(); it++ )
	{
		if ((*it).second.getEngineName()==DllName)
		{
			(*it).second.registerPlugin(FilePath);
		}
	}
}

CString CrspDlg::GetPluginPath()
{
	TCHAR  exePath[MAX_PATH];
	GetModuleFileName(NULL,exePath,MAX_PATH);
	CString Exepath;
	for(int i = 0;i<MAX_PATH;i++)
	{
		if ('\0' == exePath[i])
		{
			break;
		}
		CString str;
		str.Format(_T("%c"),exePath[i]);
		Exepath = Exepath + str;
	}
	Exepath = Exepath.Left(Exepath.ReverseFind('\\')+1);
	Exepath = Exepath + "plugin\\";
	return Exepath;
}

//初始化插件环境，加载插件文件夹所有的插件
void CrspDlg::InitDll()
{
	//存储文件名的变量
	CStringArray FileName;
	//搜索路径为应用程序的根目录
	CString Pluginpath = GetPluginPath() + _T("*.dll");
	//对*.exe进行查找
	CFileFind pFile;
	BOOL BeWork = pFile.FindFile(Pluginpath);
	while(BeWork)
	{
		BeWork = pFile.FindNextFile();
		if (pFile.IsDots())//如果不是想要的跳过
		{
			continue;
		}
		FileName.Add(pFile.GetFilePath());//如果找到添加进数组中
	}
	pFile.Close();
	//如果没有找到之后不做处理，直接返回
	if (0 == FileName.GetCount())
	{
		return;
	}

	//动态添加菜单
	CMenu *pMenu = GetMenu();
	//首先找到列
	int Pos = FindMenuItem(pMenu,_T("插件管理"));
	if (-1 == Pos)
	{
		return;
	}
	//找到下面的子项
	CMenu *pSubMenu = pMenu->GetSubMenu(Pos);
	for (int i = 0;i<FileName.GetCount();i++)
	{
		//添加子项
		//string strName=CString(FileName.GetAt(i)).GetBuffer(0);
		k1.loadPlugin(CString(FileName.GetAt(i)).GetBuffer(0));
		std::map<std::string, Plugin>::iterator it=k1.m_LoadedPlugins.begin();
		pSubMenu->InsertMenu(i,MF_BYPOSITION,ID_MENUSTART+i,(*it).second.getEngineName());
		it++;
	}
	//添加完成后清空数组
	FileName.RemoveAll();
}

//手写数字识别
void CrspDlg::OnDigitRecog()
{
	// TODO: 在此添加命令处理程序代码
	CDigitRecogDlg dlg;
	dlg.DoModal();
}
