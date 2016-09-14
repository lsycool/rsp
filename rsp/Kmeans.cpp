// Kmeans.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rsp.h"
#include "Kmeans.h"
#include <math.h>
#include <afxwin.h>
#include "afxdialogex.h"

int l=1000;//����������
int M_num=2;//��ʼ��������

CString KOPath;//����Ӱ���ļ�·��
CString KRPath;//����ļ�·��
// Kmeans �Ի���

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


// Kmeans ��Ϣ�������


void Kmeans::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	l=m_DieD;//����������
	M_num=m_Kvalue;//��ʼ��������

	BeginWaitCursor();//��ʼ�ȴ����

	m_Progress->SetRange(0,100);

	GDALDataset* poDataset;//GDAL���ݼ�
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(KOPath,GA_ReadOnly);
	if(poDataset==NULL)
	{
		AfxMessageBox(_T("���ļ�ʧ�ܣ�����"));
		return;
	}

	//ң�е���������
	GDALRasterBand* Band1;
	GDALRasterBand* Band2;
	GDALRasterBand* Band3;

	int nImageSizeX=poDataset->GetRasterXSize();
	int nImageSizeY=poDataset->GetRasterYSize();
	int Bands = poDataset->GetRasterCount();

	m_Progress->SetPos(5);//���ý���������
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
	m_Progress->SetPos(50);//���ý���������

	VQ_CENTER* m_center=new VQ_CENTER[M_num];
	for (int i=0;i<M_num;i++)
	{
		m_center[i].Data=new BYTE[Ndimension];
	}
	LBGCluster(samp,nImageSizeX*nImageSizeY,m_center,M_num);

	BYTE *OutData = new BYTE[nImageSizeX*nImageSizeY*Ndimension];//������
	
	int ColorValue;
	if (M_num!=0)
	{
		ColorValue = 255/M_num;//��ÿһ���������ɫֵ
	}
	else
	{
		MessageBox("����������Ϊ�㣬���������룡����");
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

	////////////////////////////////�����Ӱ��洢Ϊһ���ļ�//////////////////////////
	char* pszFormat;
	if(KRPath.Right(4)==".BMP")
	{
		pszFormat = "BMP";
	}	

	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	GDALDataset *poDstDS;
	char **papszOptions =poDriver->GetMetadata();
	poDstDS = poDriver->Create( (char*)(LPCTSTR)KRPath,//���·��
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
	{
		m_Progress->SetPos(100);
		AfxMessageBox("������ɣ�");
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
/* ��ŷ�Ͼ���Ļ�������LBG�㷨��������������
������
x����������
n��������������
y��������
m���������ĸ���
����ֵ��
0����ȷ
1������                                                                     */
/************************************************************************/
int LBGCluster(VQ_VECTOR* X,int N,VQ_CENTER* Y,int M)
{
	//�ж��Ƿ���Ҫ����
	if (N<M)
	{
		return -1;
	}

	//l�ǵ������������������޸�
	int m=1,nCenter,i,j,k=0;
	int nDimension=X[0].nDimension;
	double D0,D;
	//����VQ_CENTERINFO�ṹ�������㷨�м���̵ı���
	struct VQ_CENTERINFO
	{
		double* Data;
		int nDimension;
		double* SumData;
		int Num;
	};
	VQ_CENTERINFO* Center=(VQ_CENTERINFO*)malloc(M*sizeof(VQ_CENTERINFO));
	//�ж��ڴ��Ƿ�����ɹ� 
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
			AfxMessageBox("�ڴ治��!");
			return -1;
		}

		//�ȼ����ʼ����������
		for(j=0;j<nDimension;j++)
		{
			Center[i].Data[j]=X[i*N/M].Data[j];
			Center[i].SumData[j]=0;
		}
		Center[i].Num=0;
	}
	D0=1;
	D=1e+10;
	//ѭ����1e-5�������ֵ�������޸�
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
					Dist+=(X[i].Data[k]-Center[j].Data[k]*(X[i].Data[k]-Center[j].Data[k]));//����ÿ�������������������ĵľ���
				}
				if (Dist<Distance[i])
				{
					nCenter=j;//��¼���
					Distance[i]=Dist;
				}
			}

			X[i].nCluster=nCenter;//ȷ��������������

			for (k=0;k<nDimension;k++)
			{
				Center[nCenter].SumData[k]+=X[i].Data[k];//����ÿ���������������ĺͣ���������ȷ��������
			}
			Center[nCenter].Num++;//��������Ԫ�ظ���
			D+=Distance[i];//����ÿ��Ԫ�ص���������С����ĺ�
		}

		for (i=0;i<M;i++)//M:�������ĸ���
		{
			if (Center[i].Num!=0)
			{
				for (k=0;k<nDimension;k++)
				{
					Center[i].Data[k]=Center[i].SumData[k]/Center[i].Num;//����ÿ����ľ�ֵ
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

				for (k=0;k<N;k++)//N:������������
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

//�����ļ�·��
void Kmeans::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="�����������ң��Ӱ��";
	if (file.DoModal()==IDOK)
	{
		KOPath=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_INK,KOPath);
}

//����ļ�·��
void Kmeans::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(false, "BMP", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.BMP)|*.BMP|JPG�ļ�(*.JPG)|*.JPG;*.JPEG|tiff�ļ�(*.TIFF)|*.TIFF;*.TIF|img(*.IMG)|*.IMG|hdr(*.HDR)|*.HDR||");
	file.m_ofn.lpstrTitle="�����������";
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_Progress=(CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	m_Progress->SendMessage(PBM_SETBARCOLOR,0,RGB(251,4,29));
	m_Progress->ModifyStyleEx(WS_EX_STATICEDGE,0);
	m_Progress->Invalidate(FALSE);

	m_DieD=30;
	m_Kvalue=20;
	UpdateData(FALSE);
	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
