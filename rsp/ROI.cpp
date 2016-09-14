// ROI.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rsp.h"
#include "ROI.h"
#include "afxdialogex.h"


// CROI �Ի���

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


// CROI ��Ϣ�������

//���ROI
void CROI::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int a=m_list.GetItemCount();
	m_list.InsertItem(a,"*",0);
	m_list.SetItemText(a,0,"����");
	m_list.SetItemText(a,1,"ѡ����ɫ");
}

BOOL CROI::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	this->ModifyStyleEx(0,WS_EX_APPWINDOW);
	MoveWindow(800,76,326,337);

	ListView_SetExtendedListViewStyle(m_list.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_OWNERDRAWFIXED|LVS_REPORT); 
	m_list.InsertColumn(0,"����",LVCFMT_CENTER,146,COLUMN_STYLE_EDIT);
	m_list.InsertColumn(1,"��ɫ",LVCFMT_CENTER,146,COLUMN_STYLE_COMBOBOX,NULL);
	m_list.InsertItem(0,"*",0);
	m_list.SetItemText(0,0,"����");
	m_list.SetItemText(0,1,"ѡ����ɫ");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

LRESULT CROI::OnItemChanged(WPARAM wParam,LPARAM lParam)
{
	CString str=m_list.GetItemText((int)wParam,(int)lParam);
	TRACE("========%s\n",str);//���������Ϣ
	return 0;
}
//ɾ��ROI
void CROI::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_list.GetFirstSelectedItemPosition();//���ѡȡ�Ŀ��Ƶ�λ��
	if(!pos)
		return;
	while (pos)
	{ 
		int nItem = m_list.GetNextSelectedItem(pos);//�����һ�����Ƶ�����
		m_list.DeleteItem(nItem);
	}

}
