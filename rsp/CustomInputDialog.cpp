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
	flagD="���������ݣ�";
}

CCustomInputDialog::~CCustomInputDialog()
{

}
BOOL CCustomInputDialog::OnInitDialog()
{
	CMemoryDialog::OnInitDialog();
	if (BiaoZhi=="UnweightingMerge"||BiaoZhi=="IHSMERGE")
	{
		flagD="�����뱣���ַ��";
	}
	else if (BiaoZhi=="Sobel"||BiaoZhi=="Prewitt"||BiaoZhi=="RegionGrow")
	{
		flagD="������ָ���ֵ��";
	}

	m_StaticCtrl.Create(flagD,WS_CHILD | WS_VISIBLE,CRect(30,20,180,40),this);
	m_Edit.Create(WS_CHILD | WS_VISIBLE |WS_BORDER,CRect(30,60,180,80),this,WM_USER+3000);
	m_Button.Create("OK",WS_CHILD | WS_VISIBLE,CRect(60,120,150,150),this,IDOK);

	m_Edit.SetFocus();
	MoveWindow(300,300,230,200);

	return true;
}


// //���ñ���
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
			AfxMessageBox("���������0�����֣�");
			return;
		}
	}
	else
	{
		CFileDialog file(false, NULL, NULL, 
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
			"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
		file.m_ofn.lpstrTitle="����Ӱ��";
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DestroyWindow();
//	CMemoryDialog::OnClose();
}
