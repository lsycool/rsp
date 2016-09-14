// ROI.cpp : 实现文件
//

#include "stdafx.h"
#include "rsp.h"
#include "ROI.h"
#include "afxdialogex.h"


// CROI 对话框

IMPLEMENT_DYNAMIC(CROI, CDialogEx)

CROI::CROI(CWnd* pParent /*=NULL*/)
	: CDialogEx(CROI::IDD, pParent)
{

}

CROI::~CROI()
{
}

void CROI::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_list);

}

BEGIN_MESSAGE_MAP(CROI, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CROI::OnBnClickedButton1)
	ON_MESSAGE(WM_ITEM_CHANGED,OnItemChanged)
	ON_BN_CLICKED(IDC_BUTTON2, &CROI::OnBnClickedButton2)
END_MESSAGE_MAP()


// CROI 消息处理程序

//添加ROI
void CROI::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	int a=m_list.GetItemCount();
	m_list.InsertItem(a,"*",0);
	m_list.SetItemText(a,0,"新类");
	m_list.SetItemText(a,1,"选择颜色");
}

BOOL CROI::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->ModifyStyleEx(0,WS_EX_APPWINDOW);
	MoveWindow(800,76,326,337);

	ListView_SetExtendedListViewStyle(m_list.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_OWNERDRAWFIXED|LVS_REPORT); 
	m_list.InsertColumn(0,"类名",LVCFMT_CENTER,146,COLUMN_STYLE_EDIT);
	m_list.InsertColumn(1,"颜色",LVCFMT_CENTER,146,COLUMN_STYLE_COMBOBOX,NULL);
	m_list.InsertItem(0,"*",0);
	m_list.SetItemText(0,0,"新类");
	m_list.SetItemText(0,1,"选择颜色");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

LRESULT CROI::OnItemChanged(WPARAM wParam,LPARAM lParam)
{
	CString str=m_list.GetItemText((int)wParam,(int)lParam);
	TRACE("========%s\n",str);//输出调试信息
	return 0;
}
//删除ROI
void CROI::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_list.GetFirstSelectedItemPosition();//获得选取的控制点位置
	if(!pos)
		return;
	while (pos)
	{ 
		int nItem = m_list.GetNextSelectedItem(pos);//获得下一个控制点索引
		m_list.DeleteItem(nItem);
	}

}
