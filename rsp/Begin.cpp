// Begin.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rsp.h"
#include "Begin.h"
#include "afxdialogex.h"

int iApha;//���ƴ���͸����
// Begin �Ի���

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


// Begin ��Ϣ�������


BOOL Begin::Create(UINT lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: �ڴ����ר�ô����/����û���
	return CDialogEx::Create(lpszTemplateName, pParentWnd);
}


void Begin::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowPos(&wndTop,0,0,500,300,SWP_NOMOVE);
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^WS_EX_LAYERED); 
	SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd,GWL_EXSTYLE) &~WS_EX_APPWINDOW|WS_EX_TOOLWINDOW ); //ȡ������������ʾ
//	SetLayeredWindowAttributes(0,100,LWA_ALPHA); 
	ShowWindow(SW_SHOW);
	m_nTimerID1=SetTimer(2, 1500, 0);
	ShowWindow(SW_SHOW);	
	UpdateWindow();	
	m_nTimerID = SetTimer(1, 6000, 0);
	ASSERT(m_nTimerID);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void Begin::OnPaint()
{
	
	CPaintDC dc(this); // device context for painting
	CDC dcCompatible;//����һ������DC��������ʾ��Ļ
	dcCompatible.CreateCompatibleDC(&dc);//������װ��λͼ��Դ
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP1);//����λͼ
	dcCompatible.SelectObject(&bmp);//��λͼѡ�����DC
	CRect rect;//�������
	GetClientRect(&rect);//������DCɽ�����ݸ��Ƶ���ǰDC��;�ú�����ȡ���ڿͻ��������ꡣ�ͻ�������ָ���ͻ��������ϽǺ����½ǡ����ڿͻ�����������Դ��ڿͻ��������ϽǶ��Եģ�������Ͻ�����Ϊ��0��0��
	BITMAP bitmap;//�������
	bmp.GetBitmap(&bitmap);
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);//StretchBlt������ʾ������Ӧ���ڳߴ�仯
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}
