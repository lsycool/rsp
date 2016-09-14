// ISODATA.cpp : 实现文件
//

#include "stdafx.h"
#include "rsp.h"
#include "ISODATA.h"
#include "afxdialogex.h"
#include "rspDlg.h"
#include "math.h"
//#include "ImageShow.h"

CString OPath;//输入影像文件路径
CString RPath;//输出文件路径
CString name;
int CMAX=1;//初始最大类数


// ISODATA 对话框

IMPLEMENT_DYNAMIC(ISODATA, CDialogEx)

ISODATA::ISODATA(CWnd* pParent /*=NULL*/)
	: CDialogEx(ISODATA::IDD, pParent)
{
	m_nTotalClassNum=0; //实际计算的类数
}

ISODATA::~ISODATA()
{
	
}

void ISODATA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Classes);
	DDX_Control(pDX, IDC_EDIT2, m_IniClasses);
	DDX_Control(pDX, IDC_EDIT3, m_LoopNum);
	DDX_Control(pDX, IDC_EDIT4, m_MinPixelNum);
	DDX_Control(pDX, IDC_EDIT5, m_MaxSD);
	DDX_Control(pDX, IDC_EDIT6, m_MinClassDis);
	DDX_Control(pDX, IDC_EDIT7, m_MaxMergeClassNum);
}


BEGIN_MESSAGE_MAP(ISODATA, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ISODATA::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &ISODATA::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &ISODATA::OnBnClickedOk)
END_MESSAGE_MAP()


// ISODATA 消息处理程序


void ISODATA::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	//获得输入文件路径
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.bmp)|*.bmp|JPG文件(*.jpg)|*.jpg;*.jpeg|tiff文件(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="请输入待分类遥感影像";
	if (file.DoModal()==IDOK)
	{
		OPath=file.GetPathName();
		name=file.GetFileTitle();
	}
		SetDlgItemText(IDC_STATIC_IN,OPath);
}


BOOL ISODATA::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	OPath=path;
	SetDlgItemText(IDC_STATIC_IN,OPath);
	//赋初值
	m_Classes.SetWindowText("7");//期望类数
	m_IniClasses.SetWindowText("5");//初始类数
	m_LoopNum.SetWindowText("4");//迭代次数
	m_MaxMergeClassNum.SetWindowText("2");//最大合并类数
	m_MinPixelNum.SetWindowText("0.01");//最少像元数
	m_MaxSD.SetWindowText("6.0");//最大标准差
	m_MinClassDis.SetWindowText("3.0");//最小类间距离

	m_Progress=(CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	m_Progress->SendMessage(PBM_SETBARCOLOR,0,RGB(251,4,29));
	m_Progress->ModifyStyleEx(WS_EX_STATICEDGE,0);
	m_Progress->Invalidate(FALSE);

	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void ISODATA::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	//获得输出文件路径
	name=name+"（分类结果）";
	CFileDialog file(false, "BMP", name, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.BMP)|*.BMP|JPG文件(*.JPG)|*.JPG;*.JPEG|tiff文件(*.TIFF)|*.TIFF;*.TIF|img(*.IMG)|*.IMG|hdr(*.HDR)|*.HDR||");
	file.m_ofn.lpstrTitle="请输入分类结果";
	if (file.DoModal()==IDOK)
	{
		RPath=file.GetPathName();
		switch(file.m_ofn.nFilterIndex)
		{
		case 2:
			{
				if (RPath.Right(4)!=".BMP")
				{
					RPath=RPath+".BMP";
				}
				break;
			}
		case 4:
			{
				if (RPath.Right(5)!=".TIFF")
				{
					RPath=RPath+".TIFF";
				}
				break;
			}
			
		}

	}
	SetDlgItemText(IDC_STATIC_OUT,RPath);
}


