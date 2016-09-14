// Kmeans.cpp : 实现文件
//

#include "stdafx.h"
#include "rsp.h"
#include "Kmeans.h"
#include <math.h>
#include <afxwin.h>
#include "afxdialogex.h"

int l=1000;//最大迭代次数
int M_num=2;//初始聚类中心

CString KOPath;//输入影像文件路径
CString KRPath;//输出文件路径
// Kmeans 对话框

IMPLEMENT_DYNAMIC(Kmeans, CDialogEx)

Kmeans::Kmeans(CWnd* pParent /*=NULL*/)
	: CDialogEx(Kmeans::IDD, pParent)
{

	//  m_Kvalue = 0;
	m_DieD = 0;
	m_Kvalue = 0;
}

Kmeans::~Kmeans()
{
}

void Kmeans::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT1, m_Kvalue);
	DDX_Text(pDX, IDC_EDIT2, m_DieD);
	DDX_Text(pDX, IDC_EDIT1, m_Kvalue);
}


BEGIN_MESSAGE_MAP(Kmeans, CDialogEx)
	ON_BN_CLICKED(IDOK, &Kmeans::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &Kmeans::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Kmeans::OnBnClickedButton2)
END_MESSAGE_MAP()


// Kmeans 消息处理程序


void Kmeans::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	l=m_DieD;//最大迭代次数
	M_num=m_Kvalue;//初始聚类中心

	BeginWaitCursor();//开始等待光标

	m_Progress->SetRange(0,100);

	GDALDataset* poDataset;//GDAL数据集
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(KOPath,GA_ReadOnly);
	if(poDataset==NULL)
	{
		AfxMessageBox(_T("打开文件失败！！！"));
		return;
	}

	//遥感的三个波段
	GDALRasterBand* Band1;
	GDALRasterBand* Band2;
	GDALRasterBand* Band3;

	int nImageSizeX=poDataset->GetRasterXSize();
	int nImageSizeY=poDataset->GetRasterYSize();
	int Bands = poDataset->GetRasterCount();

	m_Progress->SetPos(5);//设置进度条进度
	VQ_VECTOR* samp=new VQ_VECTOR[nImageSizeX*nImageSizeY];

	BYTE *InData1 = new BYTE[nImageSizeX*nImageSizeY];
	BYTE *InData2 = new BYTE[nImageSizeX*nImageSizeY];
	BYTE *InData3 = new BYTE[nImageSizeX*nImageSizeY];
	if (Bands==1)
	{
		samp[0].nDimension=1;
		Band1=poDataset->GetRasterBand(1);
		Band1->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData1,nImageSizeX,nImageSizeY, GDT_Byte,0,0);
	}
	else
	{
		samp[0].nDimension=3;
		Band1=poDataset->GetRasterBand(1);
		Band2=poDataset->GetRasterBand(2);
		Band3=poDataset->GetRasterBand(3);

		Band1->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData1,nImageSizeX,nImageSizeY, GDT_Byte,0,0);
		Band2->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData2,nImageSizeX,nImageSizeY, GDT_Byte,0,0);
		Band3->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData3,nImageSizeX,nImageSizeY, GDT_Byte,0,0);
	}

	int Ndimension=samp[0].nDimension;

	for (int j=0;j<nImageSizeX*nImageSizeY;j++)
	{
		samp[j].Data=new BYTE[Ndimension];
		for (int i=0;i<Ndimension;i++)	
		{
			if (i==0)
			{
				samp[j].Data[i]=InData1[j];
			}
			if (i==1)
			{
				samp[j].Data[i]=InData2[j];
			}
			if (i==2)
			{
				samp[j].Data[i]=InData3[j];
			}
		}
	}
	m_Progress->SetPos(50);//设置进度条进度

	VQ_CENTER* m_center=new VQ_CENTER[M_num];
	for (int i=0;i<M_num;i++)
	{
		m_center[i].Data=new BYTE[Ndimension];
	}
	LBGCluster(samp,nImageSizeX*nImageSizeY,m_center,M_num);

	BYTE *OutData = new BYTE[nImageSizeX*nImageSizeY*Ndimension];//输出结果
	
	int ColorValue;
	if (M_num!=0)
	{
		ColorValue = 255/M_num;//给每一个类分配颜色值
	}
	else
	{
		MessageBox("分类数不能为零，请重新输入！！！");
	}
	
	for (int i=0;i<nImageSizeY;i++)
	{
		for (int j=0;j<nImageSizeX;j++)
		{
			OutData[nImageSizeY*nImageSizeX*0+i*nImageSizeX + j]=ColorValue*samp[i*nImageSizeX + j].nCluster;
			OutData[nImageSizeY*nImageSizeX*1+i*nImageSizeX + j]=(ColorValue/2)*samp[i*nImageSizeX + j].nCluster;
			OutData[nImageSizeY*nImageSizeX*2+i*nImageSizeX + j]=(ColorValue/4)*samp[i*nImageSizeX + j].nCluster;
		}
	}	
	m_Progress->SetPos(80);

	////////////////////////////////将输出影像存储为一个文件//////////////////////////
	char* pszFormat;
	if(KRPath.Right(4)==".BMP")
	{
		pszFormat = "BMP";
	}	

	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	GDALDataset *poDstDS;
	char **papszOptions =poDriver->GetMetadata();
	poDstDS = poDriver->Create( (char*)(LPCTSTR)KRPath,//输出路径
		nImageSizeX,nImageSizeY, //影像数据宽高
		3, //存或取的波段数
		GDT_Byte, 	//缓冲区数据类型
		papszOptions );

	EndWaitCursor();
	m_Progress->SetPos(100);

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
	{
		m_Progress->SetPos(100);
		AfxMessageBox("分类完成！");
	}

	GDALClose(poDstDS);
	delete []OutData;
	delete []InData1;
	delete []InData2;
	delete []InData3;
	delete []samp;
	delete []m_center;

	CDialogEx::OnOK();
}

