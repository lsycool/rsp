// CustomInputDialog.cpp: implementation of the CCustomInputDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "rsp.h"
#include "CustomInputDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CString filename;
extern CString BiaoZhi;
extern int fenge;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCustomInputDialog, CMemoryDialog)
	ON_BN_CLICKED(IDOK, &CCustomInputDialog::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


CCustomInputDialog::CCustomInputDialog()
{
	m_dlgTempl.cx = 100;  
	m_dlgTempl.cy = 80;  
	flagD="请输入数据：";
}

CCustomInputDialog::~CCustomInputDialog()
{

}
BOOL CCustomInputDialog::OnInitDialog()
{
	CMemoryDialog::OnInitDialog();
	if (BiaoZhi=="UnweightingMerge"||BiaoZhi=="IHSMERGE")
	{
		flagD="请输入保存地址：";
	}
	else if (BiaoZhi=="Sobel"||BiaoZhi=="Prewitt"||BiaoZhi=="RegionGrow")
	{
		flagD="请输入分割阈值：";
	}

	m_StaticCtrl.Create(flagD,WS_CHILD | WS_VISIBLE,CRect(30,20,180,40),this);
	m_Edit.Create(WS_CHILD | WS_VISIBLE |WS_BORDER,CRect(30,60,180,80),this,WM_USER+3000);
	m_Button.Create("OK",WS_CHILD | WS_VISIBLE,CRect(60,120,150,150),this,IDOK);

	m_Edit.SetFocus();
	MoveWindow(300,300,230,200);

	return true;
}


// //设置标题
void CCustomInputDialog::setflag(CString file)
{
	flagD=file+":";
	m_StaticCtrl.SetWindowText(flagD);
}

void CCustomInputDialog::OnBnClickedOk()
{
	if (BiaoZhi=="Sobel"||BiaoZhi=="Prewitt"||BiaoZhi=="RegionGrow")
	{
		BOOL* a=FALSE; 
		fenge=GetDlgItemInt(WM_USER+3000,a,FALSE);
		if (fenge==0)
		{
			AfxMessageBox("请输入大于0的数字！");
			return;
		}
	}
	else
	{
		CFileDialog file(false, NULL, NULL, 
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
			"所有支持文件(*.*)|*.*|位图文件(*.bmp)|*.bmp|JPG文件(*.jpg)|*.jpg;*.jpeg|tiff文件(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
		file.m_ofn.lpstrTitle="打开新影像";
		if (file.DoModal()==IDOK)
		{
			filename=file.GetPathName();
		}
		else
		{
			return;
		}
	}
	CDialogEx::OnOK();
}

void CCustomInputDialog::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DestroyWindow();
//	CMemoryDialog::OnClose();
}