void ISODATA::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cs;
	m_Classes.GetWindowText(cs);
	m_iClasses = atoi(cs);//获取期望类数k
	m_IniClasses.GetWindowText(cs);
	m_iIniClasses = atoi(cs);//获得初始类中心个数
	m_LoopNum.GetWindowText(cs);
	m_iLoopNum = atoi(cs);//获得最大迭代次数
	m_MaxMergeClassNum.GetWindowText(cs);
	m_iMaxMergeClassNum = atoi(cs);//获得最大合并类数
	m_MinPixelNum.GetWindowText(cs);
	m_fMinPixelNum = (float)atof(cs);//获得最小像元数
	m_MaxSD.GetWindowText(cs);
	m_fMaxSD = (float)atof(cs);//获得最大标准差
	m_MinClassDis.GetWindowText(cs);
	m_fMinClassDis =(float) atof(cs);//获得最小类间距离


	BeginWaitCursor();//开始等待光标

	m_Progress->SetRange(0,100);

	GDALDataset* poDataset;//GDAL数据集
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(OPath,GA_ReadOnly);
	if(poDataset==NULL)
	{
		AfxMessageBox(_T("打开文件失败！！！"));
		return;
	}
	//GDALRasterBand* Band3;
	//GDALRasterBand* Band4;

	//Band3=poDataset->GetRasterBand(3);
	//Band4=poDataset->GetRasterBand(4);

	int nImageSizeX=poDataset->GetRasterXSize();
	int nImageSizeY=poDataset->GetRasterYSize();
	int Bands = poDataset->GetRasterCount();

	//BYTE* pafScanblock3,*pafScanblock4;
	//pafScanblock3=(BYTE*)CPLMalloc((nImageSizeX)*(nImageSizeY));
	//pafScanblock4=(BYTE*)CPLMalloc((nImageSizeX)*(nImageSizeY));

	//Band3->RasterIO(GF_Read,0,0,nImageSizeX,nImageSizeY,pafScanblock3,nImageSizeX,nImageSizeY,GDT_Byte,0,0);
	//Band4->RasterIO(GF_Read,0,0,nImageSizeX,nImageSizeY,pafScanblock4,nImageSizeX,nImageSizeY,GDT_Byte,0,0);

	//long int sum1=0;
	//long int sum2=0;
	//
	//for (int i=0;i<nImageSizeY;i++)
	//{
	//	for (int j=0;j<nImageSizeX;j++)
	//	{
	//		BYTE dn3=*pafScanblock3;
	//		BYTE dn4=*pafScanblock4;
	//		
	//		sum1=sum1+dn3;//计算第三波段亮度值的总和
	//		sum2=sum2+dn4;//计算第四波段亮度值的总和

	//		pafScanblock3++;
	//		pafScanblock4++;
	//	}
	//}
	//double JUNZHI3;//计算三波段样本均值
	//double JUNZHI4;//计算四波段样本均值
	//double BiaoZhunCha3;//计算标准差
	//double BiaoZhunCha4;
	//
	//JUNZHI3=sum1/(nImageSizeX*nImageSizeY);
	//JUNZHI4=sum2/(nImageSizeX*nImageSizeY);

	//double sum3=0;//计算每个象元与均值的平方和
	//double sum4=0;
	//for(int i=0;i<nImageSizeY;i++)
	//{
	//	for(int j=0;j<nImageSizeX;j++)
	//	{
	//		BYTE dn3=pafScanblock3[i*nImageSizeY+j];
	//		BYTE dn4=pafScanblock4[i*nImageSizeY+j];

	//		sum3=sum3+(dn3-JUNZHI3)*(dn3-JUNZHI3);
	//		sum4=sum4+(dn4-JUNZHI4)*(dn4-JUNZHI4);

	//	}
	//}
	//sum3=sum3/(nImageSizeX*nImageSizeY);
	//sum4=sum4/(nImageSizeX*nImageSizeY);
	//
	////得到标准差
	//BiaoZhunCha3=sqrt(sum3);
	//BiaoZhunCha4=sqrt(sum4);

	////清空聚类中心
	//m_sample.clear();
	//for (int i=0;i<CMAX;i++)
	//{
	//	Sample* pSample=new Sample;
	//	pSample->m_nClassNumber=i;
	//	pSample->pX=JUNZHI3-((CMAX-1)/2)*BiaoZhunCha3+i*BiaoZhunCha3;
	//	pSample->pY=JUNZHI4-((CMAX-1)/2)*BiaoZhunCha4+i*BiaoZhunCha4;
	//	pSample->m_pStandardDifference=0;
	//	m_sample.push_back(pSample);//把类中心点加入类中心容器中
	//	m_nTotalClassNum++;
	//}

	////按照欧氏距离原则把各个样本点分入最邻近的类中
	//int temp=0;
	//int dmin=100000;
	//for(int i=0;i<nImageSizeY;i++)
	//{
	//	for(int j=0;j<nImageSizeX;j++)
	//	{
	//		BYTE dn3=*pafScanblock3;
	//		BYTE dn4=*pafScanblock4;

	//		for (int k=0;k<CMAX;k++)//计算每一个类对应类中心的距离
	//			{
	//				int dx=(dn3-m_sample[k]->pX)*(dn3-m_sample[k]->pX);
	//				int dy=(dn4-m_sample[k]->pY)*(dn3-m_sample[k]->pY);
	//				int dj=dx+dy;

	//				if (dmin>dj)
	//				{
	//					dmin=dj;
	//					temp=k;
	//				}
	//			}
	//		//第一次迭代分类
	//		m_sample[temp]->m_FeatureClassData.push_back(pafScanblock3);
	//		m_sample[temp]->m_FeatureClassData.push_back(pafScanblock4);
	//		m_sample[temp]->m_nClassPixelNum++;
	//		m_sample[temp]->m_nClassPixelNum++;
	//		pafScanblock3++;
	//		pafScanblock4++;
	//	}
	//}
	m_Progress->SetPos(5);//设置进度条进度
	BYTE *InData = new BYTE[nImageSizeX*nImageSizeY*Bands];
	poDataset->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData,nImageSizeX,nImageSizeY, GDT_Byte,Bands, NULL,0,0,0 );
