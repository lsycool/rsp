// ChangeDetect.cpp : 实现文件
//

#include "stdafx.h"
#include "rsp.h"
#include "ChangeDetect.h"
#include "afxdialogex.h"
#include "rspDlg.h"
#include "ImageShow.h"

CString FPATH1;//变化前文件路径
CString LPATH1;//变化后文件路径
CString RPATH;//结果路径
// ChangeDetect 对话框

IMPLEMENT_DYNAMIC(ChangeDetect, CDialogEx)

ChangeDetect::ChangeDetect(CWnd* pParent /*=NULL*/)
	: CDialogEx(ChangeDetect::IDD, pParent)
{

}

ChangeDetect::~ChangeDetect()
{
}

void ChangeDetect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ChangeDetect, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_FORMER, &ChangeDetect::OnBnClickedButtonFormer)
	ON_BN_CLICKED(IDC_BUTTON_LATTER, &ChangeDetect::OnBnClickedButtonLatter)
	ON_BN_CLICKED(IDC_BUTTON_RESULT, &ChangeDetect::OnBnClickedButtonResult)
	ON_BN_CLICKED(IDOK, &ChangeDetect::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ChangeDetect::OnBnClickedCancel)
END_MESSAGE_MAP()


// ChangeDetect 消息处理程序


BOOL ChangeDetect::OnInitDialog()
{
	CDialogEx::OnInitDialog();
//	ShowWindow(SW_SHOW);
	// TODO:  在此添加额外的初始化
	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void ChangeDetect::OnBnClickedButtonFormer()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.bmp)|*.bmp|JPG文件(*.jpg)|*.jpg;*.jpeg|tiff文件(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="请输入变化前遥感影像";
	if (file.DoModal()==IDOK)
	{
		FPATH1=file.GetPathName();
		CStatic* cs=(CStatic*)GetDlgItem(IDC_STATIC_FORMER);
		cs->SetWindowText(FPATH1);
	}
}


void ChangeDetect::OnBnClickedButtonLatter()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.bmp)|*.bmp|JPG文件(*.jpg)|*.jpg;*.jpeg|tiff文件(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="请输入变化后遥感影像";
	if (file.DoModal()==IDOK)
	{
		LPATH1=file.GetPathName();
		CStatic* cs=(CStatic*)GetDlgItem(IDC_STATIC_LATTER);
		cs->SetWindowText(LPATH1);
	}
}


void ChangeDetect::OnBnClickedButtonResult()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(false, "BMP", "检测结果", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.BMP)|*.BMP|JPG文件(*.JPG)|*.JPG;*.JPEG|tiff文件(*.TIFF)|*.TIFF;*.TIF|img(*.IMG)|*.IMG|hdr(*.HDR)|*.HDR||");
	file.m_ofn.lpstrTitle="请输入结果影像";
	if (file.DoModal()==IDOK)
	{
		RPATH=file.GetPathName();
		CStatic* cs=(CStatic*)GetDlgItem(IDC_STATIC_RESULT);
		cs->SetWindowText(RPATH);
	}
}


void ChangeDetect::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	BeginWaitCursor();//开始等待光标

	GDALDataset* poDataset;//GDAL数据集
	GDALDataset* poDataset1;//GDAL数据集
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(FPATH1,GA_ReadOnly);
	poDataset1=(GDALDataset*)GDALOpen(LPATH1,GA_ReadOnly);

	if(poDataset==NULL||poDataset1==NULL)
	{
		AfxMessageBox(_T("打开文件失败！！！"));
		return;
	}

	int nImageSizeX=poDataset->GetRasterXSize();
	int nImageSizeY=poDataset->GetRasterYSize();
	int Bands = poDataset->GetRasterCount();

	int nImageSizeX1=poDataset1->GetRasterXSize();
	int nImageSizeY1=poDataset1->GetRasterYSize();
	int Bands1 = poDataset1->GetRasterCount();

	BYTE *InData = new BYTE[nImageSizeX*nImageSizeY*Bands];
	poDataset->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData,nImageSizeX,nImageSizeY, GDT_Byte,Bands, NULL,0,0,0 );

	BYTE *InData1 = new BYTE[nImageSizeX1*nImageSizeY1*Bands1];
	poDataset1->RasterIO( GF_Read, 0, 0,nImageSizeX1,nImageSizeY1, InData1,nImageSizeX1,nImageSizeY1, GDT_Byte,Bands, NULL,0,0,0 );

	BYTE *OutData = new BYTE[nImageSizeX*nImageSizeY*3];//输出结果
	for (int iBands = 0;iBands<1;iBands++)
	{
		for (int i = 0;i<nImageSizeY;i++)
		{
			for (int j = 0;j<nImageSizeX;j++)
			{
				if (InData[nImageSizeY*nImageSizeX*iBands+i*nImageSizeX + j]-InData1[nImageSizeY*nImageSizeX*iBands+i*nImageSizeX + j]!=0)
				{
					OutData[nImageSizeY*nImageSizeX*iBands+i*nImageSizeX + j]=255;
				}
				else
				{
					OutData[nImageSizeY*nImageSizeX*iBands+i*nImageSizeX + j]=0;
				}
			}
		}
	}

	char* pszFormat;
	if(RPATH.Right(4)==".BMP")
	{
		pszFormat = "BMP";
	}	

	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	GDALDataset *poDstDS;
	char **papszOptions =poDriver->GetMetadata();
	poDstDS = poDriver->Create( (char*)(LPCTSTR)RPATH,//输出路径
		nImageSizeX,nImageSizeY, //影像数据宽高
		3, //存或取的波段数
		GDT_Byte, 	//缓冲区数据类型
		papszOptions );

	EndWaitCursor();
	if(poDstDS->RasterIO(
		GF_Write,						//IO方式
		0,0,							//影像数据起始位置x,y
		nImageSizeX,nImageSizeY,				//影像数据宽高
		OutData,						//指向缓冲区数据指针
		nImageSizeX,nImageSizeY,				//缓冲区数据宽高
		GDT_Byte,						//缓冲区数据类型
		3,						//存或取的波段数
		NULL,							//存或取的波段列表
		0,								//缓冲区数据一个像素占用的字节数
		0,								//缓冲区一行数据所占用的字节数
		0)==CE_Failure )				//缓冲区数据存储方式 0为RRRRGGGGBBBB-bsp 1为RGBRGBRGB-bip	
	{
		AfxMessageBox("保存文件失败！");
		return;
	}

	else
		AfxMessageBox("变化检测成功！");
	GDALClose(poDstDS);
	delete []OutData;
	delete []InData;
	delete []InData1;

	path=RPATH;
	ImageShow* image=new ImageShow;
	image->Create(IDD_IMAGE_SHOW);
	image->ShowWindow(SW_SHOW);
	CDialogEx::OnOK();
}


void ChangeDetect::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	DestroyWindow();
//	CDialogEx::OnCancel();
}
