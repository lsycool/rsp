// GCP.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rsp.h"
#include "GCP.h"
#include "afxdialogex.h"
#include <math.h>
#include "ProgressBar.h"


CString GCPFPATH;//��У���ļ�·��
CString GCPLPATH;//�ο�Ӱ���ļ�·��
CString GCPRPATH;//���·��
CString KZDP;//���Ƶ��ļ�·��

CPoint m_pointCursor(0,0);
int CCYIndex=0;//�ز���ѡ��
int alever=2;//����������
int iwidth;//Ӱ��1���
int iheight;//Ӱ��1�߶�

int iwidth2;//Ӱ��2���
int iheight2;//Ӱ��2�߶�

int numCount=0;//��¼���Ƶ������
int aflag=0;//���Ƶ�������־λ
// GCP �Ի���

IMPLEMENT_DYNAMIC(GCP, CDialogEx)

GCP::GCP(CWnd* pParent /*=NULL*/)
	: CDialogEx(GCP::IDD, pParent)
{
	m_fPara=new float[6];
	m_OutPara=new float[6];

	m_basex = 0.0f;
	m_basey = 0.0f;
	m_warpx = 0.0f;
	m_warpy = 0.0f;
	m_count = 0;
}

GCP::~GCP()
{
}

void GCP::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_GCPLIST, m_GCPList);
}


BEGIN_MESSAGE_MAP(GCP, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &GCP::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &GCP::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &GCP::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &GCP::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_RADIO1, &GCP::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &GCP::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &GCP::OnBnClickedRadio3)
	ON_BN_CLICKED(IDOK, &GCP::OnBnClickedOk)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_ADDGCP, &GCP::OnBnClickedButtonAddgcp)
	ON_BN_CLICKED(IDC_BUTTON6, &GCP::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON_AUTO, &GCP::OnBnClickedButtonAuto)
	ON_BN_CLICKED(IDCANCEL, &GCP::OnBnClickedCancel)
END_MESSAGE_MAP()


// GCP ��Ϣ�������


BOOL GCP::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	pic1=(CStatic*)GetDlgItem(IDC_PICTURE_ORIGIN);
	pic2=(CStatic*)GetDlgItem(IDC_PICTURE_BASE);

	m_GCPList=(CListCtrl*)GetDlgItem(IDC_GCPLIST);
	m_GCPList->InsertColumn(0,"���",LVCFMT_CENTER,105);
	m_GCPList->InsertColumn(1,"��׼�� X",LVCFMT_CENTER,105);
	m_GCPList->InsertColumn(2,"��׼�� Y",LVCFMT_CENTER,105);
	m_GCPList->InsertColumn(3,"У���� X",LVCFMT_CENTER,105);
	m_GCPList->InsertColumn(4,"У���� Y",LVCFMT_CENTER,105);
	m_GCPList->SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);   //�ı��б����

	m_progress=(CProgressCtrl*)GetDlgItem(IDC_PROGRESS_JZ);
	m_progress->SendMessage(PBM_SETBARCOLOR,0,RGB(250,30,30));
	m_progress->ModifyStyleEx(WS_EX_STATICEDGE,0);
	m_progress->Invalidate(FALSE);
	m_progress->SetRange(0,100);

	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//��ԭʼӰ��
void GCP::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="������仯ǰң��Ӱ��";
	if (file.DoModal()==IDOK)
	{
		GCPFPATH=file.GetPathName();
		CStatic* cs=(CStatic*)GetDlgItem(IDC_STATIC_JZY);
		cs->SetWindowText(GCPFPATH);

		poDataset=(GDALDataset*)GDALOpen(GCPFPATH,GA_ReadOnly);
		if(poDataset==NULL)
		{
			AfxMessageBox(_T("���ļ�ʧ�ܣ�����"));
			return;
		}
		CreatePyramids(GCPFPATH,poDataset);
		ReadImage(GCPFPATH,poDataset,pic1);
		
	}
}


//�򿪲ο�Ӱ��
void GCP::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="������仯ǰң��Ӱ��";
	if (file.DoModal()==IDOK)
	{
		GCPLPATH=file.GetPathName();
		CStatic* cs=(CStatic*)GetDlgItem(IDC_STATIC_JZC);
		cs->SetWindowText(GCPLPATH);

		poDataset2=(GDALDataset*)GDALOpen(GCPLPATH,GA_ReadOnly);
		if(poDataset2==NULL)
		{
			AfxMessageBox(_T("���ļ�ʧ�ܣ�����"));
			return;
		}
		CreatePyramids(GCPLPATH,poDataset2);
		ReadImage(GCPLPATH,poDataset2,pic2);
		
	}
}

//������Ӱ��
void GCP::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(false, "BMP", "У�����Ӱ��", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="�����ң��Ӱ��";
	if (file.DoModal()==IDOK)
	{
		GCPRPATH=file.GetPathName();
		CStatic* cs=(CStatic*)GetDlgItem(IDC_STATIC_JZR);
		cs->SetWindowText(GCPRPATH);
	}
}

//������Ƶ��ļ�
void GCP::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(true, "GCP", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|GCP(*.gcp)|*.gcp||");
	file.m_ofn.lpstrTitle="��������Ƶ��ļ�";
	if (file.DoModal()==IDOK)
	{
		KZDP=file.GetPathName();
		CStatic* cs=(CStatic*)GetDlgItem(IDC_STATIC_JZK);
		cs->SetWindowText(KZDP);
	}
	float fAdjustCoord[6];
	float fReferCoord[6];
	GetGCPFromFile((char*)(LPCTSTR)KZDP,fAdjustCoord,fReferCoord);//��ȡ���Ƶ��ļ�
	GetParaFromCoord(fAdjustCoord,fReferCoord,m_fPara);//���У��Ӱ�����ϵ��
	GetParaFromCoord(fReferCoord,fAdjustCoord,m_OutPara);//��òο�Ӱ�����ϵ��
	
}