/////////////////////得到影像矩阵范围中的宽和高中的最小值//
	int ImageRange = nImageSizeY;
	if (ImageRange>nImageSizeX)
	{
		ImageRange =nImageSizeX;
	}
	///////初始化中心的采样间距
	int RangeDis = ImageRange/m_iIniClasses;
	int Location = 0;
	for (int i = 0; i < m_iIniClasses; i ++)
	{
		Sample *pClass = new Sample;
		float pCenter ;
		float pStandardDifference = 0;
		//为一个类申请内存后 同样为其存放Center向量和StandardDifference向量申请内存，释放时也应同时释放
		pCenter = InData[Location*nImageSizeX + Location];

		pClass->m_nClassNumber = i;		
		pClass->m_pCenter = pCenter;
		pClass->m_pStandardDifference = pStandardDifference;
		Location = Location + RangeDis;
		m_sample.push_back(pClass);
		m_nTotalClassNum++;
	}

	m_Progress->SetPos(25);

	///////////////②按最近邻规则将样本集{xi}中每个样本分到某一类中
	for (int iBands = 0;iBands<Bands;iBands++)
	{
		for (int i = 0;i<nImageSizeY;i++)
		{
			for (int j = 0;j<nImageSizeX;j++)
			{
				int IdClass = -1;
				float MinDis = 0;
				float Dis = 0;
				for (int iClass = 0;iClass<m_iIniClasses;iClass++)
				{
					Dis = 0;
					Dis = ((float)InData[nImageSizeY*nImageSizeX*iBands+i*nImageSizeX + j] - m_sample[iClass]->m_pCenter)*((float)InData[nImageSizeY*nImageSizeX*iBands+i*nImageSizeX + j] - m_sample[iClass]->m_pCenter);
					if(!iClass)
					{
						MinDis = Dis;
						IdClass = 0;
					}
					if(Dis<MinDis)
					{
						MinDis = Dis;
						IdClass = iClass;
					}
				}
				m_sample[IdClass]->m_FeatureClassData.push_back(InData + nImageSizeY*nImageSizeX*iBands+i*nImageSizeX + j);
				m_sample[IdClass]->m_nClassPixelNum++;
			}
		}
	}

	m_Progress->SetPos(50);

	Classfying();//根据新的聚类中心重新分类
	CalClassMaxMin();//分类后重新计算分类中心

	BYTE *OutData = new BYTE[nImageSizeX*nImageSizeY*3];//输出结果
	int ColorValue = 255/m_nTotalClassNum;//给每一个类分配颜色值

	for (int iBands = 0;iBands<Bands;iBands++)
	{
		for (int i = 0;i<nImageSizeY;i++)
		{
			for (int j = 0;j<nImageSizeX;j++)
			{
				for (int iClass = 0;iClass<m_nTotalClassNum;iClass++)
				{
					ClassMaxMin MyLocal = m_ClassMaxMin[m_sample[iClass]->m_nClassNumber];
					if (InData[nImageSizeY*nImageSizeX*iBands+i*nImageSizeX + j]>=MyLocal.Min&&InData[nImageSizeY*nImageSizeX*iBands+i*nImageSizeX + j]<=MyLocal.Max)
					{
						OutData[nImageSizeY*nImageSizeX*0+i*nImageSizeX + j] = ColorValue*(m_nTotalClassNum-2)*MyLocal.ClassID;
						OutData[nImageSizeY*nImageSizeX*1+i*nImageSizeX + j] = ColorValue*(m_nTotalClassNum-1)*MyLocal.ClassID;
						OutData[nImageSizeY*nImageSizeX*2+i*nImageSizeX + j] = ColorValue*(m_nTotalClassNum)*MyLocal.ClassID;
						break;
					}					
				}
			}
		}
	}
	
	m_Progress->SetPos(80);
	////////////////////////////////将输出影像存储为一个文件//////////////////////////
	char* pszFormat;
	if(RPath.Right(4)==".BMP")
	{
		pszFormat = "BMP";
	}	

	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	GDALDataset *poDstDS;
	char **papszOptions =poDriver->GetMetadata();
	poDstDS = poDriver->Create( (char*)(LPCTSTR)RPath,//输出路径
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
		AfxMessageBox("分类完成！");

	GDALClose(poDstDS);
	delete []OutData;
	delete []InData;

	path=RPath;
	ImageShow* image=new ImageShow;
	image->Create(IDD_IMAGE_SHOW);
	image->MoveWindow(690,75,660,660);
	image->ShowWindow(SW_SHOW);

	CDialogEx::OnOK();
}

