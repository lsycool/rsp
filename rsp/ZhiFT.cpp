// ZhiFT.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rsp.h"
#include "ZhiFT.h"
#include "afxdialogex.h"
#include "rspDlg.h"
#include "ImageShow.h"

int a=0;//��������
int b=0;//���ݲ�������

unsigned long int HuiDu[256];//��¼�Ҷ�ֵ����
double TongJi[256];//ͳ��ÿ���Ҷȵ�Ƶ��

BYTE JunHen[256];
double JunH[256];
// ZhiFT �Ի���

IMPLEMENT_DYNAMIC(ZhiFT, CDialogEx)

ZhiFT::ZhiFT(CWnd* pParent /*=NULL*/)
	: CDialogEx(ZhiFT::IDD, pParent)
{

}

ZhiFT::~ZhiFT()
{
}

void ZhiFT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ZhiFT, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &ZhiFT::OnCbnSelchangeCombo1)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &ZhiFT::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ZhiFT::OnBnClickedCancel)
END_MESSAGE_MAP()


// ZhiFT ��Ϣ�������


BOOL ZhiFT::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
//	MoveWindow(700,15,100,100);
	if (path=="")
	{
		AfxMessageBox("Ӱ���ʼ��ʧ��");
		return FALSE;
	}
	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	ShowWindow(SW_SHOWNA);
//	Invalidate(TRUE);
//	UpdateWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


