// MinMax.cpp : 实现文件
//
/************************************************************************/
/* 在特征矢量集中以最大距离原则选取新的聚类中心，以最小距离原则进行模式归类                                                                     */
/************************************************************************/

#include "stdafx.h"
#include "rsp.h"
#include "MinMax.h"
#include "afxdialogex.h"
#include <math.h>
#include<afx.h>

CString MaMi_in;//输入文件路径
CString MaMi_out;//输出文件路径

// MinMax 对话框

IMPLEMENT_DYNAMIC(MinMax, CDialogEx)

MinMax::MinMax(CWnd* pParent /*=NULL*/)
	: CDialogEx(MinMax::IDD, pParent)
{

	//  m_sida = 0.0f;
	m_sida = 0.0f;
}

MinMax::~MinMax()
{
}

void MinMax::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SIDA, m_sida);
	DDV_MinMaxFloat(pDX, m_sida, 0, 1);
}


BEGIN_MESSAGE_MAP(MinMax, CDialogEx)
	ON_BN_CLICKED(IDOK, &MinMax::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &MinMax::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &MinMax::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &MinMax::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &MinMax::OnBnClickedButton4)
END_MESSAGE_MAP()


// MinMax 消息处理程序


void MinMax::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int num=0;//记录聚类中心个数
	UpdateData(TRUE);

	GDALDataset* poDataset;//GDAL数据集
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(MaMi_in,GA_ReadOnly);

	if(poDataset==NULL)
	{
		AfxMessageBox(_T("打开文件失败！！！"));
		return;
	}

	m_progress->SetRange(0,100);

	GDALRasterBand* Band1;
	int nImageSizeX=poDataset->GetRasterXSize();
	int nImageSizeY=poDataset->GetRasterYSize();

	BYTE *InData1 = new BYTE[nImageSizeX*nImageSizeY];
	msample=new MaSample[nImageSizeX*nImageSizeY];

	mclass=new MaClass[nImageSizeX*nImageSizeY];
//	mclass[num].sample.push_back(&InData1[0]);

	Band1=poDataset->GetRasterBand(1);
	Band1->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData1,nImageSizeX,nImageSizeY, GDT_Byte,0,0);

	mclass[0].data=InData1[0];

	BeginWaitCursor();
	m_progress->SetPos(10);

	for (int i=0;i<nImageSizeY;i++)
	{
		for (int j=0;j<nImageSizeX;j++)
		{
			msample[i*nImageSizeY+j].data=InData1[i*nImageSizeY+j];
			msample[i*nImageSizeY+j].mark=0;
		}
	}

	double Dis12=0;//记录最小类间距
	double maxDis=0;//最小距离中的最大值

	int z1;//记录第二个聚类中心

	for(int i=0;i<nImageSizeY;i++)
	{
		for (int j=0;j<nImageSizeX;j++)
		{
			//	double dis=(InData1[0]-InData1[j])*(InData1[0]-InData1[j]);//计算距离第一个像素最远的像素
			double dis=(msample[0].data-InData1[i*nImageSizeY+j])*(msample[0].data-InData1[i*nImageSizeY+j]);
			dis=sqrt(dis);

			if (maxDis<dis)
			{
				maxDis=dis;
				Dis12=dis;
				z1=j;
			}
		}
	}
	int k=++num;
//	mclass[k].sample.push_back(&InData1[z1]);
	mclass[k].data=InData1[z1];

loop1:
	int z3;//记录其他聚类中心
	int maxDis1=0;
	for (int i=0;i<nImageSizeX*nImageSizeY;i++)
		{
			////计算每个样本点到两个类的最小距离
			//double dis1=((InData1[i]-InData1[0])*(InData1[i]-InData1[0]));
			//double dis2=((InData1[i]-InData1[z1])*(InData1[i]-InData1[z1]));

			//计算每个样本点到各个聚类中心的距离
			double dis1,dis2;
			double mind=3276700000;//记录最小类间距
			for (int i1=0;i1<=num;i1++)
			{
				dis1=(InData1[i]-mclass[i1].data)*(InData1[i]-mclass[i1].data);
				dis1=sqrt(dis1);
				if (dis1<mind)
				{
					mind=dis1;
				}
			}
			if (maxDis1<mind)
			{
				maxDis1=mind;
				z3=i;
			}
			/*if (dis1>=dis2)
			{
			dis1=dis2;
			dis1=sqrt(dis1);
			}
			else
			{
			dis1=sqrt(dis1);
			}

			if (maxDis<dis1)
			{
			maxDis=dis1;
			z3=i;
			}*/
		}
	
	if (maxDis1>m_sida*Dis12)
	{
		int k1=++num;
//		mclass[k1].sample.push_back(&InData1[z3]);
		mclass[k1].data=InData1[z3];
		goto loop1;
	}
	else
	{
		goto loop;
	}