Sample::Sample()
{
	m_nClassNumber = 0;
	m_nClassPixelNum = 0;
	n_fMaxSD = 0.0;
	m_nNumOfMaxSD = 0;
	m_pStandardDifference = 0;
	pX=0;
	pY=0;
}

//计算各类的中心
void Sample::CalculateParameters()
{
	float  pTotal = 0;
	for (int j = 0; j < m_nClassPixelNum; j ++)
	{
		pTotal  += *(m_FeatureClassData[j]);
	}
	m_pCenter= pTotal / m_nClassPixelNum;///均值

	///////////////计算各类中样本到类心的平均距离：
	float fTotal = 0;
	for (int i = 0; i < m_nClassPixelNum; i ++)
	{
		fTotal = fTotal + (*m_FeatureClassData[i] - m_pCenter)*(*m_FeatureClassData[i] - m_pCenter);
	}
	fTotal = fTotal/m_nClassPixelNum;
	m_pStandardDifference = sqrtf(fTotal);//类分解的分离值（新类的标准差）
}

void ISODATA::Classfying()
{
	for (int i = 0; i < m_iLoopNum; i ++) ///以下为迭代主程序a、若迭代次数Ip ＝NS，则算法结束；
	{
		for (int i = 0; i < m_nTotalClassNum; i ++)
		{
			m_sample[i]->CalculateParameters();			//计算分类后的参数：各类重心、类内平均距离及总体平均距离
			if (m_sample[i] == NULL)
				continue;
			if(m_sample[i]->m_nClassPixelNum < m_fMinPixelNum)
			{///依据TN判断合并：如果类ωj中样本数nj< TN，则取消该类的中心zj，Nc=Nc-1，转至 。
				DelFeatureClass(m_sample[i]);
				vector<Sample*>::iterator p = m_sample.begin();
				m_sample.erase(p + i);
			}

		}
		////b、若Nc ≤c/2，则转到（将一些类分裂）；
		if (m_nTotalClassNum <= m_iClasses / 2)
		{
			for (int j = 0; j < m_nTotalClassNum; j ++)
			{
				m_sample[j]->CalculateParameters();
				if (m_sample[j]->m_pStandardDifference > m_fMaxSD)
				{
					DivFeatureClass(m_sample[j]);
					vector<Sample*>::iterator p = m_sample.begin();
					m_sample.erase(p + j);
				}
			}	
		}
		////c、若Nc ≥2c，则转至（跳过分裂处理）；（合并处理）。
		else if (m_nTotalClassNum >=2* m_iClasses)
		{	
			UinClassDisVec();
			for (int m = 0; m < (int)m_UinClassDis.size()&& m < m_iMaxMergeClassNum; m ++)
			{
				int nClassA = m_UinClassDis[m].nNumClassA;
				int nClassB = m_UinClassDis[m].nNumClassB;
				UniFeatureClass(m_sample[nClassA], m_sample[nClassB]);
				vector<Sample*>::iterator p = m_sample.begin();
				m_sample.erase(p + nClassB);
			}

		}
		////d、若c/2< Nc<2c，当迭代次数Ip是奇数时转至（分裂处理）；迭代次数Ip是偶数时转至（合并处理）。
		else
		{
			if (m_nTotalClassNum % 2 == 1)
			{
				for (int j = 0; j < m_nTotalClassNum; j ++)
				{
					m_sample[j]->CalculateParameters();
					if (m_sample[j]->m_pStandardDifference > m_fMaxSD)
					{
						DivFeatureClass(m_sample[j]);
						vector<Sample*>::iterator p = m_sample.begin();
						m_sample.erase(p + j);
					}
				}	
			}
			else
			{
				UinClassDisVec();
				for (int m = 0; m < (int)m_UinClassDis.size()&& m < m_iMaxMergeClassNum; m ++)
				{
					int nClassA = m_UinClassDis[m].nNumClassA;
					int nClassB = m_UinClassDis[m].nNumClassB;
					UniFeatureClass(m_sample[nClassA], m_sample[nClassB]);
					vector<Sample*>::iterator p = m_sample.begin();
					m_sample.erase(p + nClassB);
				}
			}
		}
	}
	return;
}

