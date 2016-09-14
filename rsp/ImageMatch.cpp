// ImageMatch.cpp : 实现文件
//

#include "stdafx.h"
#include "rsp.h"
#include "ImageMatch.h"
#include "afxdialogex.h"


// CImageMatch 对话框

IMPLEMENT_DYNAMIC(CImageMatch, CDialogEx)

CImageMatch::CImageMatch(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageMatch::IDD, pParent)
{

	m_threL = 1000;
	m_threR = 1000;
	m_winL1 = 7;
	m_winL2 = 5;
	m_winR1 = 7;
	m_winR2 = 5;
	m_scare = 11;
	m_cort = 0.9;
}

CImageMatch::~CImageMatch()
{
}

void CImageMatch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MATCHTHRE2, m_threL);
	DDX_Text(pDX, IDC_EDIT_MATCHTHRE3, m_threR);
	DDX_Text(pDX, IDC_EDIT_MATCHWIN3, m_winL1);
	DDX_Text(pDX, IDC_EDIT_MATCHWIN4, m_winL2);
	DDX_Text(pDX, IDC_EDIT_MATCHWIN5, m_winR1);
	DDX_Text(pDX, IDC_EDIT_MATCHWIN6, m_winR2);
	DDX_Text(pDX, IDC_EDIT_SCARE, m_scare);
	DDX_Text(pDX, IDC_EDIT2_MATCHCORT, m_cort);
}


BEGIN_MESSAGE_MAP(CImageMatch, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MOIN, &CImageMatch::OnBnClickedButtonMoin)
	ON_BN_CLICKED(IDC_BUTTON_MOIN2, &CImageMatch::OnBnClickedButtonMoin2)
	ON_BN_CLICKED(IDC_BUTTON_MOUT, &CImageMatch::OnBnClickedButtonMout)
	ON_BN_CLICKED(IDC_BUTTON_MOUT2, &CImageMatch::OnBnClickedButtonMout2)
	ON_BN_CLICKED(IDC_BUTTON_CODAT, &CImageMatch::OnBnClickedButtonCodat)
	ON_BN_CLICKED(IDOK, &CImageMatch::OnBnClickedOk)
END_MESSAGE_MAP()


// CImageMatch 消息处理程序

//输入左影像
void CImageMatch::OnBnClickedButtonMoin()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.bmp)|*.bmp|JPG文件(*.jpg)|*.jpg;*.jpeg|tiff文件(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="请输入待提取moravec遥感影像";
	if (file.DoModal()==IDOK)
	{
		MH_inL=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_INMATCHL,MH_inL);

}


void CImageMatch::OnBnClickedButtonMoin2()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.bmp)|*.bmp|JPG文件(*.jpg)|*.jpg;*.jpeg|tiff文件(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="请输入待提取moravec遥感影像";
	if (file.DoModal()==IDOK)
	{
		MH_inR=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_INMATCHR,MH_inR);

}

//输出左影像
void CImageMatch::OnBnClickedButtonMout()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(false, "BMP", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.BMP)|*.BMP|JPG文件(*.JPG)|*.JPG;*.JPEG|tiff文件(*.TIFF)|*.TIFF;*.TIF|img(*.IMG)|*.IMG|hdr(*.HDR)|*.HDR||");
	file.m_ofn.lpstrTitle="请输入结果影像";
	if (file.DoModal()==IDOK)
	{
		MH_outL=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_OUTMATCHL,MH_outL);
}


void CImageMatch::OnBnClickedButtonMout2()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(false, "BMP", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.BMP)|*.BMP|JPG文件(*.JPG)|*.JPG;*.JPEG|tiff文件(*.TIFF)|*.TIFF;*.TIF|img(*.IMG)|*.IMG|hdr(*.HDR)|*.HDR||");
	file.m_ofn.lpstrTitle="请输入结果影像";
	if (file.DoModal()==IDOK)
	{
		MH_outR=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_OUTMATCHR,MH_outR);
}

