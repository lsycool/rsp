// ImageShow.cpp : 实现文件
//

#include "stdafx.h"
#include "rsp.h"
#include "afxdialogex.h"
#include "rspDlg.h"
#include "BandSythesis.h"
#include "SuoYin.h"
#include "Contrast.h"
#include "ProgressBar.h"
#include "RGBXY.h"
#include "FileList.h"
#include "Arthmetic.h"
#include "ImageShow.h"
#include "ROI.h"
// ImageShow 对话框
int nImageSizeX;
int nImageSizeY;
CPoint m_point;//用来记录鼠标点击点的位置
CPoint EndPoint;//用来记录鼠标移动到的点位
BITMAP bitmap;
int Type_Image=0;
int d=0;//缩小系数
int e=0;//放大系数
int flag1=0;//判断是否灰度转换或者是索引显示
int curous1=0;//控制光标

int nScreenWidthZ;
int nScreenHeightZ;

int nsx;//显示区x
int nsy;

int OffsetX;//偏移量
int OffsetY;

BOOL selnum=FALSE;
BOOL info=FALSE;
BOOL mark=FALSE;//判断是否移动
BOOL lbtnD=FALSE;//判断鼠标左键是否按下

CRGBXY* crgb;//像素信息
CProgressBar* pro1;//进度条
CFileList* cfiles=new CFileList;//文件列表
CROI* roi;

CRect rect1(0,0,0,0);//图像缩放区域

extern CPoint BeginSub;//记录裁剪区
extern CPoint EndSub;
extern CString BiaoZhi;
COLORREF	RGBColor;
int SuoFangBiLi=0;

IMPLEMENT_DYNAMIC(ImageShow, CDialogEx)

ImageShow::ImageShow(CWnd* pParent /*=NULL*/)
: CDialogEx(ImageShow::IDD, pParent)

{
	rgbColor=RGB(255,255,255);
}

ImageShow::~ImageShow()
{
}

void ImageShow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL ImageShow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->ModifyStyleEx(0,WS_EX_APPWINDOW);

	MoveWindow(15,75,660,660);
	ShowWindow(SW_SHOW);

	m_brush.CreateSolidBrush(RGB(255, 255, 255)); // 生成一白色刷子
	cfiles->Create(IDD_DIALOG_FILELIST);
	return TRUE;
	
}

BEGIN_MESSAGE_MAP(ImageShow, CDialogEx)
	ON_WM_PAINT()
	ON_COMMAND(IDOK, &ImageShow::OnIdok)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_32785, &ImageShow::OnUpdateImage)
	ON_COMMAND(ID_32782, &ImageShow::OnElarge)
	ON_COMMAND(ID_32783, &ImageShow::OnNarrow)
//	ON_COMMAND(ID_32786, &ImageShow::On32786)
	ON_COMMAND(ID_32786, &ImageShow::OnMobile)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_32784, &ImageShow::OnRecovery)
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_32789, &ImageShow::OnRGBtoGray)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// ImageShow 消息处理程序