void ISODATA::DelFeatureClass(Sample*  pClass)
{
	if (pClass == NULL)
		return;

	for (int j = 0; j < pClass->m_nClassPixelNum; j ++)
	{
		int IdClass = -1;
		float MinDis = 0;
		float Dis = 0;
		for (int iClass = 0;iClass<m_nTotalClassNum;iClass++)
		{
			Dis = 0;
			Dis = (*(pClass->m_FeatureClassData[j]) - (m_sample[iClass]->m_pCenter))*((*pClass->m_FeatureClassData[j]) - (m_sample[iClass]->m_pCenter));
			if(!iClass)
			{
				MinDis = Dis;
				IdClass = 0;
			}
			if(Dis<MinDis)
			{
				MinDis = Dis;
				IdClass = iClass;
			}
		}
		m_sample[IdClass]->m_FeatureClassData.push_back(pClass->m_FeatureClassData[j]);
		m_sample[IdClass]->m_nClassPixelNum++;		
	}
	m_nTotalClassNum --;
	delete pClass;
	pClass = NULL;
}

bool ISODATA::CalClassMaxMin(void)
{
	for (int i = 0;i<m_nTotalClassNum;i++)
	{
		ClassMaxMin ClassMM;
		ClassMM.ClassID = m_sample[i]->m_nClassNumber;
		ClassMM.Min = 255;
		ClassMM.Max = 0;
		for (int j = 0;j<m_sample[i]->m_nClassPixelNum;j++)
		{
			if (ClassMM.Min>(*m_sample[i]->m_FeatureClassData[j]))
			{
				ClassMM.Min = (*m_sample[i]->m_FeatureClassData[j]);
			}
			if (ClassMM.Max<(*m_sample[i]->m_FeatureClassData[j]))
			{
				ClassMM.Max = (*m_sample[i]->m_FeatureClassData[j]);
			}
		}
		m_ClassMaxMin.push_back(ClassMM);
	}
	return true;
}