loop:	
	for (int i=0;i<nImageSizeX*nImageSizeY;i++)//计算每个样本点到各个聚类中心的距离，并归类
		{
			int n1=0;//记录所属的类号
			double mind=3276700000;//记录最小类间距

			for (int j=0;j<=num;j++)
			{
				double dis1=InData1[i]-mclass[j].data;
				dis1=sqrt(dis1);

				if (mind>dis1)
				{
					mind=dis1;
					n1=j;
				}
			}
			msample[i].mark=n1;
			mclass[n1].sample.push_back(&msample[i]);
		}

		m_progress->SetPos(50);

		delete []InData1;
		delete []mclass;

		BYTE value=255/(num+1);
		BYTE *OutData = new BYTE[nImageSizeX*nImageSizeY];//输出结果
		for (int i=0;i<nImageSizeY;i++)
		{
			for (int j=0;j<nImageSizeX;j++)
			{
				OutData[i*nImageSizeY+j]=msample[i*nImageSizeY+j].mark*value;
			}
		}

		delete []msample;


		////////////////////////////////将输出影像存储为一个文件//////////////////////////
		char* pszFormat;
		if(MaMi_out.Right(4)==".BMP")
		{
			pszFormat = "BMP";
		}	

		GDALDriver *poDriver;
		poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
		GDALDataset *poDstDS;
		char **papszOptions =poDriver->GetMetadata();
		poDstDS = poDriver->Create( (char*)(LPCTSTR)MaMi_out,//输出路径
			nImageSizeX,nImageSizeY, //影像数据宽高
			1, //存或取的波段数
			GDT_Byte, 	//缓冲区数据类型
			papszOptions );

		EndWaitCursor();

		m_progress->SetPos(100);

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
		delete []OutData;
	CDialogEx::OnOK();
}


BOOL MinMax::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_progress=(CProgressCtrl*)GetDlgItem(IDC_PROGRESS_MinMax);
	m_progress->SendMessage(PBM_SETBARCOLOR,0,RGB(251,4,29));
	m_progress->ModifyStyleEx(WS_EX_STATICEDGE,0);
	m_progress->Invalidate(FALSE);

	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//输入文件
void MinMax::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.bmp)|*.bmp|JPG文件(*.jpg)|*.jpg;*.jpeg|tiff文件(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="请输入待分类遥感影像";
	if (file.DoModal()==IDOK)
	{
		MaMi_in=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_INMaMi,MaMi_in);
}

//输出文件
void MinMax::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(false, "BMP", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.BMP)|*.BMP|JPG文件(*.JPG)|*.JPG;*.JPEG|tiff文件(*.TIFF)|*.TIFF;*.TIF|img(*.IMG)|*.IMG|hdr(*.HDR)|*.HDR||");
	file.m_ofn.lpstrTitle="请输入分类结果";
	if (file.DoModal()==IDOK)
	{
		MaMi_out=file.GetPathName();
		switch(file.m_ofn.nFilterIndex)
		{
		case 2:
			{
				if (MaMi_out.Right(4)!=".BMP")
				{
					MaMi_out=MaMi_out+".BMP";
				}
				break;
			}
		case 4:
			{
				if (MaMi_out.Right(5)!=".TIFF")
				{
					MaMi_out=MaMi_out+".TIFF";
				}
				break;
			}

		}

	}
	SetDlgItemText(IDC_STATIC_OUTMaMi,MaMi_out);

}

