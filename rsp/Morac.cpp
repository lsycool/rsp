// Morac.cpp : 实现文件
//

#include "stdafx.h"
#include "rsp.h"
#include "Morac.h"
#include "afxdialogex.h"
#include "ProgressBar.h"

#define min(a,b)            (((a) < (b)) ? (a) : (b));

CProgressBar* pro;
vector<FEATUREPOINT> RemoveReplicative(vector<FEATUREPOINT> v,int &i)//去除重复的兴趣点
{
	vector<FEATUREPOINT> ret;

	vector<FEATUREPOINT>::iterator iter = v.begin();

	ret.clear();

	ret.push_back(*iter);

	i=0;

	BOOL bEXIST(FALSE);

	for (iter = v.begin(); iter != v.end(); ++iter)
	{
		bEXIST = false;

		for (vector<FEATUREPOINT>::iterator iter2 = ret.begin(); iter2 != ret.end(); ++iter2)
		{
			int a = iter->x;
			int aa = iter2->x;
			int b = iter->y;
			int bb = iter2->y;

			if (a == aa && b == bb)
			{
				bEXIST = TRUE;	//存在
				break;
			}
		}

		if (bEXIST == FALSE)
		{
			ret.push_back(*iter);
			++i;
		}
	}
	return ret;
}


// CMorac 对话框

IMPLEMENT_DYNAMIC(CMorac, CDialogEx)

CMorac::CMorac(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMorac::IDD, pParent)
{

	m_thre = 1000.0;
	m_win1 = 7.0;
	m_win2 = 5.0;
}

CMorac::~CMorac()
{
}

void CMorac::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_THRE, m_thre);
	DDX_Text(pDX, IDC_EDIT_WIN1, m_win1);
	DDX_Text(pDX, IDC_EDIT_WIN2, m_win2);
}


BEGIN_MESSAGE_MAP(CMorac, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MOIN, &CMorac::OnBnClickedButtonMoin)
	ON_BN_CLICKED(IDC_BUTTON_MOUT, &CMorac::OnBnClickedButtonMout)
	ON_BN_CLICKED(IDC_BUTTON_CODAT, &CMorac::OnBnClickedButtonCodat)
	ON_BN_CLICKED(IDOK, &CMorac::OnBnClickedOk)
END_MESSAGE_MAP()


// CMorac 消息处理程序


//输入影像
void CMorac::OnBnClickedButtonMoin()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.bmp)|*.bmp|JPG文件(*.jpg)|*.jpg;*.jpeg|tiff文件(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="请输入待提取moravec遥感影像";
	if (file.DoModal()==IDOK)
	{
		M_in=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_INMOC,M_in);
}

//输出结果影像
void CMorac::OnBnClickedButtonMout()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(false, "BMP", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.BMP)|*.BMP|JPG文件(*.JPG)|*.JPG;*.JPEG|tiff文件(*.TIFF)|*.TIFF;*.TIF|img(*.IMG)|*.IMG|hdr(*.HDR)|*.HDR||");
	file.m_ofn.lpstrTitle="请输入结果影像";
	if (file.DoModal()==IDOK)
	{
		M_out=file.GetPathName();
		switch(file.m_ofn.nFilterIndex)
		{
		case 2:
			{
				if (M_out.Right(4)!=".BMP")
				{
					M_out=M_out+".BMP";
				}
				break;
			}
		case 4:
			{
				if (M_out.Right(5)!=".TIFF")
				{
					M_out=M_out+".TIFF";
				}
				break;
			}

		}

	}
	SetDlgItemText(IDC_STATIC_OUTMOC,M_out);
}

//保存兴趣点
void CMorac::OnBnClickedButtonCodat()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(false, "txt", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|文本(*.txt)|*.txt||");
	file.m_ofn.lpstrTitle="请输入兴趣点文件";
	if (file.DoModal()==IDOK)
	{
		/*if (M_data.Right(4)!=".txt")
		{
			M_data=file.GetPathName()+".txt";
		}
		else*/
		M_data=file.GetPathName();	
	}
	SetDlgItemText(IDC_STATIC_OUTDATA,M_data);
}


