// ERZH.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rsp.h"
#include "ERZH.h"
#include "afxdialogex.h"
//#include "ImageShow.h"
#include "rspDlg.h"
// ERZH �Ի���
//ImageShow* er=new ImageShow;
CSliderCtrl* CSEZ;
int EZ;

IMPLEMENT_DYNAMIC(ERZH, CDialogEx)

ERZH::ERZH(CWnd* pParent /*=NULL*/)
	: CDialogEx(ERZH::IDD, pParent)
{

}

ERZH::~ERZH()
{
}

void ERZH::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ERZH, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &ERZH::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &ERZH::OnBnClickedOk)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ERZH, &ERZH::OnNMCustomdrawSliderErzh)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// ERZH ��Ϣ�������


void ERZH::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	er->DestroyWindow();
	DestroyWindow();
	//CDialogEx::OnCancel();
}


void ERZH::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	flag1=5;
	
	//er->Create(IDD_IMAGE_SHOW);
	//er->ShowWindow(SW_SHOW);
	imageS->Invalidate(FALSE);
//	CDialogEx::OnOK();
}


BOOL ERZH::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CSEZ=(CSliderCtrl*)GetDlgItem(IDC_SLIDER_ERZH);
	CSEZ->SetRange(0,255);
	CSEZ->SetPos(100);

//	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void ERZH::OnNMCustomdrawSliderErzh(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EZ=CSEZ->GetPos();
	*pResult = 0;
}


void ERZH::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString str;
	str.Format("%d",EZ);
	SetDlgItemText(IDC_STATIC_EZ,str);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
