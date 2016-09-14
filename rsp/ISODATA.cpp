// ISODATA.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rsp.h"
#include "ISODATA.h"
#include "afxdialogex.h"
#include "rspDlg.h"
#include "math.h"
//#include "ImageShow.h"

CString OPath;//����Ӱ���ļ�·��
CString RPath;//����ļ�·��
CString name;
int CMAX=1;//��ʼ�������


// ISODATA �Ի���

IMPLEMENT_DYNAMIC(ISODATA, CDialogEx)

ISODATA::ISODATA(CWnd* pParent /*=NULL*/)
	: CDialogEx(ISODATA::IDD, pParent)
{
	m_nTotalClassNum=0; //ʵ�ʼ��������
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


// ISODATA ��Ϣ�������


void ISODATA::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��������ļ�·��
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="�����������ң��Ӱ��";
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	OPath=path;
	SetDlgItemText(IDC_STATIC_IN,OPath);
	//����ֵ
	m_Classes.SetWindowText("7");//��������
	m_IniClasses.SetWindowText("5");//��ʼ����
	m_LoopNum.SetWindowText("4");//��������
	m_MaxMergeClassNum.SetWindowText("2");//���ϲ�����
	m_MinPixelNum.SetWindowText("0.01");//������Ԫ��
	m_MaxSD.SetWindowText("6.0");//����׼��
	m_MinClassDis.SetWindowText("3.0");//��С������

	m_Progress=(CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	m_Progress->SendMessage(PBM_SETBARCOLOR,0,RGB(251,4,29));
	m_Progress->ModifyStyleEx(WS_EX_STATICEDGE,0);
	m_Progress->Invalidate(FALSE);

	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void ISODATA::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//�������ļ�·��
	name=name+"����������";
	CFileDialog file(false, "BMP", name, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.BMP)|*.BMP|JPG�ļ�(*.JPG)|*.JPG;*.JPEG|tiff�ļ�(*.TIFF)|*.TIFF;*.TIF|img(*.IMG)|*.IMG|hdr(*.HDR)|*.HDR||");
	file.m_ofn.lpstrTitle="�����������";
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cs;
	m_Classes.GetWindowText(cs);
	m_iClasses = atoi(cs);//��ȡ��������k
	m_IniClasses.GetWindowText(cs);
	m_iIniClasses = atoi(cs);//��ó�ʼ�����ĸ���
	m_LoopNum.GetWindowText(cs);
	m_iLoopNum = atoi(cs);//�������������
	m_MaxMergeClassNum.GetWindowText(cs);
	m_iMaxMergeClassNum = atoi(cs);//������ϲ�����
	m_MinPixelNum.GetWindowText(cs);
	m_fMinPixelNum = (float)atof(cs);//�����С��Ԫ��
	m_MaxSD.GetWindowText(cs);
	m_fMaxSD = (float)atof(cs);//�������׼��
	m_MinClassDis.GetWindowText(cs);
	m_fMinClassDis =(float) atof(cs);//�����С������


	BeginWaitCursor();//��ʼ�ȴ����

	m_Progress->SetRange(0,100);

	GDALDataset* poDataset;//GDAL���ݼ�
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(OPath,GA_ReadOnly);
	if(poDataset==NULL)
	{
		AfxMessageBox(_T("���ļ�ʧ�ܣ�����"));
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
	//		sum1=sum1+dn3;//���������������ֵ���ܺ�
	//		sum2=sum2+dn4;//������Ĳ�������ֵ���ܺ�

	//		pafScanblock3++;
	//		pafScanblock4++;
	//	}
	//}
	//double JUNZHI3;//����������������ֵ
	//double JUNZHI4;//�����Ĳ���������ֵ
	//double BiaoZhunCha3;//�����׼��
	//double BiaoZhunCha4;
	//
	//JUNZHI3=sum1/(nImageSizeX*nImageSizeY);
	//JUNZHI4=sum2/(nImageSizeX*nImageSizeY);

	//double sum3=0;//����ÿ����Ԫ���ֵ��ƽ����
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
	////�õ���׼��
	//BiaoZhunCha3=sqrt(sum3);
	//BiaoZhunCha4=sqrt(sum4);

	////��վ�������
	//m_sample.clear();
	//for (int i=0;i<CMAX;i++)
	//{
	//	Sample* pSample=new Sample;
	//	pSample->m_nClassNumber=i;
	//	pSample->pX=JUNZHI3-((CMAX-1)/2)*BiaoZhunCha3+i*BiaoZhunCha3;
	//	pSample->pY=JUNZHI4-((CMAX-1)/2)*BiaoZhunCha4+i*BiaoZhunCha4;
	//	pSample->m_pStandardDifference=0;
	//	m_sample.push_back(pSample);//�������ĵ����������������
	//	m_nTotalClassNum++;
	//}

	////����ŷ�Ͼ���ԭ��Ѹ���������������ڽ�������
	//int temp=0;
	//int dmin=100000;
	//for(int i=0;i<nImageSizeY;i++)
	//{
	//	for(int j=0;j<nImageSizeX;j++)
	//	{
	//		BYTE dn3=*pafScanblock3;
	//		BYTE dn4=*pafScanblock4;

	//		for (int k=0;k<CMAX;k++)//����ÿһ�����Ӧ�����ĵľ���
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
	//		//��һ�ε�������
	//		m_sample[temp]->m_FeatureClassData.push_back(pafScanblock3);
	//		m_sample[temp]->m_FeatureClassData.push_back(pafScanblock4);
	//		m_sample[temp]->m_nClassPixelNum++;
	//		m_sample[temp]->m_nClassPixelNum++;
	//		pafScanblock3++;
	//		pafScanblock4++;
	//	}
	//}
	m_Progress->SetPos(5);//���ý���������
	BYTE *InData = new BYTE[nImageSizeX*nImageSizeY*Bands];
	poDataset->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData,nImageSizeX,nImageSizeY, GDT_Byte,Bands, NULL,0,0,0 );
/////////////////////�õ�Ӱ�����Χ�еĿ�͸��е���Сֵ//
	int ImageRange = nImageSizeY;
	if (ImageRange>nImageSizeX)
	{
		ImageRange =nImageSizeX;
	}
	///////��ʼ�����ĵĲ������
	int RangeDis = ImageRange/m_iIniClasses;
	int Location = 0;
	for (int i = 0; i < m_iIniClasses; i ++)
	{
		Sample *pClass = new Sample;
		float pCenter ;
		float pStandardDifference = 0;
		//Ϊһ���������ڴ�� ͬ��Ϊ����Center������StandardDifference���������ڴ棬�ͷ�ʱҲӦͬʱ�ͷ�
		pCenter = InData[Location*nImageSizeX + Location];

		pClass->m_nClassNumber = i;		
		pClass->m_pCenter = pCenter;
		pClass->m_pStandardDifference = pStandardDifference;
		Location = Location + RangeDis;
		m_sample.push_back(pClass);
		m_nTotalClassNum++;
	}

	m_Progress->SetPos(25);

	///////////////�ڰ�����ڹ���������{xi}��ÿ�������ֵ�ĳһ����
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

	Classfying();//�����µľ����������·���
	CalClassMaxMin();//��������¼����������

	BYTE *OutData = new BYTE[nImageSizeX*nImageSizeY*3];//������
	int ColorValue = 255/m_nTotalClassNum;//��ÿһ���������ɫֵ

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
	////////////////////////////////�����Ӱ��洢Ϊһ���ļ�//////////////////////////
	char* pszFormat;
	if(RPath.Right(4)==".BMP")
	{
		pszFormat = "BMP";
	}	

	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	GDALDataset *poDstDS;
	char **papszOptions =poDriver->GetMetadata();
	poDstDS = poDriver->Create( (char*)(LPCTSTR)RPath,//���·��
		nImageSizeX,nImageSizeY, //Ӱ�����ݿ��
		3, //���ȡ�Ĳ�����
		GDT_Byte, 	//��������������
		papszOptions );

	EndWaitCursor();
	m_Progress->SetPos(100);

	if(poDstDS->RasterIO(
		GF_Write,						//IO��ʽ
		0,0,							//Ӱ��������ʼλ��x,y
		nImageSizeX,nImageSizeY,				//Ӱ�����ݿ��
		OutData,						//ָ�򻺳�������ָ��
		nImageSizeX,nImageSizeY,				//���������ݿ��
		GDT_Byte,						//��������������
		3,						//���ȡ�Ĳ�����
		NULL,							//���ȡ�Ĳ����б�
		0,								//����������һ������ռ�õ��ֽ���
		0,								//������һ��������ռ�õ��ֽ���
		0)==CE_Failure )				//���������ݴ洢��ʽ 0ΪRRRRGGGGBBBB-bsp 1ΪRGBRGBRGB-bip	
	{
		AfxMessageBox("�����ļ�ʧ�ܣ�");
		return;
	}
	else
		AfxMessageBox("������ɣ�");

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

//������������
void Sample::CalculateParameters()
{
	float  pTotal = 0;
	for (int j = 0; j < m_nClassPixelNum; j ++)
	{
		pTotal  += *(m_FeatureClassData[j]);
	}
	m_pCenter= pTotal / m_nClassPixelNum;///��ֵ

	///////////////������������������ĵ�ƽ�����룺
	float fTotal = 0;
	for (int i = 0; i < m_nClassPixelNum; i ++)
	{
		fTotal = fTotal + (*m_FeatureClassData[i] - m_pCenter)*(*m_FeatureClassData[i] - m_pCenter);
	}
	fTotal = fTotal/m_nClassPixelNum;
	m_pStandardDifference = sqrtf(fTotal);//��ֽ�ķ���ֵ������ı�׼�
}

void ISODATA::Classfying()
{
	for (int i = 0; i < m_iLoopNum; i ++) ///����Ϊ����������a������������Ip ��NS�����㷨������
	{
		for (int i = 0; i < m_nTotalClassNum; i ++)
		{
			m_sample[i]->CalculateParameters();			//��������Ĳ������������ġ�����ƽ�����뼰����ƽ������
			if (m_sample[i] == NULL)
				continue;
			if(m_sample[i]->m_nClassPixelNum < m_fMinPixelNum)
			{///����TN�жϺϲ���������j��������nj< TN����ȡ�����������zj��Nc=Nc-1��ת�� ��
				DelFeatureClass(m_sample[i]);
				vector<Sample*>::iterator p = m_sample.begin();
				m_sample.erase(p + i);
			}

		}
		////b����Nc ��c/2����ת������һЩ����ѣ���
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
		////c����Nc ��2c����ת�����������Ѵ��������ϲ�������
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
		////d����c/2< Nc<2c������������Ip������ʱת�������Ѵ�������������Ip��ż��ʱת�����ϲ�������
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

//��Ҫ���кϲ�������֮�����С��������
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
			//�����������ĵľ���
			float fDis = ((m_sample[k]->m_pCenter) - m_sample[h]->m_pCenter)*(m_sample[k]->m_pCenter - m_sample[h]->m_pCenter);
			fDis = sqrtf(fDis);
			if (fDis < m_fMinClassDis)
			{
				CenterDistant clsDst;
				clsDst.fDis = fDis;//��¼������֮���
				clsDst.nNumClassA = k;//��¼�ϲ�����A
				clsDst.nNumClassB = h;//��¼�ϲ�����B
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
	//����ϲ���������ľ�ֵ
	pClass->m_pCenter = (pClass->m_nClassPixelNum*pClass->m_pCenter + pClassB->m_nClassPixelNum*pClassB->m_pCenter)/(pClass->m_nClassPixelNum+pClassB->m_nClassPixelNum);
	//��pClassB���е���������뵽pClass����
	for (int i = 0; i < pClassB->m_nClassPixelNum; i ++)
	{
		BYTE * pfV = pClassB->m_FeatureClassData[i];
		pClass->m_FeatureClassData.push_back(pfV);
		pClass->m_nClassPixelNum++;
	}
	m_nTotalClassNum --;
	pClassB = NULL;
}

//��������֮��ı�׼���������Ѹ���
void ISODATA::DivFeatureClass(Sample* pClass)
{
	if (pClass == NULL)
		return;
	float k = 0.5;				//���䷶Χ
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
		vector<BYTE*>::iterator p = pClass->m_FeatureClassData.begin(); //��������ʹ�ã�ǰ��ȡ���������ĵĲ�����ҲӦ��ʹ�ã�����δʹ��
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