void CMorac::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (m_thre<1||m_win1<1||m_win2<1)
	{
		AfxMessageBox("参数必须大于0，请重新输入！");
		return;
	}

	GDALDataset* poDataset;//GDAL数据集
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(M_in,GA_ReadOnly);

	if(poDataset==NULL)
	{
		AfxMessageBox(_T("打开文件失败！！！"));
		return;
	}

	int nImageSizeX=poDataset->GetRasterXSize();
	int nImageSizeY=poDataset->GetRasterYSize();

	BYTE *InData1 = new BYTE[nImageSizeX*nImageSizeY*3];
	BYTE *OutData1=new BYTE[nImageSizeX*nImageSizeY*3];
	poDataset->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData1,nImageSizeX,nImageSizeY, GDT_Byte,3, NULL,0,0,0 );
	/*GDALRasterBand* Band1;
	Band1=poDataset->GetRasterBand(1);
	Band1->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData1,nImageSizeX,nImageSizeY, GDT_Byte,0,0);*/

	BeginWaitCursor();

	//设置进度条
	pro=new CProgressBar;
	pro->Create(IDD_DIALOG_JDT);
	pro->ShowWindow(SW_SHOW);
	//计算莫罗维克
	CaculMoc(InData1,OutData1,nImageSizeX,nImageSizeY,m_win1,m_win2,m_thre,M_out,M_data);

	delete pro;

	int IDType=AfxMessageBox("是否立即查看结果影像和特征点文档？",MB_YESNO|MB_ICONINFORMATION);
	if (IDType==IDYES)
	{
		ShellExecute(NULL,"open",M_out,NULL,NULL,SW_SHOWNORMAL);
		ShellExecute(NULL,"open",M_data,NULL,NULL,SW_SHOWNORMAL);
	}
	

	EndWaitCursor();

	delete[] InData1;
	delete[] OutData1;
	GDALClose(poDataset);

	CDialogEx::OnOK();
}


