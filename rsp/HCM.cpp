// HCM.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rsp.h"
#include "HCM.h"
#include "afxdialogex.h"
#include <math.h>

CString hcm_in;
CString hcm_out;
// HCM �Ի���

IMPLEMENT_DYNAMIC(HCM, CDialogEx)

HCM::HCM(CWnd* pParent /*=NULL*/)
	: CDialogEx(HCM::IDD, pParent)
{

	EnableAutomation();

}

HCM::~HCM()
{
}

void HCM::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease�����ཫ�Զ�
	// ɾ���ö����ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CDialogEx::OnFinalRelease();
}

void HCM::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(HCM, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &HCM::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &HCM::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &HCM::OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(HCM, CDialogEx)
END_DISPATCH_MAP()

// ע��: ������� IID_IHCM ֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {EED3DF14-C5F5-4425-871E-0F4089A0DF65}
static const IID IID_IHCM =
{ 0xEED3DF14, 0xC5F5, 0x4425, { 0x87, 0x1E, 0xF, 0x40, 0x89, 0xA0, 0xDF, 0x65 } };

BEGIN_INTERFACE_MAP(HCM, CDialogEx)
	INTERFACE_PART(HCM, IID_IHCM, Dispatch)
END_INTERFACE_MAP()


// HCM ��Ϣ�������


void HCM::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="�����������ң��Ӱ��";
	if (file.DoModal()==IDOK)
	{
		hcm_in=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_HCMIN,hcm_in);
}


BOOL HCM::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_progress=(CProgressCtrl*)GetDlgItem(IDC_PROGRESS_HCM);
	m_progress->SendMessage(PBM_SETBARCOLOR,0,RGB(251,4,29));
	m_progress->ModifyStyleEx(WS_EX_STATICEDGE,0);
	m_progress->Invalidate(FALSE);

	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void HCM::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(false, "BMP", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.BMP)|*.BMP|JPG�ļ�(*.JPG)|*.JPG;*.JPEG|tiff�ļ�(*.TIFF)|*.TIFF;*.TIF|img(*.IMG)|*.IMG|hdr(*.HDR)|*.HDR||");
	file.m_ofn.lpstrTitle="�����������";
	if (file.DoModal()==IDOK)
	{
		hcm_out=file.GetPathName();
		switch(file.m_ofn.nFilterIndex)
		{
		case 2:
			{
				if (hcm_out.Right(4)!=".BMP")
				{
					hcm_out=hcm_out+".BMP";
				}
				break;
			}
		case 4:
			{
				if (hcm_out.Right(5)!=".TIFF")
				{
					hcm_out=hcm_out+".TIFF";
				}
				break;
			}

		}

	}
	SetDlgItemText(IDC_STATIC_HCMOUT,hcm_out);
}


void HCM::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Hsamp* hs;//�洢������
	Hclass* hc;//�洢ÿ����Ԫ����

	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
	poDataset=(GDALDataset*)GDALOpen(hcm_in,GA_ReadOnly);

	if(poDataset==NULL)
	{
		AfxMessageBox(_T("���ļ�ʧ�ܣ�����"));
		return;
	}

	m_progress->SetRange(0,100);

	GDALRasterBand* Band1;
	//GDALRasterBand* Band2;
	//GDALRasterBand* Band3;