//计算回归方程
void MinMax::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码	
//	((CStatic*)GetDlgItem(IDC_STATIC_pic))->SetBitmap(::LoadImage(NULL,"F:\\课件\\定量遥感作业\\植被覆盖度\\QQ截图20130608230418.bmp",IMAGE_BITMAP,256,256,LR_LOADFROMFILE));


	double a,b;//存储回归方程结果
	float LAI=0;//计算LAI

	GDALDataset* poDataset;//GDAL数据集
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(MaMi_in,GA_ReadOnly);

	if(poDataset==NULL)
	{
		AfxMessageBox(_T("打开文件失败！！！"));
		return;
	}

	GDALRasterBand* Band1;
	GDALRasterBand* Band2;
	nImageSizeX=poDataset->GetRasterXSize();
	nImageSizeY=poDataset->GetRasterYSize();

	BYTE *InData1 = new BYTE[nImageSizeX*nImageSizeY];
	BYTE *InData2 = new BYTE[nImageSizeX*nImageSizeY];
	float *InData3 = new float[nImageSizeX*nImageSizeY];

	Band1=poDataset->GetRasterBand(3);
	Band1->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData1,nImageSizeX,nImageSizeY, GDT_Byte,0,0);

	Band2=poDataset->GetRasterBand(4);
	Band2->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData2,nImageSizeX,nImageSizeY, GDT_Byte,0,0);

	BeginWaitCursor();
	LinearRegression(InData1,InData2,nImageSizeX*nImageSizeY,&b,&a);

	CString ca,cb;
	ca.Format("%f",a);
	cb.Format("%f",b);

	CStatic* cab=(CStatic* )GetDlgItem(IDC_STATIC_a);
	cab->SetWindowText(ca);
	cab=(CStatic* )GetDlgItem(IDC_STATIC_b);
	cab->SetWindowText(cb);


	CDC* dc=GetDlgItem(IDC_STATIC_pic)->GetDC();
	Graphics graph(dc->GetSafeHdc());
	Image image(L"QQ截图20130608230418.bmp");
	graph.DrawImage(&image,0,0);
	
	for (int i=0;i<255;i++)
	{
		dc->SetPixel(i,256-(int)(i*a+b),RGB(255,0,0));
	}

	////把lai存入txt中
	//CFile mFile;
	//mFile.Open(MaMi_out+".txt",CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
	//CArchive ar(&mFile,CArchive::store);

	//计算lai
	for (int j=0;j<nImageSizeX*nImageSizeY;j++)
	{
		double min=364325648;
		for(float i=0.01;i<3;i=i+0.01)
		{
			double DNS4=((InData2[j]-255)*exp(0.42*i)+(255-0.0049*InData2[j]))/((1-0.0049*InData2[j]/255)+(0.0049*InData2[j]/255-0.0049)*exp(0.42*i));
			double DNS3=((InData1[j]-255)*exp(1.2*i)+(255-0.0025*InData1[j]))/((1-0.0025*InData1[j]/255)+(0.0025*InData1[j]/255-0.0025)*exp(1.2*i));

			double poor=DNS4-(a*DNS3+b);

			/*if (fabs(poor)<0.0001)
			{*/
			if (min>fabs(poor))
			{
				min=fabs(poor);
				LAI=i;
			}
		}
		cb.Format("%f",LAI);
		cab=(CStatic* )GetDlgItem(IDC_STATIC_LAI);
		cab->SetWindowText(cb);
		/*char sRead[2];
		CFile mFile(_T("user.txt "), CFile::modeWrite|CFile::modeCreate);
		mFile.Write(sRead,2);
		mFile.Flush();
		mFile.Close();*/
	
//		ar<<LAI<<'\r\n';

//		InData3[j]=(LAI/5)*255;
		InData3[j]=LAI;
	}
	////////////////////////////////将输出影像存储为一个文件//////////////////////////
	/*char* pszFormat;
	if(MaMi_out.Right(4)==".BMP")
	{
		pszFormat = "BMP";
	}	*/