//输出匹配点
void CImageMatch::OnBnClickedButtonCodat()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(false, "txt", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|文本(*.txt)|*.txt||");
	file.m_ofn.lpstrTitle="请输入兴趣点文件";
	if (file.DoModal()==IDOK)
	{
		MH_data=file.GetPathName();	
	}
	SetDlgItemText(IDC_STATIC_OUTMATCHDATA,MH_data);
}



//参数说明：Data：存放影像数据；nImageSizeX：返回影像宽度；nImageSizeY：返回影像长度；FilePath：打开影像的路径；
void CImageMatch::ReadImage(BYTE* &Data,int &nImageSizeX,int &nImageSizeY,CString FilePath)
{
	GDALDataset* poDataset;//GDAL数据集
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(FilePath,GA_ReadOnly);

	if(poDataset==NULL)
	{
		AfxMessageBox(_T("打开文件失败！！！"));
		return;
	}

	nImageSizeX=poDataset->GetRasterXSize();
	nImageSizeY=poDataset->GetRasterYSize();

	Data = new BYTE[nImageSizeX*nImageSizeY*3];
	poDataset->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, Data,nImageSizeX,nImageSizeY, GDT_Byte,3, NULL,0,0,0 );
	GDALClose(poDataset);
}


// 存储影像
//参数说明：Indata：要输出的影像；nImageSizeX：影像宽度；nImageSizeY：影像长度；OutPath：输出路径；ImageType：存储格式
void CImageMatch::SaveImage(BYTE* Outdata,int nImageSizeX,int nImageSizeY,CString OutPath, CString ImageType)
{
	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(ImageType);
	GDALDataset *poDstDS;
	char **papszOptions =poDriver->GetMetadata();
	poDstDS = poDriver->Create( (char*)(LPCTSTR)OutPath,//输出路径
		nImageSizeX,nImageSizeY, //影像数据宽高
		3, //存或取的波段数
		GDT_Byte, 	//缓冲区数据类型
		papszOptions );

	if(poDstDS->RasterIO(
		GF_Write,						//IO方式
		0,0,							//影像数据起始位置x,y
		nImageSizeX,nImageSizeY,				//影像数据宽高
		Outdata,						//指向缓冲区数据指针
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
	GDALClose(poDstDS);
}


// 存储匹配点
void CImageMatch::SaveToTxt(vector<MATCHEDPOINTS> MatchedPoints,float thre,int Wsize,CString OutFile)
{
	FILE*  pTxtFile= fopen(OutFile, "wt");

	if (!pTxtFile)
		return ;

	fprintf(pTxtFile, "**********\t文档说明：%s\n**********\t程序所有人：%s\n**********\t单位: %s\n"," 相关系数匹配点对输出文档","刘世永", "中国地质大学");
	fprintf(pTxtFile, "**********\t相关系数阈值: %f\n", thre);
	fprintf(pTxtFile, "**********\t匹配窗口大小: %d\n",Wsize);
	fprintf(pTxtFile, "**********\t匹配点数: %d\n\n",MatchedPoints.size());
	fprintf(pTxtFile, "X_L\tY_L\tX_R\tY_R\t相关系数\n");

	vector<MATCHEDPOINTS>::iterator iter;

	for (iter = MatchedPoints.begin(); iter != MatchedPoints.end(); ++iter)
		fprintf(pTxtFile, "%d\t%d\t%d\t%d\t%f\n",
		iter->x1, iter->y1, iter->x2, iter->y2, iter->coefficient);

	fclose(pTxtFile);

}


// //计算莫罗维克算子
void CImageMatch::Movarec(BYTE* Indata1,vector<FEATUREPOINT> &FeaturePoint,int ImageSizeX,int ImageSizeY,int m_win1,int m_win2,double m_thre)
{
	int k=m_win1/2;
	//int Count1=0;//候选点个数
	//int Count3=0;

	float* pInterest = new float[ImageSizeX*ImageSizeY];
	ZeroMemory(pInterest, sizeof(float)*ImageSizeX*ImageSizeY);//赋初值清零(与memset函数差不多不过只能用于win32程序)

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
	}

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
			}
		}
	}


	int k2 = m_win2 / 2;
	float max2(0);
	int tempX(0), tempY(0);
	bool b(false);

	FEATUREPOINT tempFP;
	ZeroMemory(&tempFP, sizeof(FEATUREPOINT));

	for(int i = k2; i < ImageSizeY - k2; i += m_win2)
	{
		for(int j = k2; j < ImageSizeX - k2; j += m_win2)
		{
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
			}
		}
	}



	//去除重复控制点
	vector<FEATUREPOINT> ret;
	vector<FEATUREPOINT>::iterator iter = FeaturePoint.begin();

	ret.clear();
	ret.push_back(*iter);

	BOOL b1(FALSE);

	for (iter = FeaturePoint.begin(); iter != FeaturePoint.end(); ++iter)
	{
		b1 = false;

		for (vector<FEATUREPOINT>::iterator iter2 = ret.begin(); iter2 != ret.end(); ++iter2)
		{
			if (iter->x == iter2->x && iter->y == iter2->y)
			{
				b1 = TRUE;	//存在
				break;
			}
		}

		if (b1 == FALSE)
		{
			ret.push_back(*iter);
		}
	}
	FeaturePoint=ret;

	//保存结果位图
	//vector<FEATUREPOINT>::iterator iter;
	//for (iter = FeaturePoint2.begin(); iter != FeaturePoint2.end(); ++iter)
	//{
	//	int i = iter->y;
	//	int j = iter->x;

	//	//第一波段
	//	outdata[i*ImageSizeY+j] = 255;

	//	outdata[i*ImageSizeY+j+1] = 255;
	//	outdata[i*ImageSizeY+j-1] = 255;
	//	outdata[(i+1)*ImageSizeY+j] = 255;
	//	outdata[(i-1)*ImageSizeY+j] = 255;

	//	outdata[i*ImageSizeY+j+2] = 255;
	//	outdata[i*ImageSizeY+j-2] = 255;
	//	outdata[(i+2)*ImageSizeY+j] = 255;
	//	outdata[(i-2)*ImageSizeY+j] = 255;


	//	//第二波段
	//	outdata[ImageSizeX*ImageSizeY+i*ImageSizeY+j] = 0;

	//	outdata[ImageSizeX*ImageSizeY+i*ImageSizeY+j+1] = 0;
	//	outdata[ImageSizeX*ImageSizeY+i*ImageSizeY+j-1] = 0;
	//	outdata[ImageSizeX*ImageSizeY+(i+1)*ImageSizeY+j] = 0;
	//	outdata[ImageSizeX*ImageSizeY+(i-1)*ImageSizeY+j] = 0;

	//	outdata[ImageSizeX*ImageSizeY+i*ImageSizeY+j+2] = 0;
	//	outdata[ImageSizeX*ImageSizeY+i*ImageSizeY+j-2] = 0;
	//	outdata[ImageSizeX*ImageSizeY+(i+2)*ImageSizeY+j] = 0;
	//	outdata[ImageSizeX*ImageSizeY+(i-2)*ImageSizeY+j] = 0;

	//	//第三波段
	//	outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeY+j] = 0;

	//	outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeY+j+1] = 0;
	//	outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeY+j-1] = 0;
	//	outdata[ImageSizeX*ImageSizeY*2+(i+1)*ImageSizeY+j] = 0;
	//	outdata[ImageSizeX*ImageSizeY*2+(i-1)*ImageSizeY+j] = 0;

	//	outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeY+j+2] = 0;
	//	outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeY+j-2] = 0;
	//	outdata[ImageSizeX*ImageSizeY*2+(i+2)*ImageSizeY+j] = 0;
	//	outdata[ImageSizeX*ImageSizeY*2+(i-2)*ImageSizeY+j] = 0;

		//释放内存
	delete[] pInterest;
	delete[] Indata;
	delete[] TempD1;
	delete[] TempD2;
	delete[] TempD3;

}