// 计算莫罗维克
//参数说明;indata:输入影像；outdata：输出影像；ImageSizeX：输入影像的长；ImageSizeY：输入影像的宽；win1：兴趣值窗口大小；win2：特征点窗口大小；thre：兴趣点阈值；file：特征点坐标文件保存路径
void CMorac::CaculMoc(BYTE* Indata1, BYTE* outdata,int ImageSizeX,int ImageSizeY,int win1,int win2,double thre,CString file,CString TxtFile)
{
	int k=win1/2;
	int Count1=0;
	int Count3=0;

	float* pInterest = new float[ImageSizeX*ImageSizeY];
	ZeroMemory(pInterest, sizeof(float)*ImageSizeX*ImageSizeY);//赋初值清零(与memset函数差不多不过只能用于win32程序)

	if(pro != NULL)  
	{  
		pro->ReSetProcess();  
		pro->SetMessage("正在计算感兴趣点。。");
		pro->SetStepCount(ImageSizeY*ImageSizeX); //设置进度条的总步长
	}  

	//输入影像的一个拷贝
	for (int i=0;i<ImageSizeX*ImageSizeY*3;i++)
	{
		outdata[i]=Indata1[i];
	}

	BYTE* Indata=new BYTE[ImageSizeX*ImageSizeY];
	ZeroMemory(Indata,sizeof(BYTE)*ImageSizeX*ImageSizeY);

	BYTE *TempD1 = new BYTE[ImageSizeX*ImageSizeY];
	BYTE *TempD2 = new BYTE[ImageSizeX*ImageSizeY];
	BYTE *TempD3 = new BYTE[ImageSizeX*ImageSizeY]; 

	for (int i=0;i<ImageSizeX*ImageSizeY;i++)//对各个波段进行加权求和
	{
		TempD1[i]=Indata1[i]*0.299;
		TempD2[i]=Indata1[ImageSizeX*ImageSizeY+i]*0.587;
		TempD3[i]=Indata1[ImageSizeX*ImageSizeY*2+i]*0.144;
		Indata[i]=TempD1[i]+TempD2[i]+TempD3[i];//存储加权求和的结果

		if (pro!= NULL)
		{
			bool bIsCancel = pro->StepIt();  
			if(!bIsCancel)  
			{
				delete[] pInterest;
				delete[] Indata;
				delete[] TempD1;
				delete[] TempD2;
				delete[] TempD3;
				return; 
			}
		}
	}
	//delete[] TempD1;//释放内存
	//delete[] TempD2;
	//delete[] TempD3;


	for (int i = k; i < ImageSizeY - k; ++i)
	{
		for (int j = k; j < ImageSizeX - k; ++j)
		{
			double IV=0.0,v1=0.0,v2 =0.0,v3=0.0,v4=0.0;//保存四个方向的平方和
			for (int m = 0; m < m_win1-1; ++m)
			{
				v1 += (Indata[(i-k+m)*ImageSizeX + (j)] - Indata[(i-k+1+m)*ImageSizeX + (j)])*
					(Indata[(i-k+m)*ImageSizeX + (j)] - Indata[(i-k+1+m)*ImageSizeX + (j)]);//竖直方向

				v2 += (Indata[(i-k+m)*ImageSizeX + (j-k+m)] - Indata[(i-k+1+m)*ImageSizeX + (j-k+m+1)])*
					(Indata[(i-k+m)*ImageSizeX + (j-k+m)] - Indata[(i-k+1+m)*ImageSizeX + (j-k+m+1)]);//西北方向

				v3 += (Indata[(i)*ImageSizeX + (j-k+m)] - Indata[(i)*ImageSizeX + (j-k+1+m)])*
					(Indata[(i)*ImageSizeX + (j-k+m)] - Indata[(i)*ImageSizeX + (j-k+1+m)]);//水平方向

				v4 += (Indata[(i-k+m)*ImageSizeX + (j+k-m)] - Indata[(i-k+m+1)*ImageSizeX + (j+k-m-1)])*
					(Indata[(i-k+m)*ImageSizeX + (j+k-m)] - Indata[(i-k+m+1)*ImageSizeX + (j+k-m-1)]);//东北方向
			}
			IV = min(v1, v2);
			IV = min(IV, v3);
			IV = min(IV, v4);

			if (IV > m_thre)
			{
				//成为候选点
				pInterest[i*ImageSizeX+j] = IV;
				++Count1;
			}
		}
	}
	
	int Count2(0);
	int k2 = m_win2 / 2;

	if (pro!=NULL)
	{
		pro->SetMessage("正在筛选特征点。。");
		pro->SetStepCount((ImageSizeY - k2)*(ImageSizeX - k2)); //设置进度条的总步长
	}

	vector<FEATUREPOINT> FeaturePoint;
	FEATUREPOINT tempFP;
	ZeroMemory(&tempFP, sizeof(FEATUREPOINT));
	
	float max2(0);
	int tempX(0), tempY(0);
	bool b(false);

	for(int i = k2; i < ImageSizeY - k2; i += m_win2)
	{
		for(int j = k2; j < ImageSizeX - k2; j += m_win2)
		{
			if (pro!= NULL)
			{
				bool bIsCancel = pro->StepIt();  
				if(!bIsCancel)  
				{
					delete[] pInterest;
					delete[] Indata;
					delete[] TempD1;
					delete[] TempD2;
					delete[] TempD3;
					return; 
				}
			}

			max2 = 0;
			b = false;
			for (int m = i-k2; m < i+k2+1; ++m)
			{
				for (int n = j-k2; n<j+k2+1; ++n)
				{
					if(pInterest[m*ImageSizeX + n]>max2)
					{
						max2 = pInterest[m*ImageSizeX + n];//记录计算窗口中的最大值
						tempY = m;//记录最大值像素坐标
						tempX = n;
						b = true;//找到了最大值
					}
				}
			}
			if (b)
			{
				tempFP.IV = pInterest[tempY*ImageSizeX+tempX];
				tempFP.x = tempX;
				tempFP.y = tempY;
				FeaturePoint.push_back(tempFP);//保存特征点

				++Count2;	//尚未剔除重复点
			}
		}
	}

	vector<FEATUREPOINT> FeaturePoint2;
	FeaturePoint2 = RemoveReplicative(FeaturePoint,Count3);//去除重复控制点

	//保存结果位图

	if (pro!=NULL)
	{
		pro->SetMessage("正在保存结果影像。。");
		pro->SetStepCount((int)FeaturePoint2.size()); //设置进度条的总步长
	}

	vector<FEATUREPOINT>::iterator iter;

	for (iter = FeaturePoint2.begin(); iter != FeaturePoint2.end(); ++iter)
	{
		if (pro!= NULL)
		{
			bool bIsCancel = pro->StepIt();  
			if(!bIsCancel)  
			{
				delete[] pInterest;
				delete[] Indata;
				delete[] TempD1;
				delete[] TempD2;
				delete[] TempD3;
				return; 
			}
		}

		int i = iter->y;
		int j = iter->x;

		//第一波段
		outdata[i*ImageSizeX+j] = 255;

		outdata[i*ImageSizeX+j+1] = 255;
		outdata[i*ImageSizeX+j-1] = 255;
		outdata[(i+1)*ImageSizeX+j] = 255;
		outdata[(i-1)*ImageSizeX+j] = 255;

		outdata[i*ImageSizeX+j+2] = 255;
		outdata[i*ImageSizeX+j-2] = 255;
		outdata[(i+2)*ImageSizeX+j] = 255;
		outdata[(i-2)*ImageSizeX+j] = 255;


		//第二波段
		outdata[ImageSizeX*ImageSizeY+i*ImageSizeX+j] = 0;

		outdata[ImageSizeX*ImageSizeY+i*ImageSizeX+j+1] = 0;
		outdata[ImageSizeX*ImageSizeY+i*ImageSizeX+j-1] = 0;
		outdata[ImageSizeX*ImageSizeY+(i+1)*ImageSizeX+j] = 0;
		outdata[ImageSizeX*ImageSizeY+(i-1)*ImageSizeX+j] = 0;

		outdata[ImageSizeX*ImageSizeY+i*ImageSizeX+j+2] = 0;
		outdata[ImageSizeX*ImageSizeY+i*ImageSizeX+j-2] = 0;
		outdata[ImageSizeX*ImageSizeY+(i+2)*ImageSizeX+j] = 0;
		outdata[ImageSizeX*ImageSizeY+(i-2)*ImageSizeX+j] = 0;

		//第三波段
		outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeX+j] = 0;

		outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeX+j+1] = 0;
		outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeX+j-1] = 0;
		outdata[ImageSizeX*ImageSizeY*2+(i+1)*ImageSizeX+j] = 0;
		outdata[ImageSizeX*ImageSizeY*2+(i-1)*ImageSizeX+j] = 0;

		outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeX+j+2] = 0;
		outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeX+j-2] = 0;
		outdata[ImageSizeX*ImageSizeY*2+(i+2)*ImageSizeX+j] = 0;
		outdata[ImageSizeX*ImageSizeY*2+(i-2)*ImageSizeX+j] = 0;

	}
	SaveImage(outdata,ImageSizeX,ImageSizeY,M_out,"BMP");//保存影像
	SaveToText(TxtFile,Count1,FeaturePoint2);//输出特征点


	//释放内存
	delete[] pInterest;
	delete[] Indata;
	delete[] TempD1;
	delete[] TempD2;
	delete[] TempD3;
}