bool GCP::ReadImage(CString file,GDALDataset* hDataset,CStatic* pic)
{
	//GDALAllRegister();
	//CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO"); 

	//poDataset=(GDALDataset*)GDALOpen(file,GA_ReadOnly);
	//if(poDataset==NULL)
	//{
	//	AfxMessageBox(_T("���ļ�ʧ�ܣ�����"));
	//	return false;
	//}
	//long nXSize=poDataset->GetRasterXSize();
	//long nYSize=poDataset->GetRasterYSize();

	//GDALRasterBand* Band1;
	//GDALRasterBand* Band2;
	//GDALRasterBand* Band3;

	//Band1=poDataset->GetRasterBand(1);
	//Band2=poDataset->GetRasterBand(2);
	//Band3=poDataset->GetRasterBand(3);

	////����ͼ���С
	//float fSize;
	//fSize=(nXSize/1024)*(nYSize/1024)*8;
	////��ý�����ѹ������
	//BYTE* pafScanblock1,*pafScanblock2,*pafScanblock3;
	//int iSmpStep=GetSmpStep(fSize);
	//if (iSmpStep==1)
	//{
	//	pafScanblock1=(BYTE*)CPLMalloc((nXSize)*(nYSize));
	//	pafScanblock2=(BYTE*)CPLMalloc((nXSize)*(nYSize));
	//	pafScanblock3=(BYTE*)CPLMalloc((nXSize)*(nYSize));

	//	int width=nXSize;
	//	int savewidth=width%4;
	//	if (savewidth!=0)
	//	{
	//		savewidth=4-savewidth;
	//	}
	//	savewidth=savewidth+width;
	//	iwidth=savewidth;
	//	iheight=nYSize;

	//	Band1->RasterIO(GF_Read,0,0,nXSize,nYSize,pafScanblock1,iwidth,iheight,GDT_Byte,0,0);
	//}
	////ͼ��ϴ�ʱ����ѹ�����ʽ�����Ӧ�Ľ�����
	//else
	//{
	//	int OverviewList[1]={iSmpStep};
	//	Band1->BuildOverviews("NEAREST",1,OverviewList,GDALDummyProgress,NULL);
	//	Band2->BuildOverviews("NEAREST",1,OverviewList,GDALDummyProgress,NULL);
	//	Band3->BuildOverviews("NEAREST",1,OverviewList,GDALDummyProgress,NULL);

	//	int Index=0;
	//	iSmpStep=4;
	//	nXSize/=iSmpStep;
	//	nYSize/=iSmpStep;


	//}

	GDALRasterBand* Band1;
	GDALRasterBand* Band2;
	GDALRasterBand* Band3;
	int bandCS=hDataset->GetRasterCount();
	if(bandCS>=3)
	{
		Band1=hDataset->GetRasterBand(1);
		Band2=hDataset->GetRasterBand(2);
		Band3=hDataset->GetRasterBand(3);
	}
	if(bandCS<3)
	{
		Band1=hDataset->GetRasterBand(1);
		Band2=hDataset->GetRasterBand(1);
		Band3=hDataset->GetRasterBand(1);
	}

	long sizeX=hDataset->GetRasterXSize();
	long sizeY=hDataset->GetRasterYSize();
	float size=(sizeX*sizeY*8)/1024/1024;
	if (size<=32)//��ͼ���СС��32M��ʱ��ֱ�Ӷ�ȡ�����򴴽�������
	{

		BYTE* pafScanblock1,*pafScanblock2,*pafScanblock3,*TempLock1,*TempLock2,*TempLock3;
		pafScanblock1=(BYTE*)CPLMalloc((sizeX)*(sizeY));
		pafScanblock2=(BYTE*)CPLMalloc((sizeX)*(sizeY));
		pafScanblock3=(BYTE*)CPLMalloc((sizeX)*(sizeY));
		TempLock1=pafScanblock1;
		TempLock2=pafScanblock2;
		TempLock3=pafScanblock3;

		Band1->RasterIO(GF_Read,0,0,sizeX,sizeY,pafScanblock1,sizeX,sizeY,GDT_Byte,0,0);
		Band2->RasterIO(GF_Read,0,0,sizeX,sizeY,pafScanblock2,sizeX,sizeY,GDT_Byte,0,0);
		Band3->RasterIO(GF_Read,0,0,sizeX,sizeY,pafScanblock3,sizeX,sizeY,GDT_Byte,0,0);

		CPaintDC pDC(pic);
		CDC dcMem;
		CDC* cdc=pic->GetDC();
		CBitmap* OldBitmap;
		CRect rect;
		pic->GetClientRect(&rect);
		dcMem.CreateCompatibleDC(&pDC);

		MemBitmap.CreateCompatibleBitmap(&pDC,sizeX,sizeY);
		OldBitmap= dcMem.SelectObject(&MemBitmap);
		for (int i=0;i<sizeY;i++)
		{
			for (int j=0;j<sizeX;j++)
			{
				BYTE dn1=*pafScanblock1;
				BYTE dn2=*pafScanblock2;
				BYTE dn3=*pafScanblock3;

				dcMem.SetPixel(j,i,RGB(dn1,dn2,dn3));

				pafScanblock1++;
				pafScanblock2++;
				pafScanblock3++;
			}
		}
		cdc->BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);

		CPLFree(TempLock1);
		CPLFree(TempLock2);
		CPLFree(TempLock3);

		MemBitmap.DeleteObject();
		dcMem.SelectObject(OldBitmap);
		dcMem.DeleteDC();
		ReleaseDC(&pDC);
		ReleaseDC(cdc);

		return true;
	}
	else
	{
		GDALRasterBand* poBand1;
		GDALRasterBand* poBand2;
		GDALRasterBand* poBand3;

		poBand1=Band1->GetOverview(alever);
		poBand2=Band2->GetOverview(alever);
		poBand3=Band3->GetOverview(alever);

		long nXSize1=poBand1->GetXSize();
		long nYSize1=poBand1->GetYSize();
		long nXSize2=poBand1->GetXSize();
		long nYSize2=poBand1->GetYSize();
		long nXSize3=poBand1->GetXSize();
		long nYSize3=poBand1->GetYSize();

		BYTE* pafScanblock1,*pafScanblock2,*pafScanblock3,*TempLock1,*TempLock2,*TempLock3;
		pafScanblock1=(BYTE*)CPLMalloc((nXSize1)*(nYSize1));
		pafScanblock2=(BYTE*)CPLMalloc((nXSize2)*(nYSize2));
		pafScanblock3=(BYTE*)CPLMalloc((nXSize3)*(nYSize3));
		TempLock1=pafScanblock1;
		TempLock2=pafScanblock2;
		TempLock3=pafScanblock3;

		poBand1->RasterIO(GF_Read,0,0,nXSize1,nYSize1,pafScanblock1,nXSize1,nYSize1,GDT_Byte,0,0);
		poBand2->RasterIO(GF_Read,0,0,nXSize2,nYSize2,pafScanblock2,nXSize2,nYSize2,GDT_Byte,0,0);
		poBand3->RasterIO(GF_Read,0,0,nXSize3,nYSize3,pafScanblock3,nXSize3,nYSize3,GDT_Byte,0,0);

		CPaintDC pDC(pic);
		CDC dcMem;
		CDC* cdc=pic->GetDC();
		CBitmap* OldBitmap;
		CRect rect;
		pic->GetClientRect(&rect);
		dcMem.CreateCompatibleDC(&pDC);

		MemBitmap.CreateCompatibleBitmap(&pDC,nXSize1,nYSize1);
		OldBitmap= dcMem.SelectObject(&MemBitmap);

		for (int i=0;i<nYSize1;i++)
		{
			for (int j=0;j<nXSize1;j++)
			{
				BYTE dn1=*pafScanblock1;
				BYTE dn2=*pafScanblock2;
				BYTE dn3=*pafScanblock3;

				dcMem.SetPixel(j,i,RGB(dn1,dn2,dn3));

				pafScanblock1++;
				pafScanblock2++;
				pafScanblock3++;
			}
		}
		cdc->BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);

		CPLFree(TempLock1);
		CPLFree(TempLock2);
		CPLFree(TempLock3);

		MemBitmap.DeleteObject();
		dcMem.SelectObject(OldBitmap);
		dcMem.DeleteDC();
		ReleaseDC(&pDC);
		ReleaseDC(cdc);
		return true;
	}
}


int GCP::GetSmpStep(float size)
{
	/*if (size<=32)
	{
	return 1;
	}
	else
	{
	return(int(sqrt(size/32)));
	}*/
	return 0;
}

