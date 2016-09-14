// Similar.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rsp.h"
#include "Similar.h"
#include "afxdialogex.h"


// CSimilar �Ի���

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


// CSimilar ��Ϣ�������


void CSimilar::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	int num=1;//��¼�������ĸ���
	GDALDataset* poDataset;//GDAL���ݼ�
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(Similar_IN,GA_ReadOnly);

	if(poDataset==NULL)
	{
		AfxMessageBox(_T("���ļ�ʧ�ܣ�����"));
		return;
	}

	GDALRasterBand* Band1;
	int nImageSizeX=poDataset->GetRasterXSize();
	int nImageSizeY=poDataset->GetRasterYSize();

	BYTE *InData1 = (BYTE*)CPLMalloc((nImageSizeX)*(nImageSizeY));

	Band1=poDataset->GetRasterBand(1);
	Band1->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData1,nImageSizeX,nImageSizeY, GDT_Byte,0,0);

	//ȡӰ���м�����ص���Ϊ��һ����������
	center.data.push_back(InData1[int((nImageSizeX*nImageSizeY)/2)]);
	sample=new Sim_sample[nImageSizeX*nImageSizeY];

	BeginWaitCursor();

	for (int i=0;i<nImageSizeX*nImageSizeY;i++)//��ÿ��������ֵ
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
			int k=0;//�ж������������������ĵľ����Ƿ񶼴���T
			int m;//��¼���������±�
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
					m=j;//�������������С������ı��
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
	BYTE *OutData = new BYTE[nImageSizeX*nImageSizeY];//������
	for (int i=0;i<nImageSizeY;i++)
	{
		for (int j=0;j<nImageSizeX;j++)
		{
			OutData[i*nImageSizeY+j]=sample[i*nImageSizeY+j].mark*value;
		}
	}

	////////////////////////////////�����Ӱ��洢Ϊһ���ļ�//////////////////////////
	char* pszFormat;
	if(Similar_OUT.Right(4)==".BMP")
	{
		pszFormat = "BMP";
	}	

	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	GDALDataset *poDstDS;
	char **papszOptions =poDriver->GetMetadata();
	poDstDS = poDriver->Create( (char*)(LPCTSTR)Similar_OUT,//���·��
		nImageSizeX,nImageSizeY, //Ӱ�����ݿ��
		1, //���ȡ�Ĳ�����
		GDT_Byte, 	//��������������
		papszOptions );

	EndWaitCursor();

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
	CPLFree(InData1);
	delete []OutData;
	delete []sample;


	CDialogEx::OnOK();
}

//����·��
void CSimilar::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="�����������ң��Ӱ��";
	if (file.DoModal()==IDOK)
	{
		Similar_IN=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_SIMLIN,Similar_IN);
}

//���·��
void CSimilar::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(false, "BMP", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.BMP)|*.BMP|JPG�ļ�(*.JPG)|*.JPG;*.JPEG|tiff�ļ�(*.TIFF)|*.TIFF;*.TIF|img(*.IMG)|*.IMG|hdr(*.HDR)|*.HDR||");
	file.m_ofn.lpstrTitle="�����������";
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
