// CustomProgressDialog.cpp: implementation of the CCustomProgressDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "rsp.h"
#include "CustomProgressDialog.h"
#include "BandSythesis.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
BOOL check=FALSE;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomProgressDialog::CCustomProgressDialog()
{
	m_dlgTempl.cx = 180;  
	m_dlgTempl.cy = 40;  
}

CCustomProgressDialog::~CCustomProgressDialog()
{

}
BOOL CCustomProgressDialog::OnInitDialog() 
{

	CMemoryDialog::OnInitDialog();

	this->ModifyStyleEx(0,WS_EX_APPWINDOW);

	return true;
}
BEGIN_MESSAGE_MAP(CCustomProgressDialog, CMemoryDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CCustomProgressDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	if (check)
	{
		ShowImage(BN[0],BN[1],BN[2],&dc);//初始显示图像
	}
	check=TRUE;
	// 不为绘图消息调用 CMemoryDialog::OnPaint()
}