//需要进行合并类两类之间的最小距离排序
void ISODATA::UinClassDisVec(void)
{
	for (int i = 0; i < m_nTotalClassNum; i ++)
	{
		m_sample[i]->CalculateParameters();
	}
	for (int k = 0; k < m_nTotalClassNum; k ++)
	{
		for (int h = k + 1; h < m_nTotalClassNum; h ++)
		{
			//相邻两类中心的距离
			float fDis = ((m_sample[k]->m_pCenter) - m_sample[h]->m_pCenter)*(m_sample[k]->m_pCenter - m_sample[h]->m_pCenter);
			fDis = sqrtf(fDis);
			if (fDis < m_fMinClassDis)
			{
				CenterDistant clsDst;
				clsDst.fDis = fDis;//记录两个类之间的
				clsDst.nNumClassA = k;//记录合并的类A
				clsDst.nNumClassB = h;//记录合并的类B
				if (m_UinClassDis.size() == 0)
					m_UinClassDis.push_back(clsDst);
				else if (m_UinClassDis.size() == 1)
				{
					if (fDis < m_UinClassDis[0].fDis)
					{
						vector<CenterDistant>::iterator p = m_UinClassDis.begin();
						m_UinClassDis.insert(p, clsDst);
					}
					else
					{
						m_UinClassDis.push_back(clsDst);
					}
				}
				else
				{
					for (int i = 0; i < m_UinClassDis.size() - 1; i ++)
					{
						if (fDis >= m_UinClassDis[i].fDis && fDis <= m_UinClassDis[i + 1].fDis)
						{
							vector<CenterDistant>::iterator p = m_UinClassDis.begin();
							m_UinClassDis.insert(p + i, clsDst);
						}
						else if (fDis < m_UinClassDis[0].fDis)
						{
							vector<CenterDistant>::iterator p = m_UinClassDis.begin();
							m_UinClassDis.insert(p, clsDst);
						}
						else
							m_UinClassDis.push_back(clsDst);
					}
				}
			}
		}
	}
}

void ISODATA::UniFeatureClass(Sample* pClass,Sample* pClassB)
{
	//计算合并后两个类的均值
	pClass->m_pCenter = (pClass->m_nClassPixelNum*pClass->m_pCenter + pClassB->m_nClassPixelNum*pClassB->m_pCenter)/(pClass->m_nClassPixelNum+pClassB->m_nClassPixelNum);
	//把pClassB类中的样本点加入到pClass类中
	for (int i = 0; i < pClassB->m_nClassPixelNum; i ++)
	{
		BYTE * pfV = pClassB->m_FeatureClassData[i];
		pClass->m_FeatureClassData.push_back(pfV);
		pClass->m_nClassPixelNum++;
	}
	m_nTotalClassNum --;
	pClassB = NULL;
}

//当两个类之间的标准差过大则分裂该类
void ISODATA::DivFeatureClass(Sample* pClass)
{
	if (pClass == NULL)
		return;
	float k = 0.5;				//区间范围
	Sample * pNewClassA = new Sample;
	float  pNewCenterA ;

	Sample * pNewClassS = new Sample;
	float  pNewCenterS ;

	pNewCenterA = (pClass->m_pCenter) + k * pClass->m_pStandardDifference;
	pNewCenterS = (pClass->m_pCenter) - k * pClass->m_pStandardDifference;


	for (int i = 0; i < pClass->m_nClassPixelNum; i ++)
	{
		float fDistanceToNew = 0;
		float fDistanceToOld = 0;
		vector<BYTE*>::iterator p = pClass->m_FeatureClassData.begin(); //迭代器的使用，前面取消聚类中心的操作中也应该使用，但还未使用
		BYTE  *pVector = pClass->m_FeatureClassData[i];

		fDistanceToNew = fDistanceToNew + (pNewCenterA - *pVector) * (pNewCenterA - *pVector);
		fDistanceToOld = fDistanceToOld + (pNewCenterS - *pVector) * (pNewCenterS - *pVector);
		if (fDistanceToNew < fDistanceToOld)
		{
			pNewClassA->m_FeatureClassData.push_back(pVector);
			pNewClassA->m_nClassPixelNum ++;
		}
		else
		{
			pNewClassS->m_FeatureClassData.push_back(pVector);
			pNewClassS->m_nClassPixelNum ++;
		}
	}
	m_nTotalClassNum ++;
	pNewClassA->m_nClassNumber = m_nTotalClassNum - 1;
	pNewClassS->m_nClassNumber = pClass->m_nClassNumber;
	pNewClassA->m_pCenter = pNewCenterA;
	pNewClassS->m_pCenter = pNewCenterS;
	pNewClassA->CalculateParameters();
	pNewClassS->CalculateParameters();
	m_sample.push_back(pNewClassA);
	m_sample.push_back(pNewClassS);
	delete pClass;
	pClass = NULL;
}