//	GDALDriver *poDriver;
//	poDriver = GetGDALDriverManager()->GetDriverByName("BMP");
//	
//	if( poDriver == NULL )   
//	{  
//		MessageBox("无法保存成该文件格式");
//		return;  
//	}  
//
//
//	GDALDataset *poDstDS;
//	char **papszOptions =poDriver->GetMetadata();
////	MaMi_out=MaMi_out-".BMP"+".TIFF";
//	poDstDS = poDriver->Create( (char*)(LPCTSTR)MaMi_out,//输出路径
//		nImageSizeX,nImageSizeY, //影像数据宽高
//		1, //存或取的波段数
//		GDT_Float32, 	//缓冲区数据类型
//		papszOptions );
//
//	EndWaitCursor();
//
//	if(poDstDS->RasterIO(
//		GF_Write,						//IO方式
//		0,0,							//影像数据起始位置x,y
//		nImageSizeX,nImageSizeY,				//影像数据宽高
//		InData3,						//指向缓冲区数据指针
//		nImageSizeX,nImageSizeY,				//缓冲区数据宽高
//		GDT_Float32,						//缓冲区数据类型
//		1,						//存或取的波段数
//		NULL,							//存或取的波段列表
//		0,								//缓冲区数据一个像素占用的字节数
//		0,								//缓冲区一行数据所占用的字节数
//		0)==CE_Failure )				//缓冲区数据存储方式 0为RRRRGGGGBBBB-bsp 1为RGBRGBRGB-bip	
//	{
//		AfxMessageBox("保存文件失败！");
//		return;
//	}
//	else
//	{
//		AfxMessageBox("完成！");
//	}

	GDALDataset* pD=NULL;//数据集句柄
	GDALDatasetH  pDriver;//数据集驱动
	GDALRasterBandH hBand;

	double adfGeoTransform[6]={444720,30,0,3751320,0,-30};
	char* pszSRS_WKT=NULL;
	/*double adfGeoTransform[6];
	poDataset->GetGeoTransform(adfGeoTransform);
	const char* pszSRS_WKT=NULL;*/

	pDriver=GetGDALDriverManager()->GetDriverByName("HFA");
	//创建数据
	pD=(GDALDataset*)GDALCreate(pDriver,(MaMi_out),nImageSizeX,nImageSizeY,1,GDT_Float32,NULL);
	//添加地理信息
	pD->SetGeoTransform(adfGeoTransform);
	//添加投影信息
	pD->SetProjection(pszSRS_WKT);

	hBand=GDALGetRasterBand(pD,1);
	TRY {
		GDALRasterIO(hBand,GF_Write,0,0,nImageSizeX,nImageSizeY,InData3,nImageSizeX,nImageSizeY,GDT_Float32,0,0);
	}
	CATCH (CMemoryException, e)
	{
		AfxMessageBox("影像存储失败！");
	}
	END_CATCH
	
	AfxMessageBox("完成！");

	EndWaitCursor();

	GDALClose(pD);
	GDALClose(pDriver);

	ReleaseDC(dc);
	GDALClose(poDataset);
//	ar.Close();
//	mFile.Close();
	delete []InData1;
	delete []InData2;	
	delete []InData3;

}

int LinearRegression(BYTE* data1,BYTE* data2,int rows, double* a, double* b/*, double *SquarePoor*/)
{
	int m;
	BYTE *p, *q;
	double Lxx = 0.0, Lxy = 0.0, xa = 0.0, ya = 0.0;
	if (data1 == 0||data2==0|| rows < 1)
		return -1;
	for (p = data1,q=data2,m = 0; m < rows; m ++)
	{
		xa += *p ++;
		ya += *q ++;
	}

	xa /= rows;                                     // X平均值
	ya /= rows;                                     // Y平均值
	for (p = data1,q=data2,m = 0; m < rows; m ++, p +=1,q +=1)
	{
		Lxx += ((*p - xa) * (*p - xa));             // Lxx = Sum((X - Xa)平方)
		Lxy += ((*p - xa) * (*q - ya));       // Lxy = Sum((X - Xa)(Y - Ya))
	}
	*b = Lxy / Lxx;                                 // b = Lxy / Lxx
	*a = ya - *b * xa;                              // a = Ya - b*Xa
	/*if (SquarePoor == 0)
		return 0;*/
	// 方差分析
	//SquarePoor[0] = SquarePoor[1] = 0.0;
	//for (p = data, m = 0; m < rows; m ++, p ++)
	//{
	//	Lxy = *a + *b * *p ++;
	//	SquarePoor[0] += ((Lxy - ya) * (Lxy - ya)); // U(回归平方和)
	//	SquarePoor[1] += ((*p - Lxy) * (*p - Lxy)); // Q(剩余平方和)
	//}
	//SquarePoor[2] = SquarePoor[0];                  // 回归方差
	//SquarePoor[3] = SquarePoor[1] / (rows - 2);     // 剩余方差
	return 0;
}

