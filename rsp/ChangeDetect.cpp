// ChangeDetect.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rsp.h"
#include "ChangeDetect.h"
#include "afxdialogex.h"
#include "rspDlg.h"
#include "ImageShow.h"

CString FPATH1;//�仯ǰ�ļ�·��
CString LPATH1;//�仯���ļ�·��
CString RPATH;//���·��
// ChangeDetect �Ի���

IMPLEMENT_DYNAMIC(ChangeDetect, CDialogEx)

ChangeDetect::ChangeDetect(CWnd* pParent /*=NULL*/)
	: CDialogEx(ChangeDetect::IDD, pParent)
{

}

ChangeDetect::~ChangeDetect()
{
}

void ChangeDetect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ChangeDetect, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_FORMER, &ChangeDetect::OnBnClickedButtonFormer)
	ON_BN_CLICKED(IDC_BUTTON_LATTER, &ChangeDetect::OnBnClickedButtonLatter)
	ON_BN_CLICKED(IDC_BUTTON_RESULT, &ChangeDetect::OnBnClickedButtonResult)
	ON_BN_CLICKED(IDOK, &ChangeDetect::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ChangeDetect::OnBnClickedCancel)
END_MESSAGE_MAP()


// ChangeDetect ��Ϣ�������


BOOL ChangeDetect::OnInitDialog()
{
	CDialogEx::OnInitDialog();
//	ShowWindow(SW_SHOW);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



void ChangeDetect::OnBnClickedButtonFormer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="������仯ǰң��Ӱ��";
	if (file.DoModal()==IDOK)
	{
		FPATH1=file.GetPathName();
		CStatic* cs=(CStatic*)GetDlgItem(IDC_STATIC_FORMER);
		cs->SetWindowText(FPATH1);
	}
}


void ChangeDetect::OnBnClickedButtonLatter()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="������仯��ң��Ӱ��";
	if (file.DoModal()==IDOK)
	{
		LPATH1=file.GetPathName();
		CStatic* cs=(CStatic*)GetDlgItem(IDC_STATIC_LATTER);
		cs->SetWindowText(LPATH1);
	}
}


void ChangeDetect::OnBnClickedButtonResult()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(false, "BMP", "�����", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.BMP)|*.BMP|JPG�ļ�(*.JPG)|*.JPG;*.JPEG|tiff�ļ�(*.TIFF)|*.TIFF;*.TIF|img(*.IMG)|*.IMG|hdr(*.HDR)|*.HDR||");
	file.m_ofn.lpstrTitle="��������Ӱ��";
	if (file.DoModal()==IDOK)
	{
		RPATH=file.GetPathName();
		CStatic* cs=(CStatic*)GetDlgItem(IDC_STATIC_RESULT);
		cs->SetWindowText(RPATH);
	}
}


void ChangeDetect::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BeginWaitCursor();//��ʼ�ȴ����

	GDALDataset* poDataset;//GDAL���ݼ�
	GDALDataset* poDataset1;//GDAL���ݼ�
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(FPATH1,GA_ReadOnly);
	poDataset1=(GDALDataset*)GDALOpen(LPATH1,GA_ReadOnly);

	if(poDataset==NULL||poDataset1==NULL)
	{
		AfxMessageBox(_T("���ļ�ʧ�ܣ�����"));
		return;
	}

	int nImageSizeX=poDataset->GetRasterXSize();
	int nImageSizeY=poDataset->GetRasterYSize();
	int Bands = poDataset->GetRasterCount();

	int nImageSizeX1=poDataset1->GetRasterXSize();
	int nImageSizeY1=poDataset1->GetRasterYSize();
	int Bands1 = poDataset1->GetRasterCount();

	BYTE *InData = new BYTE[nImageSizeX*nImageSizeY*Bands];
	poDataset->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData,nImageSizeX,nImageSizeY, GDT_Byte,Bands, NULL,0,0,0 );

	BYTE *InData1 = new BYTE[nImageSizeX1*nImageSizeY1*Bands1];
	poDataset1->RasterIO( GF_Read, 0, 0,nImageSizeX1,nImageSizeY1, InData1,nImageSizeX1,nImageSizeY1, GDT_Byte,Bands, NULL,0,0,0 );

	BYTE *OutData = new BYTE[nImageSizeX*nImageSizeY*3];//������
	for (int iBands = 0;iBands<1;iBands++)
	{
		for (int i = 0;i<nImageSizeY;i++)
		{
			for (int j = 0;j<nImageSizeX;j++)
			{
				if (InData[nImageSizeY*nImageSizeX*iBands+i*nImageSizeX + j]-InData1[nImageSizeY*nImageSizeX*iBands+i*nImageSizeX + j]!=0)
				{
					OutData[nImageSizeY*nImageSizeX*iBands+i*nImageSizeX + j]=255;
				}
				else
				{
					OutData[nImageSizeY*nImageSizeX*iBands+i*nImageSizeX + j]=0;
				}
			}
		}
	}

	char* pszFormat;
	if(RPATH.Right(4)==".BMP")
	{
		pszFormat = "BMP";
	}	

	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	GDALDataset *poDstDS;
	char **papszOptions =poDriver->GetMetadata();
	poDstDS = poDriver->Create( (char*)(LPCTSTR)RPATH,//���·��
		nImageSizeX,nImageSizeY, //Ӱ�����ݿ��
		3, //���ȡ�Ĳ�����
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
		AfxMessageBox("�仯���ɹ���");
	GDALClose(poDstDS);
	delete []OutData;
	delete []InData;
	delete []InData1;

	path=RPATH;
	ImageShow* image=new ImageShow;
	image->Create(IDD_IMAGE_SHOW);
	image->ShowWindow(SW_SHOW);
	CDialogEx::OnOK();
}


void ChangeDetect::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DestroyWindow();
//	CDialogEx::OnCancel();
}