//ǰ��ϵ���任
BOOL GCP::CalForwardParater(void)
{
	if (m_BPointList.GetCount()<3)
		return FALSE;

	float Bx1,By1,Bx2,By2,Bx3,By3;  //ԭͼ���ϵ����������ꣻ
	float Wx1,Wy1,Wx2,Wy2,Wx3,Wy3; // У����ͼ���ϵ����������ꣻ
	POSITION positon;
	POINT  point;

	positon=m_BPointList.GetHeadPosition();  //ֻȡǰ������
	point=m_BPointList.GetNext(positon);
	Bx1=(float)point.x;
	By1=(float)point.y;
	point=m_BPointList.GetNext(positon);
	Bx2=(float)point.x;
	By2=(float)point.y;
	point=m_BPointList.GetNext(positon);
	Bx3=(float)point.x;
	By3=(float)point.y;

	positon=m_WPointList.GetHeadPosition(); 
	point=m_WPointList.GetNext(positon);
	Wx1=(float)point.x;
	Wy1=(float)point.y;
	point=m_WPointList.GetNext(positon);
	Wx2=(float)point.x;
	Wy2=(float)point.y;
	point=m_WPointList.GetNext(positon);
	Wx3=(float)point.x;
	Wy3=(float)point.y;


	float D,Dx1,Dx2,Dx3,Dy1,Dy2,Dy3;

	D=Bx2*By3+Bx1*By2+Bx3*By1-By1*Bx2-By2*Bx3-By3*Bx1;
	if(D==0)
		return FALSE;

	Dx1=Wx1*Bx2*By3+Bx1*By2*Wx3+Wx2*Bx3*By1-By1*Bx2*Wx3-By2*Bx3*Wx1-Bx1*Wx2*By3;
	Dx2=Wx2*By3+Wx1*By2+Wx3*By1-By1*Wx2-By2*Wx3-By3*Wx1;
	Dx3=Bx2*Wx3+Bx1*Wx2+Bx3*Wx1-Wx1*Bx2-Wx2*Bx3-Wx3*Bx1;

	Dy1=Wy1*Bx2*By3+Bx1*By2*Wy3+Wy2*Bx3*By1-By1*Bx2*Wy3-By2*Bx3*Wy1-Bx1*Wy2*By3;
	Dy2=Wy2*By3+Wy1*By2+Wy3*By1-By1*Wy2-By2*Wy3-By3*Wy1;
	Dy3=Bx2*Wy3+Bx1*Wy2+Bx3*Wy1-Wy1*Bx2-Wy2*Bx3-Wy3*Bx1;

	a0=Dx1/D;
	a1=Dx2/D;
	a2=Dx3/D;
	b0=Dy1/D;
	b1=Dy2/D;
	b2=Dy3/D;
	return TRUE;
}

//����ϵ���任
BOOL GCP::CalBackwardParater(void)
{
	if(m_BPointList.GetCount()<3)
		return FALSE;

	float Bx1,By1,Bx2,By2,Bx3,By3;  //ԭͼ���ϵ����������ꣻ
	float Wx1,Wy1,Wx2,Wy2,Wx3,Wy3; // У����ͼ���ϵ����������ꣻ
	POSITION positon;
	POINT  point;

	positon=m_BPointList.GetHeadPosition();  //ֻȡǰ������
	point=m_BPointList.GetNext(positon);
	Bx1=(float)point.x;
	By1=(float)point.y;
	point=m_BPointList.GetNext(positon);
	Bx2=(float)point.x;
	By2=(float)point.y;
	point=m_BPointList.GetNext(positon);
	Bx3=(float)point.x;
	By3=(float)point.y;

	positon=m_WPointList.GetHeadPosition(); 
	point=m_WPointList.GetNext(positon);
	Wx1=(float)point.x;
	Wy1=(float)point.y;
	point=m_WPointList.GetNext(positon);
	Wx2=(float)point.x;
	Wy2=(float)point.y;
	point=m_WPointList.GetNext(positon);
	Wx3=(float)point.x;
	Wy3=(float)point.y;

	float D,Dx1,Dx2,Dx3,Dy1,Dy2,Dy3;

	D=Wx2*Wy3+Wx1*Wy2+Wx3*Wy1-Wy1*Wx2-Wy2*Wx3-Wy3*Wx1;
	if(D==0)
	{
		return FALSE;
	}
	//�������
	Dx1=Bx1*Wx2*Wy3+Wx1*Wy2*Bx3+Bx2*Wx3*Wy1-Wy1*Wx2*Bx3-Wy2*Wx3*Bx1-Wx1*Bx2*Wy3;
	Dx2=Bx2*Wy3+Bx1*Wy2+Bx3*Wy1-Wy1*Bx2-Wy2*Bx3-Wy3*Bx1;
	Dx3=Wx2*Bx3+Wx1*Bx2+Wx3*Bx1-Bx1*Wx2-Bx2*Wx3-Bx3*Wx1;

	Dy1=By1*Wx2*Wy3+Wx1*Wy2*By3+By2*Wx3*Wy1-Wy1*Wx2*By3-Wy2*Wx3*By1-Wx1*By2*Wy3;
	Dy2=By2*Wy3+By1*Wy2+By3*Wy1-Wy1*By2-Wy2*By3-Wy3*By1;
	Dy3=Wx2*By3+Wx1*By2+Wx3*By1-By1*Wx2-By2*Wx3-By3*Wx1;

	a0=Dx1/D;
	a1=Dx2/D;
	a2=Dx3/D;
	b0=Dy1/D;
	b1=Dy2/D;
	b2=Dy3/D;
	return TRUE;
}

//�������ͼ��ķ�Χ
void GCP::CalScale(void)
{
	int i;
	if(!CalForwardParater())
	{
		return;
	}
	float *ax=new float[m_dataWidth*2+m_dataHeight*2];//����x����
	float *ay=new float[m_dataWidth*2+m_dataHeight*2];//����y����

	for( i=0;i<m_dataWidth;i++)
	{
		ax[i]=a0+a1*i+a2*0;
		ax[i+m_dataWidth]=a0+a1*i+a2*(m_dataHeight-1);

		ay[i]=b0+b1*i+b2*0;
		ay[i+m_dataWidth]=b0+b1*i+b2*(m_dataHeight-1);

	}

	for (i=0;i<m_dataHeight;i++)
	{
		ax[m_dataWidth*2+i]=a0+a1*0+a2*i;
		ax[m_dataWidth*2+i+m_dataHeight]=a0+a1*(m_dataWidth-1)+a2*i;

		ay[m_dataWidth*2+i]=b0+b1*0+b2*i;
		ay[m_dataWidth*2+i+m_dataHeight]=b0+b1*(m_dataWidth-1)+b2*i;
	}

	xmin=ax[0];
	xmax=ax[0];
	ymin=ay[0];
	ymax=ay[0];
	for(i=1;i<m_dataWidth*2+m_dataHeight*2;i++)
	{
		if (xmin>ax[i]) xmin=ax[i];
		if (xmax<ax[i]) xmax=ax[i];
		if (ymin>ay[i]) ymin=ay[i];
		if (ymax<ay[i]) ymax=ay[i];
	}
	xmin=int(xmin);
	ymin=int(ymin);
	xmax=int(xmax);
	ymax=int(ymax);
	delete[]ax;
	delete[]ay;
}

//���ڽ��ز���
bool GCP::NearestSample(float * fPara , int* Range,int Width,int Hight,int Bands, BYTE* InData,BYTE* OutData)
{
	float x, y;
	int OutWidth = Range[2] - Range[0];
	int OutHight = Range[3] - Range[1];
	int InDataNum = Hight*Width;
	int OutBandNum = OutWidth*OutHight;	
	for (int k = 0;k<Bands;k++)
	{
		for (int i = 0; i < OutHight; i++)
		{
			for (int j = 0; j <OutWidth; j++)
			{
				x = j + Range[0];
				y = i + Range[1];
				int xIn=(int)(fPara[0]*x + fPara[1]*y +fPara[2] + 0.5);
				int yIn=(int)(fPara[3]*x + fPara[4]*y +fPara[5] + 0.5);
				if(xIn>=0&&xIn<=Width&&yIn>=0&&yIn<=Hight)
				{
					OutData[OutBandNum*k+i*OutWidth + j]=InData[InDataNum*k+xIn+yIn*Width];
				}
				else
					OutData[OutBandNum*k+i*OutWidth + j]=(BYTE)0;			
			}
		}                
	}

	return true;
}