/************************************************************************/
/* 在欧氏距离的基础上用LBG算法将输入向量聚类
参数：
x：输入向量
n：输入向量个数
y：聚类结果
m：聚类中心个数
返回值：
0：正确
1：错误                                                                     */
/************************************************************************/
int LBGCluster(VQ_VECTOR* X,int N,VQ_CENTER* Y,int M)
{
	//判断是否需要聚类
	if (N<M)
	{
		return -1;
	}

	//l是迭代的最大次数，可以修改
	int m=1,nCenter,i,j,k=0;
	int nDimension=X[0].nDimension;
	double D0,D;
	//定义VQ_CENTERINFO结构，保存算法中间过程的变量
	struct VQ_CENTERINFO
	{
		double* Data;
		int nDimension;
		double* SumData;
		int Num;
	};
	VQ_CENTERINFO* Center=(VQ_CENTERINFO*)malloc(M*sizeof(VQ_CENTERINFO));
	//判断内存是否申请成功 
	if (Center==NULL)
	{
		return -1;
	}

	double* Distance=(double*)malloc(N*sizeof(double));

	if (Distance==NULL)
	{
		return -1;
	}

	for (i=0;i<M;i++)
	{
		Center[i].nDimension=nDimension;
		Center[i].Data=(double*)malloc(sizeof(double)*nDimension);
		Center[i].SumData=(double*)malloc(sizeof(double)*nDimension);
		if (Center[i].Data==0||Center[i].SumData==NULL)
		{
			AfxMessageBox("内存不足!");
			return -1;
		}

		//等间隔初始化聚类中心
		for(j=0;j<nDimension;j++)
		{
			Center[i].Data[j]=X[i*N/M].Data[j];
			Center[i].SumData[j]=0;
		}
		Center[i].Num=0;
	}
	D0=1;
	D=1e+10;
	//循环，1e-5误差允许值，可以修改
	while (m<l,fabs(D0-D)/D0>1e-5)
	{
		for (i=0;i<M;i++)
		{
			for(j=0;j<nDimension;j++)
				Center[i].SumData[j]=0;
			Center[i].Num=0;
		}
		D0=D;
		D=0;
		m++;

		for(i=0;i<N;i++)
		{
			Distance[i]=1e+10;
			for(j=0;j<M;j++)
			{
				double Dist=0;
				for (k=0;k<nDimension;k++)
				{
					Dist+=(X[i].Data[k]-Center[j].Data[k]*(X[i].Data[k]-Center[j].Data[k]));//计算每个输入向量到聚类中心的距离
				}
				if (Dist<Distance[i])
				{
					nCenter=j;//记录类号
					Distance[i]=Dist;
				}
			}

			X[i].nCluster=nCenter;//确定向量所属的类

			for (k=0;k<nDimension;k++)
			{
				Center[nCenter].SumData[k]+=X[i].Data[k];//计算每个类中所有样本的和，用于重新确定类中心
			}
			Center[nCenter].Num++;//保存类中元素个数
			D+=Distance[i];//保存每个元素到类中心最小距离的和
		}

		for (i=0;i<M;i++)//M:聚类中心个数
		{
			if (Center[i].Num!=0)
			{
				for (k=0;k<nDimension;k++)
				{
					Center[i].Data[k]=Center[i].SumData[k]/Center[i].Num;//计算每个类的均值
				}
			}
			else
			{
				int MaxNum=0;
				for (k=1;k<M;k++)
				{
					MaxNum=Center[i].Num>Center[MaxNum].Num? i:MaxNum;
				}
				int Num=Center[MaxNum].Num/2;
				for (k=0;k<nDimension;k++)
				{
					Center[MaxNum].SumData[k]=0;
				}
				Center[MaxNum].Num=0;

				for (k=0;k<N;k++)//N:输入向量个数
				{
					if (X[k].nCluster!=MaxNum)
					{
						continue;
					}
					if (Center[i].Num<Num)
					{
						X[k].nCluster=i;
						for (m=0;m<nDimension;m++)
						{
							Center[i].SumData[m]+=X[k].Data[m];
						}
						Center[i].Num++;
					}
					else
					{
						for (m=0;m<nDimension;m++)
						{
							Center[MaxNum].SumData[m]+=X[k].Data[m];
						}
						Center[MaxNum].Num++;
					}
				}
				for (m=0;m<nDimension;m++)
				{
					Center[i].Data[m]=Center[i].SumData[m]/Center[i].Num;
				}
				if (MaxNum<i)
				{
					for (m=0;m<nDimension;m++)
					{
						Center[MaxNum].Data[m]=Center[MaxNum].SumData[m]/Center[MaxNum].Num;
					}
				}

			}
		}
	}

	for (i=0;i<M;i++)
	{
		for (m=0;m<nDimension;m++)
		{
			Y[i].Data[m]=Center[i].Data[m];
		}
		Y[i].Num=Center[i].Num;
	}

	for (i=0;i<M;i++)
	{
		free(Center[i].Data);
		free(Center[i].SumData);
	}
	free(Center);
	free(Distance);


	return 0;
}