bool ImageShow::ShowImage(int a,int b,int c,CDC* cdc)
{
	SetWindowText(path);

	CRect m_viewRect;

	GDALDataset* poDataset;//GDAL数据集
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO"); 

	poDataset=(GDALDataset*)GDALOpen(path,GA_ReadOnly);
	if(poDataset==NULL)
	{
		AfxMessageBox(_T("打开文件失败！！！"));
		return false;
	}
	
	if (!selnum)
	{
		pro1=new CProgressBar;
		pro1->Create(IDD_DIALOG_JDT);
		pro1->ShowWindow(SW_SHOW);
	}

	//遥感的三个波段
	GDALRasterBand* Band1;
	GDALRasterBand* Band2;
	GDALRasterBand* Band3;

	nBandCount=poDataset->GetRasterCount();

	//配置波段
	if(nBandCount>=3)//多波段
	{
		Band1=poDataset->GetRasterBand(a);
		Band2=poDataset->GetRasterBand(b);
		Band3=poDataset->GetRasterBand(c);
	}
	else//单波段
	{
		Band1=poDataset->GetRasterBand(1);
		Band2=poDataset->GetRasterBand(1);
		Band3=poDataset->GetRasterBand(1);
	}
	//获得图像显示窗口的尺寸
	GetClientRect(&m_viewRect);

	nImageSizeX=poDataset->GetRasterXSize();
	nImageSizeY=poDataset->GetRasterYSize();

	//影像数据大小
	float size=(nImageSizeX/1024)*(nImageSizeY/1024)*8;
	//获得投影信息
	CString proj=poDataset->GetProjectionRef();
	CString projection="投影：";
	int a1=proj.Find("PROJCS[");
	int a2;
	if (!proj.IsEmpty())
	{
		if (a1>=0)
		{
			a2=proj.Find(",",a1);
			projection=projection+proj.Mid(a1+7,a2-(a1+7));
		}
		else
		{
			projection=projection+"geographic Lat/Lot";
		}
	}
	int a3=proj.Find("GEOGCS[");
	int a4;
	CString cordin="坐标系：";
	if (!proj.IsEmpty())
	{
		if (a3>=0)
		{
			a4=proj.Find(",",a3);
			cordin=cordin+proj.Mid(a3+7,a4-(a3+7));
		}
	}

	int a5=proj.Find("DATUM[");
	int a6;
	CString datum="椭球：";
	if (!proj.IsEmpty())
	{
		if (a5>=0)
		{
			a6=proj.Find(",",a5);
			datum=datum+proj.Mid(a5+6,a6-(a5+6));
		}
	}

	//获得仿射变换的系数
	poDataset->GetGeoTransform(adfGeoTransform);

	double right=adfGeoTransform[0]+nImageSizeX*adfGeoTransform[1];
	double left=adfGeoTransform[3]+nImageSizeY*adfGeoTransform[5];

	int nBufferSizeX,nBufferSizeY;
	nBufferSizeX=nImageSizeX;
	nBufferSizeY=nImageSizeY;

	int nScreenWidth=m_viewRect.Width();
	int nScreenHeight=m_viewRect.Height();

	//重新调整长宽比适应显示
	float m_fXYRation = (float)nImageSizeY/(float)nImageSizeX;//影像的长宽比
	if (nBufferSizeX>nBufferSizeY)
	{
		nScreenHeight = (int)((float)nScreenWidth*m_fXYRation); //显示的长宽
		SuoFangBiLi=nImageSizeY/nScreenHeight;
	}
	else
	{
		nScreenWidth = (int)((float)nScreenHeight/m_fXYRation); //显示的长宽
		SuoFangBiLi=nImageSizeX/nScreenWidth;
	}
	OffsetX = (m_viewRect.Width()- nScreenWidth)/2;
	OffsetY = (m_viewRect.Height()- nScreenHeight)/2;
	//
	nsx=nScreenWidth;
	nsy=nScreenHeight;

	nScreenWidthZ=nScreenWidth;
	nScreenHeightZ=nScreenHeight;

	//为数据缓冲区分配内存
	BYTE* pafScanblock1=(BYTE*)CPLMalloc((nScreenWidth)*(nScreenHeight));
	BYTE* pafScanblock2=(BYTE*)CPLMalloc((nScreenWidth)*(nScreenHeight));
	BYTE* pafScanblock3=(BYTE*)CPLMalloc((nScreenWidth)*(nScreenHeight));

	TempLock1=pafScanblock1;
	TempLock2=pafScanblock2;
	TempLock3=pafScanblock3;

	//读取数据(,起始图像的左上角坐标，读取或写入图像的窗口大小，数据指针，缓冲区大小，字节间隔)
	Band1->RasterIO(GF_Read,0,0,nBufferSizeX,nBufferSizeY,pafScanblock1,nScreenWidth,nScreenHeight,GDT_Byte,0,0);
	Band2->RasterIO(GF_Read,0,0,nBufferSizeX,nBufferSizeY,pafScanblock2,nScreenWidth,nScreenHeight,GDT_Byte,0,0);
	Band3->RasterIO(GF_Read,0,0,nBufferSizeX,nBufferSizeY,pafScanblock3,nScreenWidth,nScreenHeight,GDT_Byte,0,0);

	//在View逐点显示图像
/*	DWORD dwBytes=(nScreenWidth*24)/8;
//	while(DWORD(dwBytes)%4)
	{
		dwBytes++;
	}
//	BYTE* szBuffer=new BYTE[nScreenHeight*dwBytes];
//	memset(szBuffer,0,nScreenHeight*dwBytes);
	BYTE* pTemp=szBuffer;
*/
	int nIndex=0;
	CDC dcMem;
	CDC* pDC=this->GetDC();
	CBitmap* OldBitmap;
	dcMem.CreateCompatibleDC(pDC);


	MemBitmap.CreateCompatibleBitmap(pDC,nScreenWidth,nScreenHeight);
	OldBitmap= dcMem.SelectObject(&MemBitmap);

	//Bitmap bmp(nImageSizeX,nImageSizeY,nImageSizeX*3,PixelFormat24bppRGB,szBuffer);gdi+与gdal结合
	if (flag1==4)//直方图均衡化
	{
		pro1=new CProgressBar;
		pro1->Create(IDD_DIALOG_JDT);
		pro1->ShowWindow(SW_SHOW);
		if(pro1 != NULL)  
		{  
			pro1->ReSetProcess();  
			pro1->SetStepCount(nScreenHeight*nScreenWidth); //设置进度条的总步长
			pro1->SetMessage("正在计算结果请等待。。");
		}  

		for (int i=0;i<nScreenHeight;i++)
		{
			for (int j=0;j<nScreenWidth;j++)
			{
				BYTE dn1=*pafScanblock1;
				BYTE d=0;
				int a=dn1-d;
				dcMem.SetPixel(j,i,RGB(JunHen[a],JunHen[a],JunHen[a]));
				pafScanblock1++;

				pro1->StepIt();
			}
		}
		cdc->StretchBlt(OffsetX,OffsetY,nScreenWidth,nScreenHeight,&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
		flag1=0;
		/*CPLFree(TempLock1);
		CPLFree(TempLock2);
		CPLFree(TempLock3);*/
		delete pro1;
		MemBitmap.DeleteObject();
		dcMem.SelectObject(OldBitmap);
		dcMem.DeleteDC();
		ReleaseDC(pDC);
		GDALClose(poDataset);

		return true;
	}

	if (flag1==6)//分段线性拉伸
	{
		BYTE* pafScanblock4=(BYTE*)CPLMalloc((nScreenWidth)*(nScreenHeight)*nBandCount);
		poDataset->RasterIO(GF_Read,0,0,nBufferSizeX,nBufferSizeY,pafScanblock4,nScreenWidth,nScreenHeight,GDT_Byte,nBandCount,NULL,0,0,0);
		CPoint point1,point2;
		point1.x=linesdlgPara.m_nX1;
		point1.y=linesdlgPara.m_nY1;
		point2.x =linesdlgPara.m_nX2;
		point2.y=linesdlgPara.m_nY2;
		linesdlgPara.LinearStrech(point1,point2,pafScanblock4,nBandCount,nScreenWidth,nScreenHeight);
		for (int i=0;i<nScreenHeight;i++)
		{
			for (int j=0;j<nScreenWidth;j++)
			{
				BYTE dn1=linesdlgPara.m_pImgDataOut[i*nScreenWidth+j];
				BYTE dn2=linesdlgPara.m_pImgDataOut[nScreenWidth*nScreenHeight+i*nScreenWidth+j];
				BYTE dn3=linesdlgPara.m_pImgDataOut[nScreenWidth*nScreenHeight*2+i*nScreenWidth+j];
				dcMem.SetPixel(j,i,RGB(dn1,dn2,dn3));
			}
		}
		cdc->StretchBlt(OffsetX,OffsetY,nScreenWidth,nScreenHeight,&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
//		flag1=0;
		MemBitmap.DeleteObject();
		dcMem.SelectObject(OldBitmap);
		dcMem.DeleteDC();
		ReleaseDC(pDC);
		CPLFree(pafScanblock4);
		GDALClose(poDataset);
		return true;
	}

	if (flag1==7)//分段线性拉伸
	{
		// 获取用户设定的平移量
		float fA = dlgPara.m_fA;
		float fB = dlgPara.m_fB;
		delete dlgPara;

		BYTE* pafScanblock4=(BYTE*)CPLMalloc((nScreenWidth)*(nScreenHeight)*nBandCount);
		poDataset->RasterIO(GF_Read,0,0,nBufferSizeX,nBufferSizeY,pafScanblock4,nScreenWidth,nScreenHeight,GDT_Byte,nBandCount,NULL,0,0,0);
		dlgPara.LinerTrans(pafScanblock4,nBandCount,nScreenWidth,nScreenHeight,fA,fB);
		for (int i=0;i<nScreenHeight;i++)
		{
			for (int j=0;j<nScreenWidth;j++)
			{
				BYTE dn1=pafScanblock4[i*nScreenWidth+j];
				BYTE dn2=pafScanblock4[nScreenWidth*nScreenHeight+i*nScreenWidth+j];
				BYTE dn3=pafScanblock4[nScreenWidth*nScreenHeight*2+i*nScreenWidth+j];
				dcMem.SetPixel(j,i,RGB(dn1,dn2,dn3));
			}
		}
		cdc->StretchBlt(OffsetX,OffsetY,nScreenWidth,nScreenHeight,&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
		MemBitmap.DeleteObject();
		dcMem.SelectObject(OldBitmap);
		dcMem.DeleteDC();
		ReleaseDC(pDC);
		CPLFree(pafScanblock4);
		GDALClose(poDataset);
		return true;
	}

	if(flag1==8)//腐蚀
	{
		BYTE* pafScanblock4=(BYTE*)CPLMalloc((nScreenWidth)*(nScreenHeight)*nBandCount);
		poDataset->RasterIO(GF_Read,0,0,nBufferSizeX,nBufferSizeY,pafScanblock4,nScreenWidth,nScreenHeight,GDT_Byte,nBandCount,NULL,0,0,0);
		// 获取用户设定的腐蚀方向
		int nMode = dlgPara1.m_nMode;
		int structure[3][3];
		if (nMode == 2)
		{
			structure[0][0]=dlgPara1.m_nStructure1;
			structure[0][1]=dlgPara1.m_nStructure2;
			structure[0][2]=dlgPara1.m_nStructure3;
			structure[1][0]=dlgPara1.m_nStructure4;
			structure[1][1]=dlgPara1.m_nStructure5;
			structure[1][2]=dlgPara1.m_nStructure6;
			structure[2][0]=dlgPara1.m_nStructure7;
			structure[2][1]=dlgPara1.m_nStructure8;
			structure[2][2]=dlgPara1.m_nStructure9;
		}
		// 调用ErosionDIB()函数腐蚀DIB
		if (dlgPara1.ErosionDIB(pafScanblock4, nScreenWidth, nScreenHeight, nMode , structure))
		{
			for (int i=0;i<nScreenHeight;i++)
			{
				for (int j=0;j<nScreenWidth;j++)
				{
					BYTE dn1=pafScanblock4[i*nScreenWidth+j];
					BYTE dn2=pafScanblock4[i*nScreenWidth+j];
					BYTE dn3=pafScanblock4[i*nScreenWidth+j];
					dcMem.SetPixel(j,i,RGB(dn1,dn2,dn3));
				}
			}
			cdc->StretchBlt(OffsetX,OffsetY,nScreenWidth,nScreenHeight,&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
			MemBitmap.DeleteObject();
			dcMem.SelectObject(OldBitmap);
			dcMem.DeleteDC();
			ReleaseDC(pDC);
			CPLFree(pafScanblock4);
			delete dlgPara1;
			GDALClose(poDataset);
			return true;
		}
		else
		{
			// 提示用户
			MessageBox("分配内存失败或者图像中含有0和255之外的像素值！", "系统提示" , MB_ICONINFORMATION | MB_OK);
			return false;
		}
	}

	if (flag1==9)//膨胀
	{
		BYTE* pafScanblock4=(BYTE*)CPLMalloc((nScreenWidth)*(nScreenHeight)*nBandCount);
		poDataset->RasterIO(GF_Read,0,0,nBufferSizeX,nBufferSizeY,pafScanblock4,nScreenWidth,nScreenHeight,GDT_Byte,nBandCount,NULL,0,0,0);
		// 获取用户设定的膨胀方向
		int nMode = dlgPara2.m_nMode;

		int structure[3][3];
		if (nMode == 2)
		{
			structure[0][0]=dlgPara2.m_nStructure1;
			structure[0][1]=dlgPara2.m_nStructure2;
			structure[0][2]=dlgPara2.m_nStructure3;
			structure[1][0]=dlgPara2.m_nStructure4;
			structure[1][1]=dlgPara2.m_nStructure5;
			structure[1][2]=dlgPara2.m_nStructure6;
			structure[2][0]=dlgPara2.m_nStructure7;
			structure[2][1]=dlgPara2.m_nStructure8;
			structure[2][2]=dlgPara2.m_nStructure9;
		}
		// 删除对话框
		delete dlgPara2;	
		// 调用DilationDIB()函数膨胀DIB
		if (DilationDIB(pafScanblock4, nScreenWidth, nScreenHeight, nMode , structure))
		{
			for (int i=0;i<nScreenHeight;i++)
			{
				for (int j=0;j<nScreenWidth;j++)
				{
					BYTE dn1=pafScanblock4[i*nScreenWidth+j];
					BYTE dn2=pafScanblock4[i*nScreenWidth+j];
					BYTE dn3=pafScanblock4[i*nScreenWidth+j];
					dcMem.SetPixel(j,i,RGB(dn1,dn2,dn3));
				}
			}
			cdc->StretchBlt(OffsetX,OffsetY,nScreenWidth,nScreenHeight,&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
			MemBitmap.DeleteObject();
			dcMem.SelectObject(OldBitmap);
			dcMem.DeleteDC();
			ReleaseDC(pDC);
			CPLFree(pafScanblock4);
			GDALClose(poDataset);
			return true;

		}
		else
		{
			// 提示用户
			MessageBox("分配内存失败或者图像中含有0和255之外的像素值！", "系统提示" , MB_ICONINFORMATION | MB_OK);
			return false;
		}
	}

	if (flag1==10)//开运算
	{
		BYTE* pafScanblock4=(BYTE*)CPLMalloc((nScreenWidth)*(nScreenHeight)*nBandCount);
		poDataset->RasterIO(GF_Read,0,0,nBufferSizeX,nBufferSizeY,pafScanblock4,nScreenWidth,nScreenHeight,GDT_Byte,nBandCount,NULL,0,0,0);
		// 获取用户设定的开运算方向
		int nMode = dlgPara3.m_nMode;

		int structure[3][3];
		if (nMode == 2)
		{
			structure[0][0]=dlgPara3.m_nStructure1;
			structure[0][1]=dlgPara3.m_nStructure2;
			structure[0][2]=dlgPara3.m_nStructure3;
			structure[1][0]=dlgPara3.m_nStructure4;
			structure[1][1]=dlgPara3.m_nStructure5;
			structure[1][2]=dlgPara3.m_nStructure6;
			structure[2][0]=dlgPara3.m_nStructure7;
			structure[2][1]=dlgPara3.m_nStructure8;
			structure[2][2]=dlgPara3.m_nStructure9;
		}

		// 删除对话框
		delete dlgPara3;	
		if (OpenDIB(pafScanblock4, nScreenWidth, nScreenHeight, nMode , structure))
		{
			for (int i=0;i<nScreenHeight;i++)
			{
				for (int j=0;j<nScreenWidth;j++)
				{
					BYTE dn1=pafScanblock4[i*nScreenWidth+j];
					BYTE dn2=pafScanblock4[i*nScreenWidth+j];
					BYTE dn3=pafScanblock4[i*nScreenWidth+j];
					dcMem.SetPixel(j,i,RGB(dn1,dn2,dn3));
				}
			}
			cdc->StretchBlt(OffsetX,OffsetY,nScreenWidth,nScreenHeight,&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
			MemBitmap.DeleteObject();
			dcMem.SelectObject(OldBitmap);
			dcMem.DeleteDC();
			ReleaseDC(pDC);
			CPLFree(pafScanblock4);
			GDALClose(poDataset);
			return true;
		}
		else
		{
			// 提示用户
			MessageBox("分配内存失败或者图像中含有0和255之外的像素值！", "系统提示" , MB_ICONINFORMATION | MB_OK);
			return false;
		}
	}

	if (flag1==11)//闭运算
	{
		BYTE* pafScanblock4=(BYTE*)CPLMalloc((nScreenWidth)*(nScreenHeight)*nBandCount);
		poDataset->RasterIO(GF_Read,0,0,nBufferSizeX,nBufferSizeY,pafScanblock4,nScreenWidth,nScreenHeight,GDT_Byte,nBandCount,NULL,0,0,0);
		// 获取用户设定的开运算方向
		int nMode = dlgPara4.m_nMode;

		int structure[3][3];
		if (nMode == 2)
		{
			structure[0][0]=dlgPara4.m_nStructure1;
			structure[0][1]=dlgPara4.m_nStructure2;
			structure[0][2]=dlgPara4.m_nStructure3;
			structure[1][0]=dlgPara4.m_nStructure4;
			structure[1][1]=dlgPara4.m_nStructure5;
			structure[1][2]=dlgPara4.m_nStructure6;
			structure[2][0]=dlgPara4.m_nStructure7;
			structure[2][1]=dlgPara4.m_nStructure8;
			structure[2][2]=dlgPara4.m_nStructure9;
		}
		// 删除对话框
		delete dlgPara4;	
		if (CloseDIB(pafScanblock4, nScreenWidth, nScreenHeight, nMode , structure))
		{
			for (int i=0;i<nScreenHeight;i++)
			{
				for (int j=0;j<nScreenWidth;j++)
				{
					BYTE dn1=pafScanblock4[i*nScreenWidth+j];
					BYTE dn2=pafScanblock4[i*nScreenWidth+j];
					BYTE dn3=pafScanblock4[i*nScreenWidth+j];
					dcMem.SetPixel(j,i,RGB(dn1,dn2,dn3));
				}
			}
			cdc->StretchBlt(OffsetX,OffsetY,nScreenWidth,nScreenHeight,&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
			MemBitmap.DeleteObject();
			dcMem.SelectObject(OldBitmap);
			dcMem.DeleteDC();
			ReleaseDC(pDC);
			CPLFree(pafScanblock4);
			GDALClose(poDataset);
			return true;
		}
		else
		{
			// 提示用户
			MessageBox("分配内存失败或者图像中含有0和255之外的像素值！", "系统提示" , MB_ICONINFORMATION | MB_OK);
			return false;
		}
	}

	if (flag1==12)//细化算法
	{
		BYTE* pafScanblock4=(BYTE*)CPLMalloc((nScreenWidth)*(nScreenHeight)*nBandCount);
		poDataset->RasterIO(GF_Read,0,0,nBufferSizeX,nBufferSizeY,pafScanblock4,nScreenWidth,nScreenHeight,GDT_Byte,nBandCount,NULL,0,0,0);
		if (ThiningDIB(pafScanblock4, nScreenWidth, nScreenHeight))
		{
			for (int i=0;i<nScreenHeight;i++)
			{
				for (int j=0;j<nScreenWidth;j++)
				{
					BYTE dn1=pafScanblock4[i*nScreenWidth+j];
					BYTE dn2=pafScanblock4[i*nScreenWidth+j];
					BYTE dn3=pafScanblock4[i*nScreenWidth+j];
					dcMem.SetPixel(j,i,RGB(dn1,dn2,dn3));
				}
			}
			cdc->StretchBlt(OffsetX,OffsetY,nScreenWidth,nScreenHeight,&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
			MemBitmap.DeleteObject();
			dcMem.SelectObject(OldBitmap);
			dcMem.DeleteDC();
			ReleaseDC(pDC);
			CPLFree(pafScanblock4);
			GDALClose(poDataset);
			return true;
		}
		else
		{
			// 提示用户
			MessageBox("分配内存失败或者图像中含有0和255之外的像素值！", "系统提示" , MB_ICONINFORMATION | MB_OK);
			return false;
		}
	}

	if (flag1==13)//击中击不中算法
	{
		BYTE* pafScanblock4=(BYTE*)CPLMalloc((nScreenWidth)*(nScreenHeight)*nBandCount);
		poDataset->RasterIO(GF_Read,0,0,nBufferSizeX,nBufferSizeY,pafScanblock4,nScreenWidth,nScreenHeight,GDT_Byte,nBandCount,NULL,0,0,0);
		if (HitAndMiss(pafScanblock4, nScreenWidth, nScreenHeight))
		{
			for (int i=0;i<nScreenHeight;i++)
			{
				for (int j=0;j<nScreenWidth;j++)
				{
					BYTE dn1=pafScanblock4[i*nScreenWidth+j];
					BYTE dn2=pafScanblock4[i*nScreenWidth+j];
					BYTE dn3=pafScanblock4[i*nScreenWidth+j];
					dcMem.SetPixel(j,i,RGB(dn1,dn2,dn3));
				}
			}
			cdc->StretchBlt(OffsetX,OffsetY,nScreenWidth,nScreenHeight,&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
			MemBitmap.DeleteObject();
			dcMem.SelectObject(OldBitmap);
			dcMem.DeleteDC();
			ReleaseDC(pDC);
			CPLFree(pafScanblock4);
			GDALClose(poDataset);
			return true;
		}
		else
		{
			// 提示用户
			MessageBox("分配内存失败或者图像中含有0和255之外的像素值！", "系统提示" , MB_ICONINFORMATION | MB_OK);
			return false;
		}
	}

	if((selnum==FALSE)&&(pro1!= NULL))  
	{  
		pro1->ReSetProcess();
		pro1->SetMessage("正在读取影像请等待。。");
		pro1->SetStepCount(nScreenHeight*nScreenWidth); //设置进度条的总步长
	}  

	for (int i=0;i<nScreenHeight;i++)
	{
		for (int j=0;j<nScreenWidth;j++)
		{
			BYTE dn1=*pafScanblock1;
			BYTE dn2=*pafScanblock2;
			BYTE dn3=*pafScanblock3;

			//先绘制到内存DC中，然后在绘制到设备DC中
			switch(flag1)
			{
			case 0:
				{
					//拉伸到0~255
					double maxPix=Band1->GetMaximum(NULL);
					double minPix=Band1->GetMinimum(NULL);
					dn1=((dn1-minPix)/(maxPix-minPix))*255;

					maxPix=Band2->GetMaximum(NULL);
					minPix=Band2->GetMinimum(NULL);
					dn2=((dn2-minPix)/(maxPix-minPix))*255;

					maxPix=Band3->GetMaximum(NULL);
					minPix=Band3->GetMinimum(NULL);
					dn3=((dn3-minPix)/(maxPix-minPix))*255;

					dcMem.SetPixel(j,i,RGB(dn1,dn2,dn3));
					if (!selnum)
					{
						pro1->StepIt();
					}
					break;
				}
			case 1://多光谱转全色
				{
					BYTE GR=dn1*0.299;
					BYTE GG=dn2*0.587;
					BYTE GB=dn3*0.144;
					BYTE Gray=GR+GG+GB;
					dcMem.SetPixel(j,i,RGB(Gray,Gray,Gray));
					break;
				}
			case 2://索引显示
				{
					BYTE sy=0;
					int  syn=dn1-sy;
					dcMem.SetPixel(j,i,RGB(ColorsTable[SIndex][syn][0],ColorsTable[SIndex][syn][1],ColorsTable[SIndex][syn][2]));
					break;
				}	
			case 3://对比度
				{
					double f=0;
					if (nPosCon>=0)
					{
						f=(nPosCon+10.0f)/10.0f;
					}
					else
						f=(255+nPosCon)/255.0f;
					BYTE Cdn1=128+(dn1-128)*f;
					BYTE Cdn2=128+(dn2-128)*f;
					BYTE Cdn3=128+(dn3-128)*f;
					dcMem.SetPixel(j,i,RGB(Cdn1,Cdn2,Cdn3));
					break;
				}
			case 5://二值化
				{
					BYTE EZC=EZ;
					if ((dn1+dn2+dn3)/3>=EZC)
					{
						dn1=255;
						dn2=255;
						dn3=255;
					}
					else
					{
						dn1=0;
						dn2=0;
						dn3=0;
					}
					//if (dn2>=EZC)
					//{
					//	dn2=255;
					//}
					//else
					//{
					//	dn2=0;
					//}
					//if (dn3>=EZC)
					//{
					//	dn3=255;
					//}
					//else
					//{
					//	dn3=0;
					//}
					dcMem.SetPixel(j,i,RGB(dn1,dn2,dn3));
					break;				
				}
			case 14://密度分割
				{
					BYTE gray=(dn1+dn2+dn3)/3;
					if(gray<32)
					{
						dn1=0;dn2=0;dn3=0;
						goto loop1;
					}
					if(gray<64)
					{
						dn1=0;dn2=0;dn3=255;
						goto loop1;
					}
					if(gray <96)
					{
						dn1=0;dn2=255;dn3=0;
						goto loop1;
					}
					if(gray <128)
					{
						dn1=0;dn2=255;dn3=255;
						goto loop1;
					}
					if(gray <156)
					{
						dn1=255;dn2=0;dn3=0;
						goto loop1;
					}
					if(gray <192)
					{
						dn1=255;dn2=0;dn3=255;
						goto loop1;
					}
					if(gray <234)
					{
						dn1=255;dn2=255;dn3=0;
						goto loop1;
					}
					dn1=255;dn2=255;dn3=255;
loop1:
					dcMem.SetPixel(j,i,RGB(dn1,dn2,dn3));
					break;				
				}
			}		
			//加权求灰度值
			pafScanblock1++;
			pafScanblock2++;
			pafScanblock3++;
		}
//		szBuffer=pTemp+dwBytes*i;
	}
//	BITMAP bitmap;
	flag1=0;
	MemBitmap.GetBitmap(&bitmap);

	switch(Type_Image)
	{
	case 0://原始显示
//		cdc->BitBlt(0,0,m_viewRect.Width(),m_viewRect.Height(),&dcMem,0,0,SRCCOPY);
		cdc->StretchBlt(OffsetX,OffsetY,nScreenWidth,nScreenHeight,&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
		//GetClientRect(&rect1);
		rect1.left=OffsetX;
		rect1.top=OffsetY;
		rect1.right=OffsetX+nScreenWidth;
		rect1.bottom=OffsetY+nScreenHeight;
		break;
	case 1://图像放大
		//放大显示的矩形区域
		if (rect1.left==0&&rect1.top==0&&rect1.bottom==0&&rect1.right==0)
		{
			//GetClientRect(&rect1);
			rect1.left=OffsetX;
			rect1.top=OffsetY;
			rect1.right=OffsetX+nScreenWidth;
			rect1.bottom=OffsetY+nScreenHeight;
		}
		rect1.left=rect1.left-e;
		rect1.top=rect1.top-e;
		rect1.right=rect1.right+e;
		rect1.bottom=rect1.bottom+e;
		cdc->StretchBlt(rect1.left,rect1.top,rect1.Width(),rect1.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
		break;
	case 2://图像缩小
		//缩小显示的矩形区域
	/*	rect1.left=m_viewRect.left+d;
		rect1.top=m_viewRect.top+d;
		rect1.right=m_viewRect.right-d;
		rect1.bottom=m_viewRect.bottom-d;*/
		rect1.left=rect1.left+d;
		rect1.top=rect1.top+d;
		rect1.right=rect1.right-d;
		rect1.bottom=rect1.bottom-d;
		cdc->StretchBlt(rect1.left,rect1.top,rect1.Width(),rect1.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
		break;
	case 3://图像移动
		int x=EndPoint.x-m_point.x;//记录x移动的大小
		int y=EndPoint.y-m_point.y;//记录y移动的大小
		
		CRect rect=rect1;

		rect1.left=rect1.left+x;
		rect1.top=rect1.top+y;
		rect1.right=rect1.right+x;
		rect1.bottom=rect1.bottom+y;
	
		//改变内存DC中位图的背景色
		
		CBrush   brushBkColor;   
		brushBkColor.CreateSolidBrush(RGB(255,255,255));   
		//dcMem.DPtoLP(rect);   
		//dcMem.FillRect(rect,&brushBkColor); 
		//SetStretchBltMode(dcMem.m_hDC,BLACKONWHITE|WHITEONBLACK);
		/*BITMAPINFO*   pMapinfo;
		setBitMapInfo(pMapinfo, nImageSizeX, nImageSizeY);*/
//		dcMem.StretchBlt(rect1.left,rect1.top,rect1.Width(),rect1.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
	/*	StretchDIBits(dcMem.m_hDC, OffsetX, OffsetY, nBufferSizeX, nBufferSizeY, 0, 0, nImgSizeX, nImgSizeY,   
			pData, pMapinfo, DIB_RGB_COLORS, SRCCOPY); 	*/
		//cdc->BitBlt(rect1.left,rect1.top,rect1.Width(),rect1.Height(),&dcMem,m_viewRect.left,m_viewRect.top,SRCCOPY);
		/*CDC dcMemory;
		dcMemory.CreateCompatibleDC(pDC);
		dcMemory.SelectObject(MemBitmap);
		dcMemory.StretchBlt(rect1.left,rect1.top,rect1.Width(),rect1.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);*/
		//显示在界面上   
		cdc->FillRect(rect,&brushBkColor); 
		cdc->StretchBlt(rect1.left,rect1.top,rect1.Width(),rect1.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
//		cdc->BitBlt(0,0,rect1.Width()+x,rect1.Height()+y,&dcMemory,rect1.left,rect1.top,SRCCOPY);

		break;
	}
	//CPLFree(TempLock1);
	//CPLFree(TempLock2);
	//CPLFree(TempLock3);
	if (!selnum)
	{
		delete pro1;//进度条析构
		cfiles->ShowWindow(SW_SHOW);
		selnum=TRUE;
	}

	//文件列表
	if (cfiles->isAdded==FALSE)
	{
		for(vector<CString>::iterator iter=cfiles->pathname.begin();iter!=cfiles->pathname.end();iter++)
		{
			if (*iter==path)
			{
				goto loop;
			}
		}
		cfiles->setMessage(path,projection,cordin,datum,nBandCount,nImageSizeX,nImageSizeY,adfGeoTransform[0],adfGeoTransform[1],adfGeoTransform[2],adfGeoTransform[3],adfGeoTransform[4],adfGeoTransform[5]);
	}
	
loop:	
	MemBitmap.DeleteObject();
	dcMem.SelectObject(OldBitmap);
	dcMem.DeleteDC();
	ReleaseDC(pDC);
	GDALClose(poDataset);
	return true;
	
}

void ImageShow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	/*CBrush pbrush(&MemBitmap);
	dc.SelectObject(pbrush);
	CRect prect;
	GetClientRect(prect);
	dc.Rectangle(prect);
	*/
	//CDC* P=this->GetDC();
	/*CDC pDC;
	pDC.CreateCompatibleDC(&dc);
	pDC.SelectObject(&MemBitmap);
	Graphics graph(pDC.GetSafeHdc());
	Bitmap* bitmap = ::new Bitmap((HBITMAP)::GetCurrentObject(pDC.GetSafeHdc(), OBJ_BITMAP),NULL);
	graph.DrawImage(bitmap,0,0,0,0,nImageSizeX,nImageSizeY,UnitPixel);
	*/
	/*
	CDC pDC;
	pDC.CreateCompatibleDC(&dc);
	pDC.SelectObject(&MemBitmap);
	BITMAP bitmap;
	CRect rect;
	GetClientRect(&rect);
	MemBitmap.GetBitmap(&bitmap);
	dc.BitBlt(0,0,rect.Width(),rect.Height(),&pDC,0,0,SRCCOPY);
	*/
	ShowImage(BN[0],BN[1],BN[2],&dc);//初始显示图像
	// 不为绘图消息调用 CDialogEx::OnPaint()
}

void ImageShow::OnIdok()
{
	Invalidate(TRUE);
	UpdateWindow();
	// TODO: 在此添加命令处理程序代码
}


void ImageShow::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (BiaoZhi=="OnSubsetimg")
	{
		CPen newpen(PS_SOLID,2,RGB(255,0,0));//创建画笔（实线，线宽2，红色）
		CClientDC dc(this);//获取设备描述表
		CPen *pOldPen=dc.SelectObject(&newpen);//定义画笔指针并制定返回画笔位置
		dc.Rectangle(BeginSub.x,BeginSub.y,EndSub.x,EndSub.y);

		BeginSub.Offset(-OffsetX,-OffsetY);
		BeginSub.x=BeginSub.x*SuoFangBiLi;
		BeginSub.y=BeginSub.y*SuoFangBiLi;

		EndSub.Offset(-OffsetX,-OffsetY);
		EndSub.x=EndSub.x*SuoFangBiLi;
		EndSub.y=EndSub.y*SuoFangBiLi;

		CArthmetic math;
		math.DoModal();
	}
	else if (BiaoZhi=="ROI")
	{
		CRgn rgn;
		int pointNum=Roi.size();
		CPoint* region=new CPoint[pointNum];
		for (int i=0;i<pointNum;i++)
		{
			CPoint n;
			n.x=Roi.at(i).x/SuoFangBiLi+OffsetX;
			n.y=Roi.at(i).y/SuoFangBiLi+OffsetY;
			region[i]=n;
		}
		rgn.CreatePolygonRgn(region,pointNum,WINDING);
		CDC * pdc=GetDC();
		CBrush brush(RGBColor);
		pdc->FillRgn(&rgn,&brush);
		rgn.DeleteObject();
		ReleaseDC(pdc);
		Roi.clear();
		delete[] region;
	}
	else
	{
		CMenu menu,*pmenu;
		menu.LoadMenu(IDR_MENU2);
		pmenu=menu.GetSubMenu(0);

		CPoint ptScreen(point); 
		ClientToScreen(&ptScreen); 

		pmenu->TrackPopupMenu(TPM_RIGHTBUTTON,ptScreen.x,ptScreen.y,this); 
	}
	CDialogEx::OnRButtonUp(nFlags, point);
}


void ImageShow::OnUpdateImage()
{
//	Type_Image=0;
	Invalidate(TRUE);
	UpdateWindow();
	// TODO: 在此添加命令处理程序代码
}


void ImageShow::OnElarge()
{
	Type_Image=1;
	e=e+33;
	curous1=1;
	Invalidate(FALSE);
	UpdateWindow();
	// TODO: 在此添加命令处理程序代码
}


void ImageShow::OnNarrow()
{
	Type_Image=2;
	d=d+33;
	curous1=2;
	Invalidate(TRUE);
	UpdateWindow();
	// TODO: 在此添加命令处理程序代码
}


void ImageShow::OnMobile()
{
	Type_Image=3;
	curous1=3;
	mark=TRUE;
	// TODO: 在此添加命令处理程序代码
}


void ImageShow::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ((MK_LBUTTON & nFlags)&&(mark==TRUE))//判断鼠标左键是否放下
	{
		EndPoint=point;
//		Invalidate(TRUE);
//		UpdateWindow();	
		CDC *dc=GetDC();
		ShowImage(BN[0],BN[1],BN[2],dc);//初始显示图像

	}
	if (info && point.x>=OffsetX && point.y>=OffsetY && point.y<=(OffsetY +nsy) && point.x<=(OffsetX + nsx))
	{
		crgb->x=(int)(point.x-OffsetX);
		crgb->y=(int)(point.y-OffsetY);
		int index=((crgb->y)*nsx+(crgb->x));
		crgb->R=TempLock1[index];
		crgb->G=TempLock2[index];
		crgb->B=TempLock3[index];

		crgb->x_geo=adfGeoTransform[0] + adfGeoTransform[1] * crgb->x + adfGeoTransform[2] * crgb->y;
		crgb->y_geo=adfGeoTransform[3] + adfGeoTransform[4] * crgb->x + adfGeoTransform[5] * crgb->y;
		crgb->updateInfo(crgb->R,crgb->G,crgb->B,crgb->x,crgb->y,crgb->x_geo,crgb->y_geo);
	}
	if ((MK_LBUTTON & nFlags)&&(BiaoZhi=="OnSubsetimg"))
	{
		CPen newpen(PS_SOLID,2,RGB(0,0,255));//创建画笔（实线，线宽2，红色）
		CClientDC dc(this);//获取设备描述表
		CPen *pOldPen=dc.SelectObject(&newpen);//定义画笔指针并制定返回画笔位置

		dc.SetROP2(R2_NOT);//设置逆转当前屏幕颜色的绘图模式(在屏幕上首次画出来的线是可见的，但在同一位置再画一遍，线就不见了，可以方便的实现不断画线，擦线的效果)
		dc.Rectangle(BeginSub.x,BeginSub.y,EndSub.x,EndSub.y);
		dc.Rectangle(BeginSub.x,BeginSub.y,point.x,point.y);
		EndSub=point;
	}
	else if ((MK_LBUTTON & nFlags)&&BiaoZhi=="ROI")
	{
		CPen newpen(PS_SOLID,1,RGBColor);//创建画笔（实线，线宽2，红色）
		CClientDC dc(this);//获取设备描述表
		CPen *pOldPen=dc.SelectObject(&newpen);//定义画笔指针并制定返回画笔位置
		dc.SetROP2(R2_NOT);//设置逆转当前屏幕颜色的绘图模式(在屏幕上首次画出来的线是可见的，但在同一位置再画一遍，线就不见了，可以方便的实现不断画线，擦线的效果)
		dc.MoveTo(m_point);//画线
		dc.LineTo(EndPoint);//保存上一条线的终点
		dc.MoveTo(m_point);//重新画线
		dc.LineTo(point);
		EndPoint=point;
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void ImageShow::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lbtnD=TRUE;

	m_point=point;
	EndPoint=point;

	BeginSub=point;
	EndSub=point;
	if (BiaoZhi=="ROI"&&!(MK_RBUTTON & nFlags))
	{
		point.Offset(-OffsetX,-OffsetY);
		point.x=point.x*SuoFangBiLi;
		point.y=point.y*SuoFangBiLi;
		Roi.push_back(point);//存储感兴趣区域的角点
	}

	switch(Type_Image)
	{
	case 1:
		e=e+33;//放大
		Invalidate(FALSE);
		UpdateWindow();
		break;
	case 2:
		d=d+33;//缩小
		Invalidate(TRUE);
		UpdateWindow();
		break;
	case 3:
		break;
	default:
		break;
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void ImageShow::SuoXiao(/*CPoint point,*/ CDC* pdc1)
{
/*	CDC pDC;
	pDC.CreateCompatibleDC(pdc1);
	pDC.SelectObject(&MemBitmap);
	d=33;//确定缩小倍数
	CRect rect;
	CRect rect1;//缩小显示的矩形区域
	GetClientRect(&rect);

	rect1.left=rect.left+d;
	rect1.top=rect.top+d;
	rect1.right=rect.right-d;
	rect1.bottom=rect.bottom-d;

//	pdc1->StretchBlt(rect1.left,rect1.top,rect1.Width(),rect1.Height(),pdc1,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
	pdc1->StretchBlt(0,0,rect1.Width(),rect1.Height(),pdc1,0,0,bitmap.bmWidth*2,bitmap.bmHeight*2,SRCCOPY);
	d=d*2;
*/
}


void ImageShow::OnRecovery()
{
	// TODO: 在此添加命令处理程序代码
	Type_Image=0;
	curous1=0;
	Invalidate(TRUE);
	UpdateWindow();
}


BOOL ImageShow::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch(curous1)
	{
	case 1://放大
		SetCursor((HCURSOR)::LoadImage(NULL,"zoom_in.ico",IMAGE_ICON,30,30,LR_LOADFROMFILE));
		break;
	case 2://缩小
		SetCursor((HCURSOR)::LoadImage(NULL,"zoom_out.ico",IMAGE_ICON,30,30,LR_LOADFROMFILE));
		break;
	case 3://移动
		if (lbtnD==TRUE)
		{
			SetCursor((HCURSOR)::LoadImage(NULL,"wii-grab-ccw.cur",IMAGE_CURSOR,30,30,LR_LOADFROMFILE));
			break;
		}
		else if (lbtnD==FALSE)
		{
			SetCursor((HCURSOR)::LoadImage(NULL,"wii-open-ccw.cur",IMAGE_CURSOR,30,30,LR_LOADFROMFILE));
			break;
		}
		else
		{
			SetCursor(LoadCursor(NULL,IDC_HAND));
			break;
		}
	default:
		SetCursor(LoadCursor(NULL,IDC_ARROW));
		break;
	}
	return TRUE;
//	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void ImageShow::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	Type_Image=6;
	curous1=6;

	CDialogEx::OnRButtonDown(nFlags, point);
}


void ImageShow::OnRGBtoGray()
{
	flag1=1;
	Invalidate(TRUE);
	UpdateWindow();
	// TODO: 在此添加命令处理程序代码
}


void ImageShow::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	mark=FALSE;
	info=!info;
	if (info==TRUE)
	{
		crgb=new CRGBXY;
		crgb->Create(IDD_DIALOG_RGBXY);
		crgb->ShowWindow(SW_SHOW);
	}
	if(info==FALSE)
	{
		delete crgb;
	}
	CDialogEx::OnLButtonDblClk(nFlags, point);
}


HBRUSH ImageShow::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(nCtlColor ==CTLCOLOR_DLG)
		return m_brush;  //返加白色刷子
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void ImageShow::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	DestroyWindow();
//	CDialogEx::OnClose();
	CPLFree (TempLock1);
	CPLFree (TempLock2);
	CPLFree (TempLock3);
	ShowWindow(SW_HIDE);
}

void setBitMapInfo(BITMAPINFO* pMapinfo,int nXsize,int nYsize)
{
	//设置位图信息头
	pMapinfo->bmiHeader.biBitCount=24;// 每个像素所需的位数，必须是1(双色), 4(16色)，8(256色)或24(真彩色)之一
	pMapinfo->bmiHeader.biClrImportant=0;// 位图显示过程中重要的颜色数
	pMapinfo->bmiHeader.biClrUsed=0;// 位图实际使用的颜色表中的颜色数
	pMapinfo->bmiHeader.biCompression= BI_RGB;//不压缩
	pMapinfo->bmiHeader.biHeight=nYsize;// 位图的高度，以像素为单位
	pMapinfo->bmiHeader.biWidth=nXsize;// 位图的宽度，以像素为单位
	pMapinfo->bmiHeader.biPlanes=1;// 目标设备的平面数不清，必须为1
	pMapinfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);// 本结构所占用字节数
	pMapinfo->bmiHeader.biSizeImage=(nXsize*24 + 31)/32*4 * nYsize; // 位图的大小，以字节为单位
	pMapinfo->bmiHeader.biXPelsPerMeter=0;// 位图水平分辨率，每米像素数
	pMapinfo->bmiHeader.biYPelsPerMeter=0;// 位图垂直分辨率，每米像素数
}

void ImageShow::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lbtnD=FALSE;
	CDialogEx::OnLButtonUp(nFlags, point);
}