//MDVIg
void MinMax::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	//CFile mFile; 
	//if(mFile.Open(MaMi_in,CFile::modeRead)==0)   
	//	return; 
	//CArchive ar(&mFile,CArchive::load); 

	GDALDataset* poDataset;//GDAL数据集
	GDALDataset* poDataset1;
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(MaMi_in,GA_ReadOnly);
	poDataset1=(GDALDataset*)GDALOpen(MaMi_out,GA_ReadOnly);

	if(poDataset==NULL||poDataset1==NULL)
	{
		AfxMessageBox(_T("打开文件失败！！！"));
		return;
	}

	BeginWaitCursor();
	GDALRasterBand* Band1;
	GDALRasterBand* Band2;

	Band1=poDataset->GetRasterBand(1);
	Band2=poDataset1->GetRasterBand(1);
	
	nImageSizeX=poDataset->GetRasterXSize();
	nImageSizeY=poDataset->GetRasterYSize();


	float *data=new float[nImageSizeX*nImageSizeY];
	float *data1=new float[nImageSizeX*nImageSizeY];
	float *ndvig=new float[nImageSizeX*nImageSizeY];

	Band1->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, data1,nImageSizeX,nImageSizeY, GDT_Float32,0,0);//读取ndvi
	Band2->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, data,nImageSizeX,nImageSizeY, GDT_Float32,0,0);//读取LAT

	CStatic* cab=(CStatic* )GetDlgItem(IDC_STATIC_LAI);
	CString cb;
	for (int i=0;i<nImageSizeX*nImageSizeY;i++)
	{
		/*ar>>data[i]; 
		if (data[i]==0)
		{
			data[i]=0.0000001;
		}*/
		if (data1[i]<0)
		{
			data1[i]=0;
		}
		ndvig[i]=data1[i]/(1-exp(-data[i]));//data1:ndvi;data:LAI

		cb.Format("%f",ndvig[i]);
		cab->SetWindowText(cb);
	}

	/*GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName("HFA");

	if( poDriver == NULL )   
	{  
		MessageBox("无法保存成该文件格式");
		return ;  
	}  */

	CString outre=MaMi_out+"植被覆盖率结果.TIFF";
	//GDALDataset *poDstDS;
	//char **papszOptions =poDriver->GetMetadata();
	//poDstDS = poDriver->Create( (char*)(LPCTSTR)outre,//输出路径
	//	nImageSizeX,nImageSizeY, //影像数据宽高
	//	1, //存或取的波段数
	//	GDT_Float32, 	//缓冲区数据类型
	//	papszOptions );

	//EndWaitCursor();

	//if(poDstDS->RasterIO(
	//	GF_Write,						//IO方式
	//	0,0,							//影像数据起始位置x,y
	//	nImageSizeX,nImageSizeY,				//影像数据宽高
	//	ndvig,						//指向缓冲区数据指针
	//	nImageSizeX,nImageSizeY,				//缓冲区数据宽高
	//	GDT_Float32,						//缓冲区数据类型
	//	1,						//存或取的波段数
	//	NULL,							//存或取的波段列表
	//	0,								//缓冲区数据一个像素占用的字节数
	//	0,								//缓冲区一行数据所占用的字节数
	//	0)==CE_Failure )				//缓冲区数据存储方式 0为RRRRGGGGBBBB-bsp 1为RGBRGBRGB-bip	
	//{
	//	AfxMessageBox("保存文件失败！");
	//	return;
	//}
	//else
	//{
	//	AfxMessageBox("完成！");
	//}
	

	//ar.Close(); 
	//mFile.Close(); 

	GDALDataset* pD=NULL;//数据集句柄
	GDALDatasetH  pDriver;//数据集驱动
	GDALRasterBandH hBand;

	double adfGeoTransform[6]={444720,30,0,3751320,0,-30};
//	double* adfGeoTransform=NULL;
	char* pszSRS_WKT=NULL;
//	poDataset->GetGeoTransform(adfGeoTransform);
//	const char* pszSRS_WKT=poDataset->GetProjectionRef();


	pDriver=GetGDALDriverManager()->GetDriverByName("HFA");
	//创建数据
	pD=(GDALDataset*)GDALCreate(pDriver,outre,nImageSizeX,nImageSizeY,1,GDT_Float32,NULL);
	//添加地理信息
	pD->SetGeoTransform(adfGeoTransform);
	//添加投影信息
	pD->SetProjection(pszSRS_WKT);

	hBand=GDALGetRasterBand(pD,1);
	TRY {
		GDALRasterIO(hBand,GF_Write,0,0,nImageSizeX,nImageSizeY,ndvig,nImageSizeX,nImageSizeY,GDT_Float32,0,0);
	}
	CATCH (CMemoryException, e)
	{
		AfxMessageBox("影像存储失败！");
	}
	END_CATCH

	AfxMessageBox("完成！");
	EndWaitCursor();

	GDALClose(pD);
	GDALClose(pDriver);

	delete[] data;
	delete[] data1;
	delete[] ndvig;
	GDALClose(poDataset);
	GDALClose(poDataset1);
}