//˫�����ز���
bool GCP::Bilinear(float* fPara, int* Range, int Width,int Hight,int Bands,BYTE* InData,BYTE* OutData)
{
	int LUrow, LUcol;
	int x, y;
	int OutWidth = Range[2] - Range[0];
	int OutHight = Range[3] - Range[1];
	int InDataNum = Hight*Width;
	int BandNum = OutWidth*OutHight;
	for (int k = 0;k<Bands;k++)
	{
		for (int i = 0; i < OutHight; i++)
		{
			for (int j = 0; j <OutWidth; j++)
			{
				x = j + Range[0];
				y = i + Range[1];
				float xIn=(fPara[0]*x + fPara[1]*y +fPara[2]);
				float yIn=(fPara[3]*x + fPara[4]*y +fPara[5]);
				LUcol = (int)xIn;//�����ڲ��õ�������ֵ
				LUrow = (int)yIn;
				float x_dis = xIn - LUcol;
				float y_dis = yIn - LUrow;
				if (LUrow < 0 || LUcol < 0 || LUrow >=Hight || LUcol >= Width)
				{
					OutData[BandNum*k+i*OutWidth + j] = 0;
				}
				else
				{
					int Value = InData[InDataNum*k+LUcol+LUrow*Width] *(1-x_dis)*(1-y_dis) + InData[InDataNum*k+LUrow*Width+LUcol+1] *(1-y_dis)*x_dis + InData[InDataNum*k+(LUrow+1)*Width+LUcol] *y_dis*(1-x_dis) + InData[InDataNum*k+(LUrow+1)*Width+LUcol+1] *x_dis*y_dis;
					OutData[BandNum*k+i*OutWidth + j] = (BYTE)Value;
				}
			}
		}                
	}

	return true;
}

//��������ز���
//����˵��buff:����Ӱ��pWDataBuff�����У����Ӱ��bands��������
void GCP::Convolution(BYTE*buff,BYTE*pWDataBuff,int m_imgewith,int m_imgeheight,int bands)
{
	delete[] pWDataBuff;
	m_dataWidth=m_imgewith ;
	m_dataHeight=m_imgeheight;
	CalScale();
//	CalBackwardParater();
	CalForwardParater();
	int i,j;
	//GDALRasterBand *pBand=NULL;
//	pBand=poDataset->GetRasterBand(1);
//	buff=new BYTE[m_dataWidth*m_dataHeight];
	//if (!pBand||!buff)
	//	return ;
	//pBand->RasterIO( GF_Read, m_dataXOff,m_dataYOff, m_dataWidth, m_dataHeight, buff, m_dataWidth,m_dataHeight, GDT_Byte, 0, 0 );

	float orgx,orgy;
	int x,y;  
	BYTE  v11,v12,v13,v14,v21,v22,v23,v24,v31,v32,v33,v34,v41,v42,v43,v44;  //�ڽ���16����Ԫֵ��
	float d11,d12,d13,d14,d21,d22,d23,d24,d31,d32,d33,d34,d41,d42,d43,d44; //������Ԫ��16������ľ����ƽ����
	float  w11,w12,w13,w14,w21,w22,w23,w24,w31,w32,w33,w34,w41,w42,w43,w44; //ÿ����Ԫ��Ȩֵ

	pWDataBuff=new BYTE[(int)((xmax-xmin+1)*(ymax-ymin+1)*bands)];//У�����ͼ��
	BYTE *pwtemp=pWDataBuff;
	for(int k=0;k<bands;k++)
	{
		for ( i=ymin;i<=ymax;i++)
		{
			for ( j=xmin;j<=xmax;j++)
			{   
				//У����������
				orgx=a0+a1*j+a2*i;
				orgy=b0+b1*j+b2*i;
				x=int(orgx);
				y=int(orgy);
				if (orgx<0||orgy<0||orgx>=m_dataWidth||orgy>=m_dataHeight)
					v11=v12=v13=v14=v21=v22=v23=v24=v31=v32=v33=v34=v41=v42=v43=v44=0;             
				else
				{

					int y1=y-1,y2=y+2,x1=x-1,x2=x+2;   //����߽�
					if(y1<0) y1=y;
					if(y2>=m_dataHeight) y2=y;
					if(x1<0) x1=x;
					if(x2>=m_dataWidth) x2=x;

					//��Χʮ�����������
					v11=buff[y1*m_dataWidth+x1];
					v12=buff[y1*m_dataWidth+x];
					v13=buff[y1*m_dataWidth+x+1];
					v14=buff[y1*m_dataWidth+x2];

					v21=buff[y*m_dataWidth+x1];
					v22=buff[y*m_dataWidth+x];
					v23=buff[y*m_dataWidth+x+1];
					v24=buff[y*m_dataWidth+x2];

					v31=buff[(y+1)*m_dataWidth+x1];
					v32=buff[(y+1)*m_dataWidth+x];
					v33=buff[(y+1)*m_dataWidth+x+1];
					v34=buff[(y+1)*m_dataWidth+x2];

					v41=buff[y2*m_dataWidth+x1];
					v42=buff[y2*m_dataWidth+x];
					v43=buff[y2*m_dataWidth+x+1];
					v44=buff[y2*m_dataWidth+x2];  
				}

				//��������ƽ��
				d11=(orgx-(x-1))*(orgx-(x-1))+(orgy-(y-1))*(orgy-(y-1));
				d12=(orgx-(x-0))*(orgx-(x-0))+(orgy-(y-1))*(orgy-(y-1));
				d13=(orgx-(x+1))*(orgx-(x+1))+(orgy-(y-1))*(orgy-(y-1));
				d14=(orgx-(x+2))*(orgx-(x+2))+(orgy-(y-1))*(orgy-(y-1));

				d21=(orgx-(x-1))*(orgx-(x-1))+(orgy-(y-0))*(orgy-(y-0));
				d22=(orgx-(x-0))*(orgx-(x-0))+(orgy-(y-0))*(orgy-(y-0));
				d23=(orgx-(x+1))*(orgx-(x+1))+(orgy-(y-0))*(orgy-(y-0));
				d24=(orgx-(x+2))*(orgx-(x+2))+(orgy-(y-0))*(orgy-(y-0));

				d31=(orgx-(x-1))*(orgx-(x-1))+(orgy-(y+1))*(orgy-(y+1));
				d32=(orgx-(x-0))*(orgx-(x-0))+(orgy-(y+1))*(orgy-(y+1));
				d33=(orgx-(x+1))*(orgx-(x+1))+(orgy-(y+1))*(orgy-(y+1));
				d34=(orgx-(x+2))*(orgx-(x+2))+(orgy-(y+1))*(orgy-(y+1));

				d41=(orgx-(x-1))*(orgx-(x-1))+(orgy-(y+2))*(orgy-(y+2));
				d42=(orgx-(x-0))*(orgx-(x-0))+(orgy-(y+2))*(orgy-(y+2));
				d43=(orgx-(x+1))*(orgx-(x+1))+(orgy-(y+2))*(orgy-(y+2));
				d44=(orgx-(x+2))*(orgx-(x+2))+(orgy-(y+2))*(orgy-(y+2));

				float sum;
				sum=1/d11+1/d12+1/d13+1/d14+1/d21+1/d22+1/d23+1/d24+1/d31+1/d32+1/d33+1/d34+1/d41+1/d42+1/d43+1/d44; //����Ȩֵ��
				w11=1/(d11*sum);
				w12=1/(d12*sum);
				w13=1/(d13*sum);
				w14=1/(d14*sum);
				w21=1/(d21*sum);
				w22=1/(d22*sum);
				w23=1/(d23*sum);
				w24=1/(d24*sum);
				w31=1/(d31*sum);
				w32=1/(d32*sum);
				w33=1/(d33*sum);
				w34=1/(d34*sum);
				w41=1/(d41*sum);
				w42=1/(d42*sum);
				w43=1/(d43*sum);
				w44=1/(d44*sum);
				//���������ز����������ֵ
				*pwtemp++=w11*v11+w12*v12+w13*v13+w14*v14+w21*v21+w22*v22+w23*v23+w24*v24+w31*v31+w32*v32+w33*v33+w34*v34+w41*v41+w42*v42+w43*v43+w44*v44;
			}
		}
	}
	delete[] buff;
	m_dataWidth=xmax-xmin+1;
	m_dataHeight=ymax-ymin+1;

	const char *pszFormat = "BMP";
	GDALDriver *poDriver;  
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	GDALDataset *poDstDS;
	char **papszOptions = NULL;
	if (GCPRPATH=="")
	{
		AfxMessageBox("��������Ӱ��·��");
		return;
	}
	poDstDS = poDriver->Create( (char*)(LPCTSTR)GCPRPATH,//���·��
		m_dataWidth,m_dataHeight, //Ӱ�����ݿ��
		bands, //���ȡ�Ĳ�����
		GDT_Byte, 	//��������������
		papszOptions );

	if(poDstDS->RasterIO(
		GF_Write,						//IO��ʽ
		0,0,							//Ӱ��������ʼλ��x,y
		m_dataWidth,m_dataHeight,				//Ӱ�����ݿ��
		pWDataBuff,						//ָ�򻺳�������ָ��
		m_dataWidth,m_dataHeight,				//���������ݿ��
		GDT_Byte,						//��������������
		bands,						//���ȡ�Ĳ�����
		NULL,							//���ȡ�Ĳ����б�
		0,								//����������һ������ռ�õ��ֽ���
		0,								//������һ��������ռ�õ��ֽ���
		0)==CE_Failure )				//���������ݴ洢��ʽ 0ΪRRRRGGGGBBBB-bsp 1ΪRGBRGBRGB-bip	
	{
		AfxMessageBox(_T("�����ļ�ʧ�ܣ�"));
		return;
	}
	else
	{
		AfxMessageBox("����У�����");
	}
	ReadImage(GCPRPATH,poDstDS,pic2);//��ʾУ�����Ӱ��
	GDALClose(poDstDS);
	delete[]pWDataBuff;
	return ;       
}