void CImageMatch::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (MH_inL==""||MH_inR==""||MH_outL==""||MH_outR==""||MH_data=="")
	{
		AfxMessageBox("输入或输出文件不能为空！");
		return;
	}


	//BYTE* ImageL=new BYTE[5000*5000*3];//存储左影像
	//ZeroMemory(ImageL,sizeof(BYTE)*5000*5000*3);
	//BYTE* ImageR=new BYTE[5000*5000*3];//存储右影像
	//ZeroMemory(ImageR,sizeof(BYTE)*5000*5000*3);
	BYTE* ImageL;
	BYTE* ImageR;

	int nImageSizeX_L;//左影像长宽;
	int nImageSizeY_L;
	int nImageSizeX_R;//右影像长宽
	int nImageSizeY_R;
	
	AfxMessageBox("计算时间较长请耐心等待。。。");

	BeginWaitCursor();

	ReadImage(ImageL,nImageSizeX_L,nImageSizeY_L,MH_inL);//读取左影像
	ReadImage(ImageR,nImageSizeX_R,nImageSizeY_R,MH_inR);//读取右影像

	//开始计算左右影像特征点
	vector<FEATUREPOINT> FeaturePointLeft;
	vector<FEATUREPOINT> FeaturePointRight;
	Movarec(ImageL,FeaturePointLeft,nImageSizeX_L,nImageSizeY_L,m_winL1,m_winL2,m_threL);
	Movarec(ImageR,FeaturePointRight,nImageSizeX_R,nImageSizeY_R,m_winR1,m_winR2,m_threR);

	//特征点计算完毕，存储于vector中
	//遍历vector, 在右片中搜寻相关系数最大的窗口
	vector<FEATUREPOINT>::iterator iter1;
	vector<FEATUREPOINT>::iterator iter2;

	int k = m_scare/2;
	int i(0), j(0), m(0), n(0);
	//BYTE* pTemplate = new BYTE[m_scare*m_scare];	//存储模板窗口
	//ZeroMemory(pTemplate, sizeof(BYTE)*m_scare*m_scare);

	//BYTE* pTarget = new BYTE[m_scare*m_scare];		//存储搜索窗口
	//ZeroMemory(pTarget, sizeof(BYTE)*m_scare*m_scare);

	vector<MATCHEDPOINTS> MatchedPoints;//存取符合条件相关系数的点
	float MaxR(0);
	float R(0);
	double A(0), B(0), C(0), D(0), E(0);	//计算相关系数的中间量

	int lMaxWidth;
	int lMaxHeight;
	int Count3(0), Count4(0);

	for (iter1 = FeaturePointLeft.begin(); iter1 != FeaturePointLeft.end(); ++iter1)
	{
		//Count2++;
		//计算该点对应的特征点

		B = 0.0;
		D = 0.0;

		MaxR = 0.0;
		R = 0.0;

		if (iter1->x < k || iter1->y < k || iter1->x > nImageSizeY_L - k || iter1->y > nImageSizeX_L - k)
		{
			Count3++;
			continue;
		}

		//可先求出B,D
		for (i = 0;i < m_scare;++i)
		{
			for(j = 0;j < m_scare;++j)
			{
				//B = 小窗口所有特征点像素灰度值求和
				//D = ..........................的平方再求和
				B += ImageL[(i+iter1->y)*nImageSizeX_L + (j+iter1->x)];
				D += ImageL[(i+iter1->y)*nImageSizeX_L + (j+iter1->x)] * ImageL[(i+iter1->y)*nImageSizeX_L + (j+iter1->x)];
			}
		}
		Count4 = 0;

		//逐个小窗口计算相关系数R
		for (iter2 = FeaturePointRight.begin(); iter2 != FeaturePointRight.end(); ++iter2)
		{
			if (iter2->x < k || iter2->y < k || iter2->x > nImageSizeY_R - k || iter2->y > nImageSizeX_R - k)
			{
				Count4++;
				continue;
			}

			A = 0.0;
			C = 0.0;
			E = 0.0;

			j = iter2->x;	//i,j为中心点
			i = iter2->y;

			j -= k;			//i,j为顶点
			i -= k;

			//这里的centerX，centerY就是右片某个小窗口的中心
			int centerX = j + k;
			int centerY = i + k;

			//左影像的特征点与右影像的所有特征点进行匹配
			//求A,C,E
			for (m = 0; m < m_scare; ++m)
			{
				for(n = 0; n < m_scare ; ++n)
				{
					//C = 右影像小窗口所有像素灰度值求和
					//E = ........................  平方的求和
					//A = 对应位置像素灰度值乘积之和
					C += ImageR[(i+m)*nImageSizeX_R + (j+n)];
					E += ImageR[(i+m)*nImageSizeX_R+(j+n)]*ImageR[(i+m)*nImageSizeX_R+(j+n)];
					A += ImageR[(i+m)*nImageSizeX_R+(j+n)] *ImageL[(m+iter1->y)*nImageSizeX_L + (n+iter1->x)];
				}
			}

			double temp1 = D - (B*B)/(m_scare*m_scare);//B = 以特征点为中心小窗口内像素灰度值之和
			double temp2 = E - (C*C)/(m_scare*m_scare);//D =.................................的平方再求和

			if (temp1 != 0 && temp2 != 0)
			{
				R = (A - B*C/(m_scare*m_scare)) / sqrt(temp1*temp2);//计算相关系数
			}
			else
				R = 0;


			//找出最大的相关系数
			if (R > MaxR)
			{
				MaxR = R;
				lMaxWidth = centerX;
				lMaxHeight = centerY;
			}
		}

		int Distance=(iter1->x-lMaxWidth)*(iter1->x-lMaxWidth)+(iter1->y-lMaxHeight)*(iter1->y-lMaxHeight);
		//把相关系数大于阈值的搜索窗口作为模板窗口的备选共轭窗口
		if (MaxR >= m_cort&&Distance<=900)
		{
			MATCHEDPOINTS mp;
			mp.coefficient = MaxR;
			mp.x1 = iter1->x;
			mp.y1 = iter1->y;
			mp.x2 = lMaxWidth;
			mp.y2 = lMaxHeight;

			MatchedPoints.push_back(mp);
		}
	}

	TRY {
		DrawCross(ImageL,ImageR, MatchedPoints,nImageSizeX_L,nImageSizeX_R,nImageSizeY_L,nImageSizeY_R);
		SaveImage(ImageL,nImageSizeX_L,nImageSizeY_L,MH_outL,"BMP");
		SaveImage(ImageR,nImageSizeX_R,nImageSizeY_R,MH_outR,"BMP");
		SaveToTxt(MatchedPoints,m_cort,m_scare,MH_data);
		int IDType=AfxMessageBox("匹配完成！，是否立即打开匹配结果影像和匹配点对文档？",MB_YESNO|MB_ICONINFORMATION);
		//打开保存结果
		if (IDType==IDYES)
		{
			ShellExecute(NULL,"open",MH_outL,NULL,NULL,SW_SHOWNORMAL);
			ShellExecute(NULL,"open",MH_outR,NULL,NULL,SW_SHOWNORMAL);
			ShellExecute(NULL,"open",MH_data,NULL,NULL,SW_SHOWNORMAL);
		}
		
	}
	CATCH (CMemoryException, e)
	{
		AfxMessageBox("保存文件失败！");
		return;
	}
	END_CATCH

	EndWaitCursor();

	delete[] ImageL;
	delete[] ImageR;
	CDialogEx::OnOK();
}

