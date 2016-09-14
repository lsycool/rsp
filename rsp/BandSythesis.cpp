// BandSythesis.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "rsp.h"
#include "BandSythesis.h"
#include "afxdialogex.h"
#include "rspDlg.h"

CString pathname;
int nBandCount;
int i=0;//�������Ʋ��κϳɲ���
int k=0;//������¼Listbox�ؼ���ʾ
int index;//��¼listbox������Ϣ
int bn[3];
int checked=1;
int BN[3]={3,2,1};
int a[3]={IDC_RADIO_R,IDC_RADIO_GR,IDC_RADIO_B};
int b[3]={IDC_STATIC_RED,IDC_STATIC_GREEN,IDC_STATIC_BLUE};
// BandSythesis �Ի���

IMPLEMENT_DYNAMIC(BandSythesis, CDialogEx)

BandSythesis::BandSythesis(CWnd* pParent /*=NULL*/)
	: CDialogEx(BandSythesis::IDD, pParent)
{
	
}

BandSythesis::~BandSythesis()
{
}

void BandSythesis::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(BandSythesis, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST_BAND, &BandSythesis::OnLbnSelchangeListBand)
	ON_BN_CLICKED(IDOK, &BandSythesis::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &BandSythesis::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO1, &BandSythesis::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &BandSythesis::OnBnClickedRadio2)
END_MESSAGE_MAP()


// BandSythesis ��Ϣ�������


BOOL BandSythesis::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString filepath=path;
	int BandCount=nBandCount;
	if (filepath=="")
	{
		AfxMessageBox("��������ң��Ӱ��");
		return FALSE;
	}
	else
	{
	CString Item[7]={"Band1--"+filepath,"Band2--"+filepath,"Band3--"+filepath,"Band4--"+filepath,"Band5--"+filepath,"Band6--"+filepath,"band7--"+filepath};//ѡ������
	ListBox = (CListBox *)GetDlgItem(IDC_LIST_BAND);
	for (int i=0;i<BandCount;i++)
	{
		ListBox->AddString(Item[i]);
	}
	//ListBox->SetCurSel(0);

	CButton* rgB=(CButton*)GetDlgItem(IDC_RADIO1);
	rgB->SetCheck(TRUE);

	CRect rect;
	GetClientRect(&rect);
	MoveWindow(800,75,rect.Width(),rect.Height(),false);

	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	ShowWindow(SW_SHOWNA);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	}
	// �쳣: OCX ����ҳӦ���� FALSE
}


void BandSythesis::OnLbnSelchangeListBand()
{
		// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString T[7]={"band1","band2","band3","band4","band5","band6","band7"};
	index=ListBox->GetCurSel();
	if(checked==1)
	{
	do
	{
		if (k>=1)
		{
			CButton* rgC=(CButton*)GetDlgItem(a[k-1]);
			rgC->SetCheck(FALSE);
		}

		if (k>=3)
		{
			k=0;
		}

		bn[k]=index;

		this->SetDlgItemText(b[k],	T[index]);
		CButton* rgB=(CButton*)GetDlgItem(a[k]);
		rgB->SetCheck(TRUE);

		k++;

		}while(k>3);
	 }
	else
	{
		this->SetDlgItemText(IDC_STATIC_Gray,T[index]);
	}

}


void BandSythesis::OnBnClickedOk()
{

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(checked==1)
	{
	do
	{
		if(i>=3)
		{
			i=0;
		}
		BN[i]=bn[i]+1;
		i++;

	}while(i<=2);
	}

	if(checked==0)
	{
		for (int c=0;c<3;c++)
		{
			BN[c]=index+1;
		}
	}

//	ImageShow* image=new ImageShow;
//	imageS->Create(IDD_IMAGE_SHOW);
//	imageS->ShowWindow(SW_SHOW);
	imageS->Invalidate(TRUE);
	//CDialogEx::OnOK();
}


void BandSythesis::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DestroyWindow();
	//CDialogEx::OnCancel();
}


void BandSythesis::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::PostNcDestroy();
	delete this;
}


void BandSythesis::OnBnClickedRadio1()
{
	checked=1;
	CStatic* RgGray=(CStatic*)GetDlgItem(IDC_STATIC_Gray);
	RgGray->ShowWindow(SW_HIDE);

	for(int c=0;c<3;c++)
	{
	CButton* rg=(CButton*)GetDlgItem(a[c]);
	rg->ShowWindow(SW_SHOW);
	CStatic* st=(CStatic*)GetDlgItem(b[c]);
	st->ShowWindow(SW_SHOW);
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void BandSythesis::OnBnClickedRadio2()
{
	checked=0;

	for (int c=0;c<3;c++)
	{
		CButton* rg=(CButton*)GetDlgItem(a[c]);
		rg->ShowWindow(SW_HIDE);
		CStatic* st=(CStatic*)GetDlgItem(b[c]);
		st->ShowWindow(SW_HIDE);

		CStatic* RgGray=(CStatic*)GetDlgItem(IDC_STATIC_Gray);
		RgGray->ShowWindow(SW_SHOW);
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
