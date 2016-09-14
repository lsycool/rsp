//һ������޷��������ⲿ���ŵ�ԭ���Ǻ���ֻ�ҵ�������û��ʵ�֣�����������ʲô���ӿ⡣
// rspDlg.cpp : ʵ���ļ�
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
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CrspDlg �Ի���




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


// CrspDlg ��Ϣ�������

BOOL CrspDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	MoveWindow(10,10,1046,63);

	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	CMenu* menu=new CMenu;//�����˵�����
	menu->LoadMenu(IDR_MENU1);//�����û��Զ���˵�
	this->SetMenu(menu);//�����Զ���˵��滻���в˵� 

	ShowWindow(SW_MINIMIZE);

	InitDll();
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CrspDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else 
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CrspDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CrspDlg::OnDigopen()
{
	// TODO: �ڴ���������������
	CString strTemp;
	CFileDialog dlg(
		true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	dlg.m_ofn.lpstrTitle="������ң��Ӱ��";
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
	// TODO: �ڴ���������������
	CDialogEx::OnCancel();
}


void CrspDlg::OnColorsy()
{
	BandSythesis* bandSy=new BandSythesis;
	bandSy->Create(IDD_DIALOG_BandSy);
	// TODO: �ڴ���������������
}

//ֱ��ͼ��ʾ
void CrspDlg::On32787()
{
	// TODO: �ڴ���������������
	ZhiFT* zft=new ZhiFT;
	zft->Create(IDD_DIALOG_ZhiFangTu);
	
}


void CrspDlg::OnRGBtoGray()
{
	flag1=1;
	imageS->Invalidate(FALSE);
	// TODO: �ڴ���������������
}


void CrspDlg::OnSuoYin()
{
	// TODO: �ڴ���������������
	SuoYin* SY=new SuoYin;
	SY->Create(IDD_DIALOG_SuoYin);
}


void CrspDlg::OnJunHen()
{
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
}


void CrspDlg::OnERZ()
{
	flag1=5;
	ERZH* er=new ERZH;
	er->Create(IDD_DIALOG_ERZ);
	// TODO: �ڴ���������������
}


void CrspDlg::OnISODATA()
{
	// TODO: �ڴ���������������
	ISODATA* iso=new ISODATA;
	iso->Create(IDD_DIALOG_ISOD);
}


void CrspDlg::OnChangeDetect()
{
	// TODO: �ڴ���������������
	ChangeDetect* CD=new ChangeDetect;
	CD->Create(IDD_DIALOG_CHANGED);
	CD->ShowWindow(SW_SHOW);
	
}


void CrspDlg::OnGCP()
{
	// TODO: �ڴ���������������
	GCP* JZgcp=new GCP;
	JZgcp->Create(IDD_DIALOG_JZ);
	JZgcp->ShowWindow(SW_SHOW);
}


void CrspDlg::OnSave()
{
	// TODO: �ڴ���������������
	CFileDialog dlg(
		false, "BMP", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	dlg.m_ofn.lpstrTitle="������洢λ��";
	if(dlg.DoModal() == IDOK)
	{
//		savePath=dlg.GetPathName();
	}
}


void CrspDlg::OnSaveA()
{
	//// TODO: �ڴ���������������
	//char* pszFormat;
	//if(RPath.Right(4)==".BMP")
	//{
	//	pszFormat = "BMP";
	//}	

	//GDALDriver *poDriver;
	//poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	//GDALDataset *poDstDS;
	//char **papszOptions =poDriver->GetMetadata();
	//poDstDS = poDriver->Create( (char*)(LPCTSTR)RPath,//���·��
	//	nImageSizeX,nImageSizeY, //Ӱ�����ݿ��
	//	3, //���ȡ�Ĳ�����
	//	GDT_Byte, 	//��������������
	//	papszOptions );

	//EndWaitCursor();
	//m_Progress->SetPos(100);

	//if(poDstDS->RasterIO(
	//	GF_Write,						//IO��ʽ
	//	0,0,							//Ӱ��������ʼλ��x,y
	//	nImageSizeX,nImageSizeY,				//Ӱ�����ݿ��
	//	OutData,						//ָ�򻺳�������ָ��
	//	nImageSizeX,nImageSizeY,				//���������ݿ��
	//	GDT_Byte,						//��������������
	//	3,						//���ȡ�Ĳ�����
	//	NULL,							//���ȡ�Ĳ����б�
	//	0,								//����������һ������ռ�õ��ֽ���
	//	0,								//������һ��������ռ�õ��ֽ���
	//	0)==CE_Failure )				//���������ݴ洢��ʽ 0ΪRRRRGGGGBBBB-bsp 1ΪRGBRGBRGB-bip	
	//{
	//	AfxMessageBox("�����ļ�ʧ�ܣ�");
	//	return;
	//}
	//else
	//	AfxMessageBox("������ɣ�");

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
	// TODO: �ڴ���������������
	Kmeans* KJZ=new Kmeans;
	KJZ->Create(IDD_DIALOG_Kmeans);
	KJZ->ShowWindow(SW_SHOW);

}


void CrspDlg::OnHcm()
{
	// TODO: �ڴ���������������
	HCM* hc=new HCM;
	hc->Create(IDD_DIALOG_HCM);
	hc->ShowWindow(SW_SHOW);
}

//�����Сֵ����
void CrspDlg::OnMinMax()
{
	// TODO: �ڴ���������������
	MinMax* mima=new MinMax;
	mima->Create(IDD_DIALOG_MinMAX);
	mima->ShowWindow(SW_SHOW);
}


void CrspDlg::OnSimilar()
{
	// TODO: �ڴ���������������
	CSimilar* sim=new CSimilar;
	sim->Create(IDD_DIALOG_Similar);
	sim->ShowWindow(SW_SHOW);
}

//BP������
void CrspDlg::OnBp()
{
	// TODO: �ڴ���������������
	CBP* BP=new CBP;
	BP->Create(IDD_DIALOG_BP);
	BP->ShowWindow(SW_SHOW);
}

//����Ī��ά������
void CrspDlg::OnMoravec()
{
	// TODO: �ڴ���������������
	CMorac* moc=new CMorac;
	moc->Create(IDD_DIALOG_PHOTOGRAMMETRY);
	moc->ShowWindow(SW_SHOW);
}

//��������׼
void CrspDlg::OnImageMatch()
{
	// TODO: �ڴ���������������
	CImageMatch* match=new CImageMatch;
	match->Create(IDD_DIALOG_IMGMATCH);
	match->ShowWindow(SW_SHOW);
}

//�ռ�󷽽����ⷽλԪ�ؽ���
void CrspDlg::OnPHOTOGRAM()
{
	// TODO: �ڴ���������������
	CPHOTOGRAM* PHG=new CPHOTOGRAM;
	PHG->Create(IDD_DIALOG_PHOGMT);
	PHG->ShowWindow(SW_SHOW);
}


void CrspDlg::OnAbout()
{
	// TODO: �ڴ���������������
	CAboutDlg cabd;
	cabd.DoModal();
}


void CrspDlg::OnGanzhiqi()
{
	// TODO: �ڴ���������������
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnIhs()
{
	// TODO: �ڴ���������������
	BiaoZhi="IHS";
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnIhsf()
{
	// TODO: �ڴ���������������
	BiaoZhi="IHSF";
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnIhsmerge()
{
	// TODO: �ڴ���������������
	BiaoZhi="IHSMERGE";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnQfft()
{
	// TODO: �ڴ���������������
	BiaoZhi="QFFT";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnQfftn()
{
	// TODO: �ڴ���������������
	BiaoZhi="QFFTN";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnFft()
{
	// TODO: �ڴ���������������
	BiaoZhi="FFT";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnFftn()
{
	// TODO: �ڴ���������������
	BiaoZhi="FFTN";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnOpenfiledialog()
{
	// TODO: �ڴ���������������
	cfiles->ShowWindow(SW_SHOW);
}


void CrspDlg::OnKl()
{
	// TODO: �ڴ���������������
	BiaoZhi="KL";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnProtran()
{
	// TODO: �ڴ���������������
	BiaoZhi="ProTran";	
	CArthmetic math;
	math.DoModal();

}


void CrspDlg::OnUnweighting()
{
	// TODO: �ڴ���������������
	BiaoZhi="UnweightingMerge";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnWeighting()
{
	// TODO: �ڴ���������������
	BiaoZhi="WeightingMerge";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnAddgaussnoise()
{
	// TODO: �ڴ���������������
	BiaoZhi="GAOSI";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnAddpeppersaltnoise()
{
	// TODO: �ڴ���������������
	BiaoZhi="SaltNoise";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnConvolution()
{
	// TODO: �ڴ���������������
	BiaoZhi="Convolution";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnRobert()
{
	// TODO: �ڴ���������������
	BiaoZhi="Robert";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnSobel()
{
	// TODO: �ڴ���������������
	BiaoZhi="Sobel";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnPrewitt()
{
	// TODO: �ڴ���������������
	BiaoZhi="Prewitt";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnLaplacian()
{
	// TODO: �ڴ���������������
	BiaoZhi="Laplacian";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnEdgetrace()
{
	// TODO: �ڴ���������������
	BiaoZhi="Edgetrace";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnRegiongrow()
{
	// TODO: �ڴ���������������
	BiaoZhi="RegionGrow";	
	CArthmetic math;
	math.DoModal();

}


void CrspDlg::OnSubsetimg()
{
	// TODO: �ڴ���������������
		BiaoZhi="OnSubsetimg";
		AfxMessageBox("������ͼ��Ȧ���ü���..(���������ʼ�������Ҽ�����)");
}


void CrspDlg::OnRoi()
{
	// TODO: �ڴ���������������
	BiaoZhi="ROI";

	roi=new CROI;
	roi->Create(IDD_DIALOG_ROI);
	roi->ShowWindow(SW_SHOW);
}


void CrspDlg::OnSlope()
{
	// TODO: �ڴ���������������
	BiaoZhi="Slope";	
	CArthmetic math;
	math.DoModal();

}


void CrspDlg::OnAspect()
{
	// TODO: �ڴ���������������
	BiaoZhi="Aspect";	
	CArthmetic math;
	math.DoModal();

}


void CrspDlg::OnTPI()
{
	// TODO: �ڴ���������������
	BiaoZhi="TPI";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnHillShade()
{
	// TODO: �ڴ���������������
	BiaoZhi="HillShade";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnTRI()
{
	// TODO: �ڴ���������������
	BiaoZhi="TRI";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnRoughness()
{
	// TODO: �ڴ���������������
	BiaoZhi="Roughness";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnMultiScaleSegment()
{
	// TODO: �ڴ���������������
	typedef BOOL (/*WINAPI */* TESTDLL)(CString);
	HINSTANCE hmod;
	hmod = ::LoadLibrary ("TestDll.dll");
	if(hmod==NULL)
	{
		AfxMessageBox("����DLLʧ��");
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
		AfxMessageBox("�޷����غ���");
	}

}


void CrspDlg::OnWatershed()
{
	// TODO: �ڴ���������������
	BiaoZhi="Watershed";	
	CArthmetic math;
	math.DoModal();
}


void CrspDlg::OnLinearStrecth()
{
	// TODO: �ڴ���������������
	//��������������öԻ���
	flag1=6;
	if(linesdlgPara.DoModal()==IDOK)
	{
		//��ȡ�û����õ���������ת�۵�
		CPoint point1,point2;
		point1.x=linesdlgPara.m_nX1;
		point1.y=linesdlgPara.m_nY1;
		point2.x =linesdlgPara.m_nX2;
		point2.y=linesdlgPara.m_nY2;

		//�жϲ����ĺ�����
		if(point1.x<0||point1.x>255||point2.x<0||point2.x>255
			||point1.y<0||point1.y>255||point2.y<0||point2.y>255
			||point1.x>point2.x)
		{
				::MessageBox(0,"����������",MB_OK,0);
				flag1=-1;//�˳�
				return;
		}
	}
	imageS->Invalidate(FALSE);
}


void CrspDlg::OnPointLiner()
{
	// TODO: �ڴ�������������
	flag1=7;
	// ��ʼ������ֵ
	dlgPara.m_fA = 2.0;
	dlgPara.m_fB = -128.0;

	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	imageS->Invalidate(FALSE);
}


void CrspDlg::OnMorphErosion()
{
	// TODO: �ڴ���������������
	flag1=8;
	// ��ʼ������ֵ
	dlgPara1.m_nMode = 0;

	if (dlgPara1.DoModal() != IDOK)
	{
		// ����
		return;
	}
	imageS->Invalidate(FALSE);
}


void CrspDlg::OnMorphDilation()
{
	// TODO: �ڴ���������������
	flag1=9;
	// ��ʼ������ֵ
	dlgPara2.m_nMode = 0;

	if (dlgPara2.DoModal() != IDOK)
	{
		// ����
		return;
	}
	imageS->Invalidate(FALSE);

}


void CrspDlg::OnMorphOpen()
{
	// TODO: �ڴ���������������
	flag1=10;
	// ��ʼ������ֵ
	dlgPara3.m_nMode = 0;

	if (dlgPara3.DoModal() != IDOK)
	{
		// ����
		return;
	}
	imageS->Invalidate(FALSE);

}


void CrspDlg::OnMorphClose()
{
	// TODO: �ڴ���������������
	flag1=11;
	// ��ʼ������ֵ
	dlgPara4.m_nMode = 0;

	if (dlgPara4.DoModal() != IDOK)
	{
		// ����
		return;
	}
	imageS->Invalidate(FALSE);
}


void CrspDlg::OnMorphThining()
{
	// TODO: �ڴ���������������
	flag1=12;
	imageS->Invalidate(FALSE);
}


void CrspDlg::OnHitAndMiss()
{
	// TODO: �ڴ���������������
	flag1=13;
	imageS->Invalidate(FALSE);
}


void CrspDlg::OnAboutAuthor()
{
	// TODO: �ڴ���������������
	ShellExecute(NULL, "open", "http://weibo.com/u/1194046363?wvr=3.6&lf=reg",NULL, NULL, SW_SHOWNORMAL);
}


void CrspDlg::OnMiDuSeg()
{
	// TODO: �ڴ���������������
	flag1=14;
	imageS->Invalidate(FALSE);
}

//���ز��
void CrspDlg::OnMenuLoadPlug()
{
	// TODO: �ڴ���������������
	CString PlugPath;
	CFileDialog dlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, "���(*.dll)|*.dll||");
	dlg.m_ofn.lpstrTitle="����Ӳ��";

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
	int Pos = FindMenuItem(pMenu,_T("�������"));
	if (-1 == Pos)
	{
		return;
	}
	CMenu *pSubMenu = pMenu->GetSubMenu(Pos);
	int count=	pSubMenu->GetMenuItemCount();			
	int i=0;
	for (std::map<std::string, Plugin>::iterator it=k1.m_LoadedPlugins.begin(); it != k1.m_LoadedPlugins.end(); it++ )
	{
		//�������
		CString Engname=(*it).second.getEngineName();
		if(FindMenuItem(pSubMenu,Engname)>0||FindMenuItem(pSubMenu,Engname)==0)//�жϵ�ǰ����Ƿ��Ѿ����أ����δ��������뵽��ǰ���������
			return;
		pSubMenu->InsertMenu(i,MF_BYPOSITION,ID_MENUSTART+i,(*it).second.getEngineName());
		i++;
	}
}

int CrspDlg::FindMenuItem(CMenu* Menu, CString MenuString)
{
	ASSERT(Menu);
	ASSERT(::IsMenu(Menu->GetSafeHmenu()));
	int count = Menu->GetMenuItemCount();//���ظò˵�������
	for (int i = 0; i < count; i++)
	{
		CString str;
		if (Menu->GetMenuString(i, str, MF_BYPOSITION) &&str.Compare(MenuString) == 0)//��ָ���˵���ı�ǩ������ָ���Ļ�����
			return i;
	}
	return -1;
}

void CrspDlg::OnMenuDeal(UINT nID)
{
	CMenu *pMenu = AfxGetMainWnd()->GetMenu();
	int Pos = FindMenuItem(pMenu,_T("�������"));
	CMenu *pSubMenu = pMenu->GetSubMenu(Pos);
	CString DllName;
	//���������ҵ�����
	pSubMenu->GetMenuString(nID,DllName,MF_BYCOMMAND);//��ȡ�������
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

//��ʼ��������������ز���ļ������еĲ��
void CrspDlg::InitDll()
{
	//�洢�ļ����ı���
	CStringArray FileName;
	//����·��ΪӦ�ó���ĸ�Ŀ¼
	CString Pluginpath = GetPluginPath() + _T("*.dll");
	//��*.exe���в���
	CFileFind pFile;
	BOOL BeWork = pFile.FindFile(Pluginpath);
	while(BeWork)
	{
		BeWork = pFile.FindNextFile();
		if (pFile.IsDots())//���������Ҫ������
		{
			continue;
		}
		FileName.Add(pFile.GetFilePath());//����ҵ���ӽ�������
	}
	pFile.Close();
	//���û���ҵ�֮��������ֱ�ӷ���
	if (0 == FileName.GetCount())
	{
		return;
	}

	//��̬��Ӳ˵�
	CMenu *pMenu = GetMenu();
	//�����ҵ���
	int Pos = FindMenuItem(pMenu,_T("�������"));
	if (-1 == Pos)
	{
		return;
	}
	//�ҵ����������
	CMenu *pSubMenu = pMenu->GetSubMenu(Pos);
	for (int i = 0;i<FileName.GetCount();i++)
	{
		//�������
		//string strName=CString(FileName.GetAt(i)).GetBuffer(0);
		k1.loadPlugin(CString(FileName.GetAt(i)).GetBuffer(0));
		std::map<std::string, Plugin>::iterator it=k1.m_LoadedPlugins.begin();
		pSubMenu->InsertMenu(i,MF_BYPOSITION,ID_MENUSTART+i,(*it).second.getEngineName());
		it++;
	}
	//�����ɺ��������
	FileName.RemoveAll();
}

//��д����ʶ��
void CrspDlg::OnDigitRecog()
{
	// TODO: �ڴ���������������
	CDigitRecogDlg dlg;
	dlg.DoModal();
}