//	int BandCount=poDataset->GetRasterCount();

	int nImageSizeX=poDataset->GetRasterXSize();
	int nImageSizeY=poDataset->GetRasterYSize();

	BYTE *InData1 = new BYTE[nImageSizeX*nImageSizeY];
	/*BYTE *InData2 = new BYTE[nImageSizeX*nImageSizeY];
	BYTE *InData3 = new BYTE[nImageSizeX*nImageSizeY];*/


	/*if (BandCount==1)
	{*/
	Band1=poDataset->GetRasterBand(1);
	Band1->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData1,nImageSizeX,nImageSizeY, GDT_Byte,0,0);
	//}
	//else
	//{
	//	Band1=poDataset->GetRasterBand(1);
	//	/*Band2=poDataset->GetRasterBand(2);
	//	Band3=poDataset->GetRasterBand(3);*/

	//	Band1->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData1,nImageSizeX,nImageSizeY, GDT_Byte,0,0);
	//	/*Band2->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData2,nImageSizeX,nImageSizeY, GDT_Byte,0,0);
	//	Band3->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData3,nImageSizeX,nImageSizeY, GDT_Byte,0,0);*/
	//}
	hc=new Hclass[nImageSizeX*nImageSizeY];
	hs=new Hsamp[nImageSizeX*nImageSizeY];

	for (int i=0;i<nImageSizeX*nImageSizeY;i++)
	{
//		hc[i].ndimension=BandCount;
		//for (int j=0;j<BandCount;j++)
		//{
		//	hc[i].data=new BYTE[BandCount];
		//	if (j==0)
		//	{
		//		hc[i].data[j]=InData1[i];
		//	}
		//	if (j==1)
		//	{
		//		hc[i].data[j]=InData2[i];
		//	}
		//	if (j==2)
		//	{
		//		hc[i].data[j]=InData3[i];
		//	}
		//}
		hc[i].data=InData1[i];
	}

	BeginWaitCursor();

	m_progress->SetPos(10);

	hcmCluster(hc,nImageSizeX*nImageSizeY,hs);

	BYTE *OutData = new BYTE[nImageSizeX*nImageSizeY];//������
	for (int i=0;i<nImageSizeY;i++)
	{
		for (int j=0;j<nImageSizeX;j++)
		{
			/*if (hc[i*nImageSizeX + j].mark==0)
			{
				OutData[i*nImageSizeX + j]=0;
			}
			if (hc[i*nImageSizeX + j].mark==1)
			{
				OutData[i*nImageSizeX + j]=255;
			}
			else
			{
				OutData[i*nImageSizeX + j]=100;
			}*/
			OutData[i*nImageSizeX + j]=hc[i*nImageSizeX + j].mark*255;
		}
	}

	m_progress->SetPos(90);
	////////////////////////////////�����Ӱ��洢Ϊһ���ļ�//////////////////////////
	char* pszFormat;
	if(hcm_out.Right(4)==".BMP")
	{
		pszFormat = "BMP";
	}	

	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	GDALDataset *poDstDS;
	char **papszOptions =poDriver->GetMetadata();
	poDstDS = poDriver->Create( (char*)(LPCTSTR)hcm_out,//���·��
		nImageSizeX,nImageSizeY, //Ӱ�����ݿ��
		1, //���ȡ�Ĳ�����
		GDT_Byte, 	//��������������
		papszOptions );

	EndWaitCursor();

	m_progress->SetPos(100);

	if(poDstDS->RasterIO(
		GF_Write,						//IO��ʽ
		0,0,							//Ӱ��������ʼλ��x,y
		nImageSizeX,nImageSizeY,				//Ӱ�����ݿ��
		OutData,						//ָ�򻺳�������ָ��
		nImageSizeX,nImageSizeY,				//���������ݿ��
		GDT_Byte,						//��������������
		1,						//���ȡ�Ĳ�����
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
		AfxMessageBox("������ɣ�");
	}

	GDALClose(poDstDS);
	delete []InData1;
	/*delete []InData2;
	delete []InData3;*/
	delete []OutData;
	delete []hc;
	delete []hs;
	CDialogEx::OnOK();
}

//num:��������
void HCM::hcmCluster(Hclass* hc,int num,Hsamp* hs)
{
	int classnum=num;//����

	for (int i=0;i<classnum;i++)
	{
		hs[i].sample.push_back(&hc[i]);//�����е�Ԫ�ض����Է�Ϊһ��
	}

	m_progress->SetPos(20);

	while(classnum>2)
	{
	//	m_progress->SetPos((20+80/(num-3)*(num-classnum)));
		//for (int i=0;i<classnum;i++ )
		//{
		//	for(int j=0;j<classnum;j++)
		//	{

		//		for (int n=0;n<nD;n++)
		//		{
		//			Dist[i][j]+=(hc[i].data[nD]-hc[j].data[nD])*(hc[i].data[nD]-hc[j].data[nD]);//����ŷ�Ͼ������������
		//		}
		//		Dist[i][j]=sqrt(Dist[i][j]);

		//		//��������о�����Сֵ
		//		if (min>Dist[i][j])
		//		{
		//			min=Dist[i][j];
		//			markI=i;
		//			markJ=j;
		//		}
		//	}
		//}
		int markI=0,markJ=0;
		int min=32767;	

		for (int i=0;i<classnum;i++)
		{
			int leiA=hs[i].sample.size();

			for (int j=i+1;j<classnum;j++)
			{
				int leiB=hs[j].sample.size();

				double mintemp=3276700000;

				for(int k=0;k<leiA;k++)
				{
					for(int z=0;z<leiB;z++)
					{
						//for (int n=0;n<nD;n++)
						//{
						//	double a=((hs[i].sample[k]->data[n]-hs[j].sample[z]->data[n])*(hs[i].sample[k]->data[n]-hs[j].sample[z]->data[n]));//����������֮��ľ���
						//	a=sqrt(a);
						//	if (mintemp>a)
						//	{
						//		mintemp=a;//ȡ�����������С����
						//	}
						//} 
						double a=((hs[i].sample[k]->data-hs[j].sample[z]->data)*(hs[i].sample[k]->data-hs[j].sample[z]->data));//����������֮��ľ���
					
						a=sqrt(a);

						if (mintemp>a)
						{
							mintemp=a;//ȡ�����������С����
						}


						if (min>mintemp)
						{
							min=mintemp;//ȡ������������е���С����
							markI=i;//ȡ����������С������������
							markJ=j;
						}
					}
				}
			}
			if (markI>markJ)
			{
				int temp=markI;
				markI=markJ;
				markJ=temp;
			}
		}

		for (int m=0;m<hs[markJ].sample.size();m++)
		{
			hs[markI].sample.push_back(hs[markJ].sample[m]);
		}

		hs[markJ].sample.clear();

		for (int i=markJ;i<classnum-1;i++)
		{
			hs[i]=hs[i+1];//����
		}
		hs[classnum-1].sample.clear();

		classnum=classnum-1;
	};

  	for (int i=0;i<classnum;i++)
	{
		int n=hs[i].sample.size();
		for (int k=0;k<n;k++)
		{
			hs[i].sample[k]->mark=i;
		}
	}

}
