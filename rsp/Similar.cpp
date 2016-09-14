// Similar.cpp : 实现文件
//

#include "stdafx.h"
#include "rsp.h"
#include "Similar.h"
#include "afxdialogex.h"


// CSimilar 对话框

IMPLEMENT_DYNAMIC(CSimilar, CDialogEx)

CSimilar::CSimilar(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSimilar::IDD, pParent)
{
	m_T = 0.0;
}

CSimilar::~CSimilar()
{
}

void CSimilar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_T);
}


BEGIN_MESSAGE_MAP(CSimilar, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSimilar::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CSimilar::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSimilar::OnBnClickedButton2)
END_MESSAGE_MAP()


// CSimilar 消息处理程序


void CSimilar::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	int num=1;//记录聚类中心个数
	GDALDataset* poDataset;//GDAL数据集
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(Similar_IN,GA_ReadOnly);

	if(poDataset==NULL)
	{
		AfxMessageBox(_T("打开文件失败！！！"));
		return;
	}

	GDALRasterBand* Band1;
	int nImageSizeX=poDataset->GetRasterXSize();
	int nImageSizeY=poDataset->GetRasterYSize();

	BYTE *InData1 = (BYTE*)CPLMalloc((nImageSizeX)*(nImageSizeY));

	Band1=poDataset->GetRasterBand(1);
	Band1->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData1,nImageSizeX,nImageSizeY, GDT_Byte,0,0);

	//取影像中间的像素点作为第一个聚类中心
	center.data.push_back(InData1[int((nImageSizeX*nImageSizeY)/2)]);
	sample=new Sim_sample[nImageSizeX*nImageSizeY];

	BeginWaitCursor();

	for (int i=0;i<nImageSizeX*nImageSizeY;i++)//给每个样本赋值
	{
		sample[i].data=InData1[i];
		sample[i].mark=-1;
	}

	sample[int((nImageSizeX*nImageSizeY)/2)].mark=0;

	for (int i=0;i<nImageSizeX*nImageSizeY;i++)
	{
		if (((sample[i].data-center.data[0])*(sample[i].data-center.data[0]))>(m_T*m_T))
		{
			center.data.push_back(sample[i].data);
			sample[i].mark=num++;
		}
		else
		{
			sample[i].mark=0;
		}
	}

	for (int i=0;i<nImageSizeX*nImageSizeY;i++)
	{
		if (sample[i].mark==-1)
		{
			int k=0;//判断样本到各个聚类中心的距离是否都大于T
			int m;//记录聚类中心下标
			double minDis=360000000000000000;
			for (int j=0;j<center.data.size();j++)
			{   
				double dis=(sample[i].data-center.data[j])*(sample[i].data-center.data[j]);
				if(dis>(m_T*m_T))
				{
					k=k+1;
				}
				if (minDis>dis)
				{
					minDis=dis;
					m=j;//距离聚类中心最小的那类的编号
				}
			}
			if (k==center.data.size())
			{
				center.data.push_back(sample[i].data);
				sample[i].mark=num++;
			}
			else
			{
				sample[i].mark=m;
			}
		}
	}

	BYTE value=255/(num);
	BYTE *OutData = new BYTE[nImageSizeX*nImageSizeY];//输出结果
	for (int i=0;i<nImageSizeY;i++)
	{
		for (int j=0;j<nImageSizeX;j++)
		{
			OutData[i*nImageSizeY+j]=sample[i*nImageSizeY+j].mark*value;
		}
	}

	////////////////////////////////将输出影像存储为一个文件//////////////////////////
	char* pszFormat;
	if(Similar_OUT.Right(4)==".BMP")
	{
		pszFormat = "BMP";
	}	

	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	GDALDataset *poDstDS;
	char **papszOptions =poDriver->GetMetadata();
	poDstDS = poDriver->Create( (char*)(LPCTSTR)Similar_OUT,//输出路径
		nImageSizeX,nImageSizeY, //影像数据宽高
		1, //存或取的波段数
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
		1,						//存或取的波段数
		NULL,							//存或取的波段列表
		0,								//缓冲区数据一个像素占用的字节数
		0,								//缓冲区一行数据所占用的字节数
		0)==CE_Failure )				//缓冲区数据存储方式 0为RRRRGGGGBBBB-bsp 1为RGBRGBRGB-bip	
	{
		AfxMessageBox("保存文件失败！");
		return;
	}
	else
	{
		AfxMessageBox("分类完成！");
	}

	GDALClose(poDstDS);
	CPLFree(InData1);
	delete []OutData;
	delete []sample;


	CDialogEx::OnOK();
}

//输入路径
void CSimilar::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.bmp)|*.bmp|JPG文件(*.jpg)|*.jpg;*.jpeg|tiff文件(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="请输入待分类遥感影像";
	if (file.DoModal()==IDOK)
	{
		Similar_IN=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_SIMLIN,Similar_IN);
}

//输出路径
void CSimilar::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(false, "BMP", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.BMP)|*.BMP|JPG文件(*.JPG)|*.JPG;*.JPEG|tiff文件(*.TIFF)|*.TIFF;*.TIF|img(*.IMG)|*.IMG|hdr(*.HDR)|*.HDR||");
	file.m_ofn.lpstrTitle="请输入分类结果";
	if (file.DoModal()==IDOK)
	{
		Similar_OUT=file.GetPathName();
		switch(file.m_ofn.nFilterIndex)
		{
		case 2:
			{
				if (Similar_OUT.Right(4)!=".BMP")
				{
					Similar_OUT=Similar_OUT+".BMP";
				}
				break;
			}
		}

	}
	SetDlgItemText(IDC_STATIC_SIMLOUT,Similar_OUT);

}


BOOL CSimilar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