//���ڽ��ز���
void GCP::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCYIndex=0;
}

//˫�����ز���
void GCP::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCYIndex=1;
}


//��������ز���
void GCP::OnBnClickedRadio3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCYIndex=2;
}

//���������ļ���ȡ���ƿ��Ƶ�
//������GcpPath GCP�ļ�·��;fAdjustCoord��У��Ӱ���GCP����;fReferCoord �ο�Ӱ���GCP����
bool GCP::GetGCPFromFile(char* GcpPath, float* fAdjustCoord, float* fReferCoord)
{
	CRect rect1;//��¼ԭʼͼ�񴰿ڵķ�Χ
	pic1->GetWindowRect(&rect1);
	ScreenToClient(&rect1);

	CRect rect2;//��¼�ο�Ӱ�񴰿ڵķ�Χ
	pic2->GetWindowRect(&rect2);
	ScreenToClient(&rect2);

	CDC* pDC=this->GetDC();
	CPen pen(PS_SOLID,2,RGB(255,0,0));
	CPen*pOldpen=pDC->SelectObject(&pen);
	CPoint base1(122,3);
	CPoint base2(558,280);
	CPoint base3(303,261);
	CPoint base[3]={base1,base2,base3};

	CPoint warp1(1,2);
	CPoint warp2(606,2);
	CPoint warp3(324,159);
	CPoint warp[3]={warp1,warp2,warp3};

	FILE *file=NULL;
	file=fopen(GcpPath, "r" );
	if (file)
	{	
		float   fp;	
		fseek(file, 0, SEEK_SET );
		int index=0;
		for (int i=0;i<3;i++)
		{
			if(fscanf_s( file, "%f", &fp )<1)return false;		 
			fAdjustCoord[index  ]=fp;
			m_basex=fp;
			if(fscanf_s( file, "%f", &fp )<1)return false; 
			fAdjustCoord[index+1]=fp;
			m_basey=fp;
			if(fscanf_s( file, "%f", &fp )<1)return false; 
			fReferCoord[index  ]=fp;	
			m_warpx=fp;
			if(fscanf_s( file, "%f", &fp )<1)return false; 
			fReferCoord[index+1]=fp;
			m_warpy=fp;
			index=index+2;
			
//�����Ƶ�

			CPoint point1;
			point1.x=warp[i].x+rect1.left;
			point1.y=warp[i].y+rect1.top;
			CString countNum;
			countNum.Format("%d",i);

			pDC->MoveTo(point1.x-10,point1.y);
			pDC->LineTo(point1.x+10,point1.y);
			pDC->MoveTo(point1.x,point1.y-10);
			pDC->LineTo(point1.x,point1.y+10);
			pDC->TextOut(point1.x+10,point1.y,countNum);

			CPoint point2;
			point2.x=base[i].x+rect2.left;
			point2.y=base[i].y+rect2.top;

			pDC->MoveTo(point2.x-10,point2.y);
			pDC->LineTo(point2.x+10,point2.y);
			pDC->MoveTo(point2.x,point2.y-10);
			pDC->LineTo(point2.x,point2.y+10);
			pDC->TextOut(point2.x+10,point2.y,countNum);

//����list�б���
			CString count,basex,basey,warpx,warpy;
			count.Format("%d",m_count);
			basex.Format("%f",m_basex);
			basey.Format("%f",m_basey);
			warpx.Format("%f",m_warpx);
			warpy.Format("%f",m_warpy);

			m_GCPList->InsertItem(m_count,count);
			m_GCPList->SetItemText(m_count,1,basex);
			m_GCPList->SetItemText(m_count,2,basey);
			m_GCPList->SetItemText(m_count,3,warpx);
			m_GCPList->SetItemText(m_count,4,warpy);
//			m_GCPList->Invalidate(TRUE);
			m_count++;
		}
		pDC->SelectObject(pOldpen);
		ReleaseDC(pDC);
		fclose( file );
	}
	return true;
}

