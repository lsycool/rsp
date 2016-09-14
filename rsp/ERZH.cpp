// ERZH.cpp : 实现文件
//

#include "stdafx.h"
#include "rsp.h"
#include "ERZH.h"
#include "afxdialogex.h"
//#include "ImageShow.h"
#include "rspDlg.h"
// ERZH 对话框
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


// ERZH 消息处理程序


void ERZH::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
//	er->DestroyWindow();
	DestroyWindow();
	//CDialogEx::OnCancel();
}


void ERZH::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	flag1=5;
	
	//er->Create(IDD_IMAGE_SHOW);
	//er->ShowWindow(SW_SHOW);
	imageS->Invalidate(FALSE);
//	CDialogEx::OnOK();
}


BOOL ERZH::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	CSEZ=(CSliderCtrl*)GetDlgItem(IDC_SLIDER_ERZH);
	CSEZ->SetRange(0,255);
	CSEZ->SetPos(100);

//	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void ERZH::OnNMCustomdrawSliderErzh(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	EZ=CSEZ->GetPos();
	*pResult = 0;
}


void ERZH::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString str;
	str.Format("%d",EZ);
	SetDlgItemText(IDC_STATIC_EZ,str);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
