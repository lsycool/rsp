// Begin.cpp : 实现文件
//

#include "stdafx.h"
#include "rsp.h"
#include "Begin.h"
#include "afxdialogex.h"

int iApha;//控制窗口透明度
// Begin 对话框

IMPLEMENT_DYNAMIC(Begin, CDialogEx)

Begin::Begin(CWnd* pParent /*=NULL*/)
	: CDialogEx(Begin::IDD, pParent)
{

}

Begin::~Begin()
{
}

void Begin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Begin, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// Begin 消息处理程序


BOOL Begin::Create(UINT lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CDialogEx::Create(lpszTemplateName, pParentWnd);
}


void Begin::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_nTimerID == nIDEvent) 
	{       
		//Destroy the timer and begin window
		KillTimer(m_nTimerID);
		DestroyWindow();         
		delete this;
		return;
	}        
	if (m_nTimerID1 == nIDEvent)
	{
		if (iApha<=151)
		{
			iApha=iApha+50;
			SetLayeredWindowAttributes(0,100+iApha,LWA_ALPHA); 
			ShowWindow(SW_SHOW);
		}
		else
		{
			KillTimer(m_nTimerID1);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL Begin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowPos(&wndTop,0,0,500,300,SWP_NOMOVE);
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^WS_EX_LAYERED); 
	SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd,GWL_EXSTYLE) &~WS_EX_APPWINDOW|WS_EX_TOOLWINDOW ); //取消在任务栏显示
//	SetLayeredWindowAttributes(0,100,LWA_ALPHA); 
	ShowWindow(SW_SHOW);
	m_nTimerID1=SetTimer(2, 1500, 0);
	ShowWindow(SW_SHOW);	
	UpdateWindow();	
	m_nTimerID = SetTimer(1, 6000, 0);
	ASSERT(m_nTimerID);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void Begin::OnPaint()
{
	
	CPaintDC dc(this); // device context for painting
	CDC dcCompatible;//创建一个兼容DC，用于显示屏幕
	dcCompatible.CreateCompatibleDC(&dc);//创建及装入位图资源
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP1);//载入位图
	dcCompatible.SelectObject(&bmp);//将位图选入兼容DC
	CRect rect;//定义对象
	GetClientRect(&rect);//将兼容DC山的内容复制到当前DC上;该函数获取窗口客户区的坐标。客户区坐标指定客户区的左上角和右下角。由于客户区坐标是相对窗口客户区的左上角而言的，因此左上角坐标为（0，0）
	BITMAP bitmap;//对象包含
	bmp.GetBitmap(&bitmap);
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);//StretchBlt调整显示背景适应窗口尺寸变化
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}