//������ͨ�����Ƶ㣬��ȡ����ϵ��(a1,b1,c1;a2,b2,c2)  u=a1*x + b1*y +c1
//������fAdjustCoord[6]У��Ӱ���3�����Ƶ������;fReferCoord[6]�ο�Ӱ���3�����Ƶ������;fPara[6]�洢����ϵ��(a1,b1,c1;a2,b2,c2)
bool GCP::GetParaFromCoord(float* fAdjustCoord, float* fReferCoord, float* fPara)
{
	//���Ƶ��Ӧ��ϵ
	float		x1_x2=fAdjustCoord[0]-fAdjustCoord[2];
	float 		x2_x3=fAdjustCoord[2]-fAdjustCoord[4];
	float 		y1_y2=fAdjustCoord[1]-fAdjustCoord[3];
	float 		y2_y3=fAdjustCoord[3]-fAdjustCoord[5];
	//����ϵ����Ӧ��ϵ
	float 	    u1_u2=fReferCoord[0]-fReferCoord[2];
	float 		u2_u3=fReferCoord[2]-fReferCoord[4];
	float 		v1_v2=fReferCoord[1]-fReferCoord[3];
	float 		v2_v3=fReferCoord[3]-fReferCoord[5];
	//�ж��Ƿ����
	if((y1_y2/x1_x2)==(y2_y3/x2_x3)||(v1_v2/u1_u2)==(v2_v3/u2_u3))
	{
		AfxMessageBox(_T("���Ƶ�λ��ͬһֱ�ߣ�"));
		return false;
	}
	//fPara[0] <---> a1
	fPara[0]=(u1_u2*y2_y3-u2_u3*y1_y2)/(x1_x2*y2_y3-x2_x3*y1_y2);
	//fPara[1] <---> b1
	fPara[1]=(u1_u2*x2_x3-u2_u3*x1_x2)/(y1_y2*x2_x3-y2_y3*x1_x2);
	//fPara[2] <---> c1   =u1- a1*x1- b1*y1  	
	fPara[2]=fReferCoord[0]-fPara[0]*fAdjustCoord[0]-fPara[1]*fAdjustCoord[1];

	//fPara[3] <---> a2
	fPara[3]=(v1_v2*y2_y3-v2_v3*y1_y2)/(x1_x2*y2_y3-x2_x3*y1_y2);
	//fPara[4] <---> b2
	fPara[4]=(v1_v2*x2_x3-v2_v3*x1_x2)/(y1_y2*x2_x3-y2_y3*x1_x2);
	//fPara[5] <---> c2   =v1- a2*x1 - b2*y1  	
	fPara[5]=fReferCoord[1]-fPara[3]*fAdjustCoord[0]-fPara[4]*fAdjustCoord[1];
	return true;
}

//�������õ����Ӱ��Χ��Χ
//����������Ӱ����Width��Hight;fPara������ϵ��;Range���Ӱ��Χ
bool GCP::GetRange(int Width, int Hight, float* fPara, int* Range)
{
	float xMin,yMin,xMax,yMax;
	xMin = yMin = xMax = yMax = 0;
	if (Hight > 0)
	{
		float x = 0, y = 0;
		for (int i = 1; i <= Width; i++)
		{
			x = fPara[1]  + fPara[0] * i + fPara[2] ;
			y = fPara[4]  + fPara[3]  * i +fPara[5] ;
			if (xMin > x)
				xMin = x;
			if (xMax < x)
				xMax = x;
			if (yMin > y)
				yMin = y;
			if (yMax < y)
				yMax = y;
		}
		for (int i = 1; i <= Width; i++)
		{
			x = fPara[1] *Hight + fPara[0] * i +fPara[2];
			y = fPara[4]*Hight  + fPara[3]  * i +fPara[5] ;
			if (xMin > x)
				xMin = x;
			if (xMax < x)
				xMax = x;
			if (yMin > y)
				yMin = y;
			if (yMax < y)
				yMax = y;
		}
		for (int i = 1; i <= Hight; i++)
		{
			x = fPara[1]*i  + fPara[0] + fPara[2] ;
			y = fPara[4]* i + fPara[3]  +fPara[5] ;
			if (xMin > x)
				xMin = x;
			if (xMax < x)
				xMax = x;
			if (yMin > y)
				yMin = y;
			if (yMax < y)
				yMax = y;
		}
		for (int i = 1; i <= Hight; i++)
		{
			x = fPara[1]*i  + fPara[0]*Width + fPara[2] ;
			y = fPara[4]* i + fPara[3]*Width  +fPara[5] ;
			if (xMin > x)
				xMin = x;
			if (xMax < x)
				xMax = x;
			if (yMin > y)
				yMin = y;
			if (yMax < y)
				yMax = y;
		}
	}
	Range[0] = (int)xMin;
	Range[1] = (int)yMin;
	Range[2] = (int)(xMax+0.5);
	Range[3] = (int)(yMax+0.5);	
	return true;
}


void GCP::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (GCPRPATH==""||GCPLPATH==""||GCPFPATH=="")
	{
		AfxMessageBox("������Ӱ��");
		return;
	}

	int  range[4];//����У����ķ�Χ
	int width = poDataset->GetRasterXSize();
	int hight = poDataset->GetRasterYSize();
	int Bands = poDataset->GetRasterCount();
	//��ʼ������Ӱ������
	BeginWaitCursor();
	BYTE *InData = new BYTE[width*hight*Bands];
	poDataset->RasterIO( GF_Read, 0, 0,width,hight, InData,width,hight, GDT_Byte,Bands, NULL,0,0,0 );
	GetRange(width,hight,m_fPara,range);//ͨ�������õ����Ƶ�
	/////���Ӱ��Ŀ��
	int OutWidth = range[2] - range[0];
	int OutHeight = range[3] - range[1];
	//��ʼ�����Ӱ������
	m_progress->SetPos(20);
	BYTE* OutData;
	OutData = new BYTE[OutWidth*OutHeight*Bands];
	m_progress->SetPos(40);
	if (CCYIndex==0)
	{//���ڽ��ز���
		NearestSample(m_OutPara,range,width,hight,Bands,InData,OutData);
	}
	if(CCYIndex==1)//˫�����ز���
	{
		Bilinear(m_OutPara,range,width,hight,Bands,InData,OutData);
	}
	if(CCYIndex==2)//��������ز���
	{
		Convolution(InData,OutData,width,hight,Bands);
	}
	m_progress->SetPos(80);
	////////////////////////////////�����Ӱ��洢Ϊһ���ļ�//////////////////////////
	if(CCYIndex!=2)
	{
		const char *pszFormat = "BMP";
		GDALDriver *poDriver;  
		poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
		GDALDataset *poDstDS;
		char **papszOptions = NULL;
		if (GCPRPATH=="")
		{
			AfxMessageBox("����������ļ�·��");
			return;
		}
		poDstDS = poDriver->Create( (char*)(LPCTSTR)GCPRPATH,//���·��
			OutWidth,OutHeight, //Ӱ�����ݿ��
			Bands, //���ȡ�Ĳ�����
			GDT_Byte, 	//��������������
			papszOptions );
		m_progress->SetPos(90);
		EndWaitCursor();
		if(poDstDS->RasterIO(
			GF_Write,						//IO��ʽ
			0,0,							//Ӱ��������ʼλ��x,y
			OutWidth,OutHeight,				//Ӱ�����ݿ��
			OutData,						//ָ�򻺳�������ָ��
			OutWidth,OutHeight,				//���������ݿ��
			GDT_Byte,						//��������������
			Bands,						//���ȡ�Ĳ�����
			NULL,							//���ȡ�Ĳ����б�
			0,								//����������һ������ռ�õ��ֽ���
			0,								//������һ��������ռ�õ��ֽ���
			0)==CE_Failure )				//���������ݴ洢��ʽ 0ΪRRRRGGGGBBBB-bsp 1ΪRGBRGBRGB-bip	
		{
			AfxMessageBox(_T("�����ļ�ʧ�ܣ�"));
			return;
		}
		else
		{
			AfxMessageBox("����У�����");
		}
		m_progress->SetPos(100);
//		ReadImage(GCPRPATH,poDstDS,pic2);//��ʾУ�����Ӱ��
		GDALClose(poDstDS);
		delete []OutData;
		delete []InData;
	}
//	OnOK();
//	CDialogEx::OnOK();
}

