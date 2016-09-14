// RGBXY.cpp : 实现文件
//

#include "stdafx.h"
#include "rsp.h"
#include "RGBXY.h"
#include "afxdialogex.h"


// CRGBXY 对话框

IMPLEMENT_DYNAMIC(CRGBXY, CDialogEx)

CRGBXY::CRGBXY(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRGBXY::IDD, pParent)
{

	R=0;
	G=0;
	B=0;
	x=0;
	y=0;
	x_geo=0;
	y_geo=0;
}

CRGBXY::~CRGBXY()
{
}

void CRGBXY::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRGBXY, CDialogEx)
END_MESSAGE_MAP()


// CRGBXY 消息处理程序


void CRGBXY::updateInfo(BYTE R, BYTE G, BYTE B, int x, int y, int xgeo, int ygeo)
{
	CString cst;
	int r=R;
	int g=G;
	int b=B;
	cs=(CStatic*)GetDlgItem(IDC_STATIC_1);
	cst.Format("%d",r);
	cs->SetWindowText(cst);
	cs=(CStatic*)GetDlgItem(IDC_STATIC_2);
	cst.Format("%d",g);
	cs->SetWindowText(cst);
	cs=(CStatic*)GetDlgItem(IDC_STATIC_3);
	cst.Format("%d",b);
	cs->SetWindowText(cst);
	cs=(CStatic*)GetDlgItem(IDC_STATIC_4);
	cst.Format("%d",x);
	cs->SetWindowText((LPCTSTR)cst);
	cs=(CStatic*)GetDlgItem(IDC_STATIC_5);
	cst.Format("%d",y);
	cs->SetWindowText((LPCTSTR)cst);
	cs=(CStatic*)GetDlgItem(IDC_STATIC_6);
	cst.Format("%dE",x_geo);
	cs->SetWindowText((LPCTSTR)cst);
	cs=(CStatic*)GetDlgItem(IDC_STATIC_7);
	cst.Format("%dN",y_geo);
	cs->SetWindowText((LPCTSTR)cst);

}