//输入文件路径
void Kmeans::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.bmp)|*.bmp|JPG文件(*.jpg)|*.jpg;*.jpeg|tiff文件(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="请输入待分类遥感影像";
	if (file.DoModal()==IDOK)
	{
		KOPath=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_INK,KOPath);
}

//输出文件路径
void Kmeans::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(false, "BMP", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.BMP)|*.BMP|JPG文件(*.JPG)|*.JPG;*.JPEG|tiff文件(*.TIFF)|*.TIFF;*.TIF|img(*.IMG)|*.IMG|hdr(*.HDR)|*.HDR||");
	file.m_ofn.lpstrTitle="请输入分类结果";
	if (file.DoModal()==IDOK)
	{
		KRPath=file.GetPathName();
		switch(file.m_ofn.nFilterIndex)
		{
		case 2:
			{
				if (KRPath.Right(4)!=".BMP")
				{
					KRPath=KRPath+".BMP";
				}
				break;
			}
		case 4:
			{
				if (KRPath.Right(5)!=".TIFF")
				{
					KRPath=KRPath+".TIFF";
				}
				break;
			}

		}

	}
	SetDlgItemText(IDC_STATIC_OUTK,KRPath);
}


BOOL Kmeans::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Progress=(CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	m_Progress->SendMessage(PBM_SETBARCOLOR,0,RGB(251,4,29));
	m_Progress->ModifyStyleEx(WS_EX_STATICEDGE,0);
	m_Progress->Invalidate(FALSE);

	m_DieD=30;
	m_Kvalue=20;
	UpdateData(FALSE);
	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