//�����������ļ�
bool GCP::CreatePyramids(CString pszFileName,GDALDataset* hDataset)
{
	long sizeX=hDataset->GetRasterXSize();
	long sizeY=hDataset->GetRasterYSize();

	float size=(sizeX/1024)*(sizeY/1024)*8;

	if (size<32)
	{
		return false;
	}
	else
	{
		m_progress->SetPos(10);
		//CPLSetConfigOption("USE_RRD","YES");    //����Erdas��ʽ�������ļ�  
	
		//hDataset=(GDALDataset*)GDALOpen(GCPFPATH,GA_ReadOnly);

		GDALDriverH hDriver = GDALGetDatasetDriver(hDataset);  
		const char* pszDriver = GDALGetDriverShortName(hDriver);  

		if( hDataset == NULL )  
		{  
			AfxMessageBox("�޷����ļ�");     
			return false;  
		}  

		if (EQUAL(pszDriver, "HFA") || EQUAL(pszDriver, "PCIDSK"))  
		{  
			GDALClose(hDataset);    //����ļ���Erdas��img����PCI��pix��ʽ�������ڽ������������Ĵ����������  
			hDataset=(GDALDataset*)GDALOpen(GCPFPATH,GA_Update); 
		}

		int iWidth = GDALGetRasterXSize(hDataset);  
		int iHeigh = GDALGetRasterYSize(hDataset);

		int iPixelNum = iWidth * iHeigh;    //ͼ���е�����Ԫ����
		int iTopNum = 4096;                 //�����������С��64*64  
		int iCurNum = iPixelNum / 4;
		int anLevels[1024] = { 0 };
		int nLevelCount = 0;                //����������

		m_progress->SetPos(30);
		do    //����������������ӵڶ���������  
		{  
			anLevels[nLevelCount] = static_cast<int>(pow(2.0, nLevelCount+2));  //���ڻ�����������֮���ת��
			nLevelCount ++;  
			iCurNum /= 4;  
		} while (iCurNum > iTopNum);  

		m_progress->SetPos(50);
		const char* pszResampling = "nearest"; //������ʽ 

		//���ý�����
		CProgressBar* pro=new CProgressBar;
		pro->Create(IDD_DIALOG_JDT);
		pro->SetStepCount(100);
		pro->ShowWindow(SW_SHOW);
		pro->SetMessage("���ڴ�������������");
		
		if (nLevelCount > 0 && GDALBuildOverviews( hDataset,pszResampling, nLevelCount, anLevels,0, NULL,ALGTermProgress,pro ) != CE_None )
		{	
			AfxMessageBox("����������ʧ��");
			return false;
		}
		m_progress->SetPos(80);
	//	GDALClose(hDataset);  
	//	GDALDestroyDriverManager();  
		m_progress->SetPos(100);
		delete pro;
		AfxMessageBox("�����������ɹ�");		
		return true;
	}
}


void GCP::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	CRect rect1;//��¼ԭʼͼ�񴰿ڵķ�Χ
	pic1->GetWindowRect(&rect1);
	ScreenToClient(&rect1);
	CRect rect2;//��¼�ο�Ӱ�񴰿ڵķ�Χ
	pic2->GetWindowRect(&rect2);
	ScreenToClient(&rect2);

	//�����ԭʼ����
	if (rect1.left<=m_pointCursor.x&&m_pointCursor.x<=rect1.right&&rect1.top<=m_pointCursor.y&&m_pointCursor.y<=rect1.bottom)
	{
		ReadImage(GCPFPATH,poDataset,pic1);
	}
	if (rect2.left<=m_pointCursor.x&&m_pointCursor.x<=rect2.right&&rect2.top<=m_pointCursor.y&&m_pointCursor.y<=rect2.bottom)
	{
		ReadImage(GCPLPATH,poDataset2,pic2);
	}

}


void GCP::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//CRect rect1;//��¼ԭʼͼ�񴰿ڵķ�Χ
	//pic1->GetWindowRect(&rect1);
	//ScreenToClient(&rect1);

	//CRect rect2;//��¼�ο�Ӱ�񴰿ڵķ�Χ
	//pic2->GetWindowRect(&rect2);
	//ScreenToClient(&rect2);
	//m_pointCursor=point;
	//if(alever>0)
	//{
	//	alever=alever-1;
	//}
	//if (rect1.left<=m_pointCursor.x&&m_pointCursor.x<=rect1.right&&rect1.top<=m_pointCursor.y&&m_pointCursor.y<=rect1.bottom)
	//{
	//	InvalidateRect(&rect1,TRUE);
	//}
	//if (rect2.left<=m_pointCursor.x&&m_pointCursor.x<=rect2.right&&rect2.top<=m_pointCursor.y&&m_pointCursor.y<=rect2.bottom)
	//{
	//	InvalidateRect(&rect2,TRUE);
	//}
	//UpdateWindow();
	CDialogEx::OnLButtonDown(nFlags, point);
}


void GCP::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect1;//��¼ԭʼͼ�񴰿ڵķ�Χ
	pic1->GetWindowRect(&rect1);
	ScreenToClient(&rect1);
	CRect rect2;//��¼�ο�Ӱ�񴰿ڵķ�Χ
	pic2->GetWindowRect(&rect2);
	ScreenToClient(&rect2);

	m_pointCursor=point;
	alever=alever+1;
	if (rect1.left<=m_pointCursor.x&&m_pointCursor.x<=rect1.right&&rect1.top<=m_pointCursor.y&&m_pointCursor.y<=rect1.bottom)
	{
		InvalidateRect(&rect1,TRUE);
		UpdateWindow();
	}
	if (rect2.left<=m_pointCursor.x&&m_pointCursor.x<=rect2.right&&rect2.top<=m_pointCursor.y&&m_pointCursor.y<=rect2.bottom)
	{
		InvalidateRect(&rect2,TRUE);
		UpdateWindow();
	}
	
	CDialogEx::OnRButtonDown(nFlags, point);
}

//�Ŵ�
void GCP::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect1;//��¼ԭʼͼ�񴰿ڵķ�Χ
	pic1->GetWindowRect(&rect1);
	ScreenToClient(&rect1);

	CRect rect2;//��¼�ο�Ӱ�񴰿ڵķ�Χ
	pic2->GetWindowRect(&rect2);
	ScreenToClient(&rect2);
	m_pointCursor=point;
	if(GCPFPATH!="")
	{
		if(alever>0)
		{
			alever=alever-1;
		}
		if (rect1.left<=m_pointCursor.x&&m_pointCursor.x<=rect1.right&&rect1.top<=m_pointCursor.y&&m_pointCursor.y<=rect1.bottom)
		{
			InvalidateRect(&rect1,TRUE);
			UpdateWindow();
		}
		if (rect2.left<=m_pointCursor.x&&m_pointCursor.x<=rect2.right&&rect2.top<=m_pointCursor.y&&m_pointCursor.y<=rect2.bottom)
		{
			InvalidateRect(&rect2,TRUE);
			UpdateWindow();
		}
	}
//	CDialogEx::OnLButtonDown(nFlags, point);

	CDialogEx::OnLButtonDblClk(nFlags, point);
}