//影像画十字
void CImageMatch::DrawCross(BYTE* pBits1, BYTE* pBits2, vector<MATCHEDPOINTS> MatchedPoints,int _width1, int _width2, int _height1, int _height2)
{
	vector<MATCHEDPOINTS>::iterator iter;

	int i(0), j(0);

	//for (i = 0; i < byteCount1; ++i)
	//{
	//	if (pBits1[i] == 255)
	//		pBits1[i] = 254;
	//}

	//for (j = 0; j < byteCount2; ++j)
	//{
	//	if (pBits2[j] == 255)
	//		pBits2[j] = 254;
	//}

	for (iter = MatchedPoints.begin(); iter != MatchedPoints.end(); ++iter)
	{
		i = iter->y1;
		j = iter->x1;

		//一波段
		pBits1[i*_width1+j] = 255;

		pBits1[i*_width1+j+1] = 255;
		pBits1[i*_width1+j-1] = 255;
		pBits1[(i+1)*_width1+j] = 255;
		pBits1[(i-1)*_width1+j] = 255;

		pBits1[i*_width1+j+2] = 255;
		pBits1[i*_width1+j-2] = 255;
		pBits1[(i+2)*_width1+j] = 255;
		pBits1[(i-2)*_width1+j] = 255;
		//二波段
		pBits1[_width1*_height1+i*_width1+j] = 0;

		pBits1[_width1*_height1+i*_width1+j+1] = 0;
		pBits1[_width1*_height1+i*_width1+j-1] = 0;
		pBits1[_width1*_height1+(i+1)*_width1+j] = 0;
		pBits1[_width1*_height1+(i-1)*_width1+j] = 0;

		pBits1[_width1*_height1+i*_width1+j+2] = 0;
		pBits1[_width1*_height1+i*_width1+j-2] = 0;
		pBits1[_width1*_height1+(i+2)*_width1+j] = 0;
		pBits1[_width1*_height1+(i-2)*_width1+j] = 0;
		//三波段
		pBits1[_width1*_height1*2+i*_width1+j] = 0;

		pBits1[_width1*_height1*2+i*_width1+j+1] = 0;
		pBits1[_width1*_height1*2+i*_width1+j-1] = 0;
		pBits1[_width1*_height1*2+(i+1)*_width1+j] = 0;
		pBits1[_width1*_height1*2+(i-1)*_width1+j] = 0;

		pBits1[_width1*_height1*2+i*_width1+j+2] = 0;
		pBits1[_width1*_height1*2+i*_width1+j-2] = 0;
		pBits1[_width1*_height1*2+(i+2)*_width1+j] = 0;
		pBits1[_width1*_height1*2+(i-2)*_width1+j] = 0;

		i = iter->y2;
		j = iter->x2;

		//一波段
		pBits2[i*_width2+j] = 255;	

		pBits2[i*_width2+j+1] = 255;
		pBits2[i*_width2+j-1] = 255;
		pBits2[(i+1)*_width2+j] = 255;
		pBits2[(i-1)*_width2+j] = 255;

		pBits2[i*_width2+j+2] = 255;
		pBits2[i*_width2+j-2] = 255;
		pBits2[(i+2)*_width2+j] = 255;
		pBits2[(i-2)*_width2+j] = 255;
		//二波段
		pBits2[_width2*_height2+i*_width2+j] = 0;	

		pBits2[_width2*_height2+i*_width2+j+1] = 0;
		pBits2[_width2*_height2+i*_width2+j-1] = 0;
		pBits2[_width2*_height2+(i+1)*_width2+j] = 0;
		pBits2[_width2*_height2+(i-1)*_width2+j] = 0;

		pBits2[_width2*_height2+i*_width2+j+2] = 0;
		pBits2[_width2*_height2+i*_width2+j-2] = 0;
		pBits2[_width2*_height2+(i+2)*_width2+j] = 0;
		pBits2[_width2*_height2+(i-2)*_width2+j] = 0;
		//三波段
		pBits2[_width2*_height2*2+i*_width2+j] = 0;	

		pBits2[_width2*_height2*2+i*_width2+j+1] = 0;
		pBits2[_width2*_height2*2+i*_width2+j-1] = 0;
		pBits2[_width2*_height2*2+(i+1)*_width2+j] = 0;
		pBits2[_width2*_height2*2+(i-1)*_width2+j] = 0;

		pBits2[_width2*_height2*2+i*_width2+j+2] = 0;
		pBits2[_width2*_height2*2+i*_width2+j-2] = 0;
		pBits2[_width2*_height2*2+(i+2)*_width2+j] = 0;
		pBits2[_width2*_height2*2+(i-2)*_width2+j] = 0;
	}
}


BOOL CImageMatch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
