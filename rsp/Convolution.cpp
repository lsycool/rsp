// Convolution.cpp : 实现文件
//

#include "stdafx.h"
#include "rsp.h"
#include "Convolution.h"
#include "afxdialogex.h"

extern double moban[9];
// CConvolution 对话框

IMPLEMENT_DYNAMIC(CConvolution, CDialogEx)

CConvolution::CConvolution(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConvolution::IDD, pParent)
{
	for(int i  = 0; i <9;i++)
		temp[i] = 0;
}

CConvolution::~CConvolution()
{
}

void CConvolution::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, temp[0]);
	DDX_Text(pDX, IDC_EDIT2, temp[1]);
	DDX_Text(pDX, IDC_EDIT3, temp[2]);
	DDX_Text(pDX, IDC_EDIT4, temp[3]);
	DDX_Text(pDX, IDC_EDIT5, temp[4]);
	DDX_Text(pDX, IDC_EDIT6, temp[5]);
	DDX_Text(pDX, IDC_EDIT7, temp[6]);
	DDX_Text(pDX, IDC_EDIT8, temp[7]);
	DDX_Text(pDX, IDC_EDIT9, temp[8]);

}


BEGIN_MESSAGE_MAP(CConvolution, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CConvolution::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_RADIO1, &CConvolution::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CConvolution::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO7, &CConvolution::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &CConvolution::OnBnClickedRadio8)
	ON_BN_CLICKED(IDOK, &CConvolution::OnBnClickedOk)
END_MESSAGE_MAP()


// CConvolution 消息处理程序


void CConvolution::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
		for(int i  = 0; i <9;i++)
		temp[i] = 0;
}


void CConvolution::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i  = 0; i <9;i++)
		temp[i] = 1;
	UpdateData(false);
}


void CConvolution::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	temp[0] = -1;
	temp[1] = -1;
	temp[2] = -1;
	temp[3] = -1;
	temp[4] = 8;
	temp[5] = -1;
	temp[6] = -1;
	temp[7] = -1;
	temp[8] = -1;
	UpdateData(false);
}


void CConvolution::OnBnClickedRadio7()
{
	// TODO: 在此添加控件通知处理程序代码
	temp[0] = -1;
	temp[1] = -1;
	temp[2] = -1;
	temp[3] = 0;
	temp[4] = 0;
	temp[5] = 0;
	temp[6] = 1;
	temp[7] = 1;
	temp[8] = 1;
	UpdateData(false);
}


void CConvolution::OnBnClickedRadio8()
{
	// TODO: 在此添加控件通知处理程序代码
	temp[0] = -1;
	temp[1] = 0;
	temp[2] = 1;
	temp[3] = -1;
	temp[4] = 0;
	temp[5] = 1;
	temp[6] = -1;
	temp[7] = 0;
	temp[8] = 1;
	UpdateData(false);
}


void CConvolution::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	for(int i=0;i<9;i++)
	{
		moban[i]=temp[i];
	}
	CDialogEx::OnOK();
}