//�����Ƶ��ʮ����
void GCP::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect1;//��¼ԭʼͼ�񴰿ڵķ�Χ
	pic1->GetWindowRect(&rect1);
	ScreenToClient(&rect1);

	CRect rect2;//��¼�ο�Ӱ�񴰿ڵķ�Χ
	pic2->GetWindowRect(&rect2);
	ScreenToClient(&rect2);
	m_pointCursor=point;
	if(GCPFPATH!=""&&GCPLPATH!="")
	{
	//��¼ԭʼӰ����Ƶ�
		if (rect1.left<=m_pointCursor.x&&m_pointCursor.x<=rect1.right&&rect1.top<=m_pointCursor.y&&m_pointCursor.y<=rect1.bottom)
		{
			aflag=aflag+1;
			CString num;
			num.Format("%d",numCount);
			CDC*pDC;
			pDC=this->GetDC();
			CPen pen(PS_SOLID,2,RGB(255,0,0));
			CPen*pOldpen=pDC->SelectObject(&pen);
		
			pDC->MoveTo(point.x-10,point.y);
			pDC->LineTo(point.x+10,point.y);
			pDC->MoveTo(point.x,point.y-10);
			pDC->LineTo(point.x,point.y+10);
			pDC->TextOut(point.x+10,point.y,num);
			pDC->SelectObject(pOldpen);

			m_warpx=point.x-rect1.left;
			m_warpy=point.y-rect1.top;
			ReleaseDC(pDC);
		}
	
	//��¼�ο�Ӱ����Ƶ�
		if (rect2.left<=m_pointCursor.x&&m_pointCursor.x<=rect2.right&&rect2.top<=m_pointCursor.y&&m_pointCursor.y<=rect2.bottom)
		{
			aflag=aflag+1;
			CString num;
			num.Format("%d",numCount);
			CDC*pDC;
			pDC=this->GetDC();
			CPen pen(PS_SOLID,2,RGB(255,0,0));
			CPen*pOldpen=pDC->SelectObject(&pen);
	
			pDC->MoveTo(point.x-10,point.y);
			pDC->LineTo(point.x+10,point.y);
			pDC->MoveTo(point.x,point.y-10);
			pDC->LineTo(point.x,point.y+10);
			pDC->TextOut(point.x+10,point.y,num);
			pDC->SelectObject(pOldpen);

			m_basex=point.x-rect2.left;
			m_basey=point.y-rect2.top;
			ReleaseDC(pDC);
		}
		if (aflag==2)
		{
			numCount++;
			aflag=0;
		}
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

//��ӿ��Ƶ�
void GCP::OnBnClickedButtonAddgcp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPoint POINT;
	POINT.x = (LONG)m_basex;
	POINT.y = (LONG)m_basey;
	m_BPointList.AddTail(POINT);//��Ӳο�Ӱ����Ƶ㵽������
	POINT.x = (LONG)m_warpx;
	POINT.y = (LONG)m_warpy;
	m_WPointList.AddTail(POINT);//���ԭʼӰ����Ƶ㵽������

	CString count,basex,basey,warpx,warpy;
	count.Format("%d",m_count);
	basex.Format("%f",m_basex);
	basey.Format("%f",m_basey);
	warpx.Format("%f",m_warpx);
	warpy.Format("%f",m_warpy);


	m_GCPList->InsertItem(m_count,count);
	m_GCPList->SetItemText(m_count,1,basex);
	m_GCPList->SetItemText(m_count,2,basey);
	m_GCPList->SetItemText(m_count,3,warpx);
	m_GCPList->SetItemText(m_count,4,warpy);
//	m_GCPList->Invalidate(TRUE);
	m_count++;
}

//ɾ�����Ƶ�
void GCP::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDC* pDC=this->GetDC();
	POSITION p;
	POSITION pos = m_GCPList->GetFirstSelectedItemPosition();//���ѡȡ�Ŀ��Ƶ�λ��
	if(!pos)
		return;
	int n=0;
	while (pos)
	{ 
		int nItem = m_GCPList->GetNextSelectedItem(pos);//�����һ�����Ƶ�����
		
		//��������ɾ��Ԫ��
		p=m_BPointList.FindIndex(nItem);
		m_BPointList.RemoveAt(p);        
		m_WPointList.FindIndex(nItem);
		m_WPointList.RemoveAt(p);

		nItem-=n;
		m_GCPList->DeleteItem(nItem);             //��list�ռ���ɾ����
		n++;
		m_count--;
	}
	CString s;
	for (int i=0;i<m_count;i++)   //�������к�
	{
		s.Format("%d",i);
		m_GCPList->SetItemText(i,0,s);
	}

	//�ػ�
	    p=m_WPointList.GetHeadPosition();
		int n1=0;
		CString c;
		CPoint point;
		CPen pen(PS_SOLID,2,RGB(0,0,255));
		CPen*pOldpen=pDC->SelectObject(&pen);  
		while(p)
		 {  
			  c.Format("%d",n1);
              point=m_WPointList.GetNext(p);  //Position++
			  pDC->MoveTo(point.x-10,point.y);
			  pDC->LineTo(point.x+10,point.y);
			  pDC->MoveTo(point.x,point.y-10);
			  pDC->LineTo(point.x,point.y+10);  
			  pDC->TextOut(point.x+10,point.y,c);
			  n1++;
		 }      
	    p=m_BPointList.GetHeadPosition();
		int n2=0;
		CString c1;
		while(p)
		{ 
			c1.Format("%d",n2);
			point=m_BPointList.GetNext(p);
			pDC->MoveTo(point.x-10,point.y);
			pDC->LineTo(point.x+10,point.y);
			pDC->MoveTo(point.x,point.y-10);
			pDC->LineTo(point.x,point.y+10);    
			pDC->TextOut(point.x+10,point.y,c1);
			n2++;
		}      
          pDC->SelectObject(pOldpen);
//	m_GCPList->Invalidate(TRUE);
}

//�Զ����ɿ��Ƶ�
void GCP::OnBnClickedButtonAuto()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!CalForwardParater())
	{
		return;
	}
	CString num;
	num.Format("%d",numCount);
	CRect rect1;//��¼ԭʼͼ�񴰿ڵķ�Χ
	pic1->GetWindowRect(&rect1);
	ScreenToClient(&rect1);

	float A0,A1,A2,B0,B1,B2;
	A0=a0;
	A1=a1;
	A2=a2;
	B0=b0;
	B1=b1;
	B2=b2;
	m_warpx=int(A0+A1*m_basex+A2*m_basey)+rect1.left;
	m_warpy=int(B0+B1*m_basex+B2*m_basey)+rect1.top;
	//�������ɿ��Ƶ�
	CDC*pDC;                           
	pDC=this->GetDC();
	CPen pen(PS_SOLID,2,RGB(0,0,255));
	CPen*pOldpen=pDC->SelectObject(&pen);
	pDC->MoveTo(m_warpx-10.0,m_warpy);
	pDC->LineTo(m_warpx+10.0,m_warpy);
	pDC->MoveTo(m_warpx,m_warpy-10.0);
	pDC->LineTo(m_warpx,m_warpy+10.0);
	pDC->TextOut(m_warpx+10.0,m_warpy,num);
//	pDC->TextOut(m_basex+10.0,m_basey,num);
	pDC->SelectObject(pOldpen);
	aflag=0;
	numCount++;
	ReleaseDC(pDC);

	//CString count,basex,basey,warpx,warpy;
	//count.Format("%d",m_count);
	//basex.Format("%f",m_basex);
	//basey.Format("%f",m_basey);
	//warpx.Format("%f",m_warpx);
	//warpy.Format("%f",m_warpy);


	//m_GCPList->InsertItem(m_count,count);
	//m_GCPList->SetItemText(m_count,1,basex);
	//m_GCPList->SetItemText(m_count,2,basey);
	//m_GCPList->SetItemText(m_count,3,warpx);
	//m_GCPList->SetItemText(m_count,4,warpy);
	//m_GCPList->Invalidate(TRUE);
	//m_count++;
}


void GCP::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DestroyWindow();
//	CDialogEx::OnCancel();
}
