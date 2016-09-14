// Contrast.cpp : 实现文件
//

#include "stdafx.h"
#include "rsp.h"
#include "Contrast.h"
#include "afxdialogex.h"
//#include "ImageShow.h"
#include "rspDlg.h"
int nPosCon;
CSliderCtrl* Const;
//ImageShow* image;
// Contrast 对话框

IMPLEMENT_DYNAMIC(Contrast, CDialogEx)

Contrast::Contrast(CWnd* pParent /*=NULL*/)
	: CDialogEx(Contrast::IDD, pParent)
{

}

Contrast::~Contrast()
{
}

void Contrast::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_SLIDER1, *Const);


}


BEGIN_MESSAGE_MAP(Contrast, CDialogEx)
	ON_BN_CLICKED(IDOK, &Contrast::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Contrast::OnBnClickedCancel)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &Contrast::OnNMCustomdrawSlider1)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// Contrast 消息处理程序


void Contrast::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
//	CDialogEx::OnOK();
	flag1=3;
	//image=new ImageShow;
	//image->Create(IDD_IMAGE_SHOW);
	//image->ShowWindow(SW_SHOW);
	imageS->Invalidate(FALSE);
}


BOOL Contrast::OnInitDialog()
{
	CDialogEx::OnInitDialog();

//	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );

	Const=(CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	Const->SetRange(-255,255);
	Const->SetPos(0);
	ShowWindow(SW_SHOW);
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void Contrast::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
//	image->DestroyWindow();
	DestroyWindow();
	//CDialogEx::OnCancel();
}


void Contrast::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	nPosCon=Const->GetPos();
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void Contrast::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//TODO: 在此添加消息处理程序代码和/或调用默认值
	  CString str;
	  str.Format("%d",nPosCon);
	  SetDlgItemText(IDC_STATIC_CONS,str);
	  CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