bool ZhiFT::ZhiFangTu(CDC* pDC)
{
	CRect m_viewRect;
	
	for (int i=0;i<256;i++)
	{
		HuiDu[i]=0;
		TongJi[i]=0;
		JunH[i]=0;
	
	}

	GDALDataset* poDataset;//GDAL���ݼ�
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(path,GA_ReadOnly);
	if(poDataset==NULL)
	{
		AfxMessageBox(_T("��ȡӰ����Ϣʧ�ܣ�"));
		return false;
	}


	//ң�е���������
	GDALRasterBand* Band1;
	int nBandCount=poDataset->GetRasterCount();

	//���ò���
	CString Item[7]={"Band1","Band2","Band3","Band4","Band5","Band6","Band7"};
	CComboBox* box=(CComboBox*)GetDlgItem(IDC_COMBO1);
	box->ResetContent();

	for (int i=0;i<nBandCount;i++)
	{
		box->AddString(Item[i]);
	}
		box->SetCurSel(a);

	Band1=poDataset->GetRasterBand(b+1);
	//���ͼ����ʾ���ڵĳߴ�
	GetClientRect(&m_viewRect);

	int nImageSizeX=poDataset->GetRasterXSize();
	int nImageSizeY=poDataset->GetRasterYSize();

	int nBufferSizeX,nBufferSizeY;
	nBufferSizeX=nImageSizeX;
	nBufferSizeY=nImageSizeY;

	//Ϊ���ݻ����������ڴ�
	BYTE* pafScanblock1,*TempLock1;
	pafScanblock1=(BYTE*)CPLMalloc((nScreenWidthZ)*(nScreenHeightZ));

	

	//��ȡ����
	Band1->RasterIO(GF_Read,0,0,nBufferSizeX,nBufferSizeY,pafScanblock1,nScreenWidthZ,nScreenHeightZ,GDT_Byte,0,0);
	TempLock1=pafScanblock1;

	BYTE PixMAX=0;//��¼�Ҷ�ͼ�����ص����ֵ
	BYTE PixMIN=255;//��¼�Ҷ�ͼ�����ص���Сֵ

	//����Li
	float Li[256];
	for (int i=0;i<256;i++)
	{
		Li[i]=i/255;
	}

	for (int i=0;i<nScreenHeightZ;i++)
	{
		for (int j=0;j<nScreenWidthZ;j++)
		{
			BYTE dn1=*pafScanblock1;
			BYTE a=0;
			int d=dn1-a;
			//�Ȼ��Ƶ��ڴ�DC�У�Ȼ���ڻ��Ƶ��豸DC��	
			HuiDu[d]++;//ͳ�ƻҶȸ���

			if (PixMAX<dn1)
			{
				PixMAX=dn1;
			}

			if (PixMIN>dn1)
			{
				PixMIN=dn1;
			}

			pafScanblock1++;		
		}
	}

	for (int i=0;i<256;i++)
	{
		TongJi[i]=HuiDu[i]/(nScreenWidthZ*nScreenHeightZ*1.0f); 
	}
	//��ȡ�������ֵ����С����ֵ
	unsigned int MAX=0;
	unsigned int MIN=255;
	for(int i=0;i<256;i++)
	{
		if (MAX<HuiDu[i])
		{
			MAX=HuiDu[i];
		}
		if(MIN>HuiDu[i])
		{
			MIN=HuiDu[i];
		}
	}


	CPen newpen(PS_SOLID,2,RGB(0,0,0));//�������ʣ�ʵ�ߣ��߿�2����ɫ��
	CPen* OldPen;
	OldPen=pDC->SelectObject(&newpen);


	//��X����
	pDC->MoveTo(10,300);
	pDC->LineTo(300,300);
	//(255,0)�̶�
	pDC->MoveTo(255,300);
	pDC->LineTo(255,295);
	//��200,0���̶�
	pDC->MoveTo(200,300);
	pDC->LineTo(200,295);
	//��150,0���̶�
	pDC->MoveTo(150,300);
	pDC->LineTo(150,295);
	//��100,0���̶�
	pDC->MoveTo(100,300);
	pDC->LineTo(100,295);
	//��50,0���̶�
	pDC->MoveTo(50,300);
	pDC->LineTo(50,295);

	//��ʾ����ֵ
	CStatic* sta1=(CStatic*)GetDlgItem(IDC_STATIC_X);
	sta1->SetWindowText("255");
	sta1->MoveWindow(255,302,30,30);

	sta1=(CStatic*)GetDlgItem(IDC_STATIC_X200);
	sta1->SetWindowText("200");
	sta1->MoveWindow(200,302,30,30);

	sta1=(CStatic*)GetDlgItem(IDC_STATIC_X150);
	sta1->SetWindowText("150");
	sta1->MoveWindow(150,302,30,30);

	sta1=(CStatic*)GetDlgItem(IDC_STATIC_X100);
	sta1->SetWindowText("100");
	sta1->MoveWindow(100,302,30,30);

	sta1=(CStatic*)GetDlgItem(IDC_STATIC_X50);
	sta1->SetWindowText("50");
	sta1->MoveWindow(50,302,30,30);
	//��y����
	pDC->MoveTo(10,300);
	pDC->LineTo(10,10);
	//��Y�̶�
	pDC->MoveTo(10,50);
	pDC->LineTo(15,50);

	pDC->MoveTo(10,175);
	pDC->LineTo(15,175);

	sta1=(CStatic*)GetDlgItem(IDC_STATIC_Y);
	sta1->SetWindowText("1");
	sta1->MoveWindow(20,50,30,30);

	sta1=(CStatic*)GetDlgItem(IDC_STATIC_Y05);
	sta1->SetWindowText("0.5");
	sta1->MoveWindow(20,175,30,30);
	//��X���ͷ
	pDC->MoveTo(300,300);
	pDC->LineTo(295,295);
	pDC->MoveTo(300,300);
	pDC->LineTo(295,305);
	//��Y���ͷ
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);
	//��ԭ������
	CStatic* sta=(CStatic*)GetDlgItem(IDC_STATIC_Origin);
	sta->SetWindowText("(0,0)");
	sta->MoveWindow(8,302,30,30);
	//��ʾ������С����ֵ
	CString Smax;
	CString Smin;
	Smax.Format("%d",MAX);
	Smin.Format("%d",MIN);
	sta=(CStatic*)GetDlgItem(IDC_STATIC_MAX);
	sta->SetWindowText(Smax);
	sta=(CStatic*)GetDlgItem(IDC_STATIC_MIN);
	sta->SetWindowText(Smin);
	//��ֱ��ͼ
	CPen pen1(PS_SOLID,1,RGB(255,0,0));
	pDC->SelectObject(&pen1);

	int a;
	for (int i=0;i<255;i++)
	{
		a=TongJi[i]*2500;
		pDC->MoveTo(i+10,300);
		pDC->LineTo(i+10,300-a);
	}

	for (int i=0;i<256;i++)
	{
		if (i==0)
		{
			JunH[i]=TongJi[i];
			JunHen[i]=JunH[i]*255.0f+0.5f;
		}
		else
		{
			JunH[i]=JunH[i-1]+TongJi[i];
			JunHen[i]=JunH[i]*255.0f+0.5f;
		}
	}

	pDC->SelectObject(&OldPen);

	ReleaseDC(pDC);
	GDALClose(poDataset);

	return true;
}


void ZhiFT::OnCbnSelchangeCombo1()
{
	CComboBox* box=(CComboBox*)GetDlgItem(IDC_COMBO1);
	a=box->GetCurSel();	
	box->SetCurSel(a);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void ZhiFT::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	ZhiFangTu(&dc);
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


void ZhiFT::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	b=a;
	Invalidate(TRUE);
	UpdateWindow();
	//CDialogEx::OnOK();
}


void ZhiFT::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DestroyWindow();
	//CDialogEx::OnCancel();
}