// 保存结果影像
void CMorac::SaveImage(BYTE* OutData, int ImageSizeX,int ImageSizeY,CString saveFile, CString ImageType)
{
//	GDALDataset* pD=NULL;//数据集句柄
//	GDALDatasetH  pDriver;//数据集驱动
//	GDALRasterBandH hBand;
//
//	double adfGeoTransform[6]={444720,30,0,3751320,0,-30};
////	double* adfGeoTransform=NULL;
//	char* pszSRS_WKT=NULL;
////	poDataset->GetGeoTransform(adfGeoTransform);
////	const char* pszSRS_WKT=poDataset->GetProjectionRef();
//
//
//	pDriver=GetGDALDriverManager()->GetDriverByName(ImageType);
//	if (pDriver==NULL)
//	{
//		AfxMessageBox("无法保存成这种格式！");
//		return;
//	}
//	
//	TRY {
//
//		//创建数据
//		pD=(GDALDataset*)GDALCreate(pDriver,saveFile,ImageSizeX,ImageSizeY,3,GDT_Byte,NULL);
//		//添加地理信息
//		pD->SetGeoTransform(adfGeoTransform);
//		//添加投影信息
//		pD->SetProjection(pszSRS_WKT);
//
//		hBand=GDALGetRasterBand(pD,1);
//
//		GDALRasterIO(hBand,GF_Write,0,0,ImageSizeX,ImageSizeY,OutData,ImageSizeX,ImageSizeY,GDT_Byte,0,0);
//	}
//	CATCH (CMemoryException, e)
//	{
//		AfxMessageBox("影像存储失败！");
//		return;
//	}
//	END_CATCH
//
//	AfxMessageBox("影像保存成功！");
//
//	GDALClose(pD);
//	GDALClose(pDriver);
	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(ImageType);
	GDALDataset *poDstDS;
	char **papszOptions =poDriver->GetMetadata();
	poDstDS = poDriver->Create( (char*)(LPCTSTR)M_out,//输出路径
		ImageSizeX,ImageSizeY, //影像数据宽高
		3, //存或取的波段数
		GDT_Byte, 	//缓冲区数据类型
		papszOptions );

	if(poDstDS->RasterIO(
		GF_Write,						//IO方式
		0,0,							//影像数据起始位置x,y
		ImageSizeX,ImageSizeY,				//影像数据宽高
		OutData,						//指向缓冲区数据指针
		ImageSizeX,ImageSizeY,				//缓冲区数据宽高
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
		AfxMessageBox("完成！");
	}

	GDALClose(poDstDS);

}


// //保存特征点坐标
void CMorac::SaveToText(CString file,int Count1,vector<FEATUREPOINT> FeaturePoint2)
{
	FILE*  pTxtFile= fopen(file, "wt");

	if (!pTxtFile)
		return ;

	fprintf(pTxtFile, "**********\t文档说明：%s\n**********\t程序所有人：%s\n**********\t单位: %s\n"," Moravec特征点输出文档","刘世永", "中国地质大学");
	fprintf(pTxtFile, "**********\t阈值: %d\n", (int)m_thre);
	fprintf(pTxtFile, "**********\t兴趣点窗口大小: %d×%d\n**********\t特征点窗口大小: %d×%d\n", (int)m_win1, (int)m_win1, (int)m_win2, (int)m_win2);
	fprintf(pTxtFile, "**********\t兴趣点数: %d\n", Count1);
	fprintf(pTxtFile, "**********\t特征点数: %d\n\n", FeaturePoint2.size());
	fprintf(pTxtFile, "坐标X\t坐标Y\t兴趣值\n");

	TRY {
		for (vector<FEATUREPOINT>::iterator iter = FeaturePoint2.begin(); iter != FeaturePoint2.end(); ++ iter)
			fprintf(pTxtFile, "%d\t%d\t%.1f\n", iter->x, iter->y, iter->IV);
		AfxMessageBox("特征点存储完成！");
	}
	CATCH (CMemoryException, e)
	{
		AfxMessageBox("存储失败！");
	}
	END_CATCH
	
	fclose(pTxtFile);
}



BOOL CMorac::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
