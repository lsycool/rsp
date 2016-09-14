// FileList.cpp : 实现文件
//

#include "stdafx.h"
#include "rsp.h"
#include "FileList.h"
#include "afxdialogex.h"

#include "BandSythesis.h"
#include "CustomProgressDialog.h"

extern CString path;
extern BOOL selnum;
extern ImageShow* imageS;
CCustomProgressDialog* MemoryDialog;
ImageShow* imageS2;

// CFileList 对话框

IMPLEMENT_DYNAMIC(CFileList, CDialogEx)

CFileList::CFileList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileList::IDD, pParent)
{
	isAdded=FALSE;
	projection="";
	filename="";
	bandcount=0;
}

CFileList::~CFileList()
{
}

void CFileList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_FILE, m_TreeFile);
	DDX_Control(pDX, IDC_MFCMENUBUTTON_WDOWDISP, m_menubutton);
}


BEGIN_MESSAGE_MAP(CFileList, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFileList::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ADDFILE, &CFileList::OnBnClickedButtonAddfile)
	ON_BN_CLICKED(IDC_BUTTON_DELFILE, &CFileList::OnBnClickedButtonDelfile)
	ON_BN_CLICKED(IDC_MFCMENUBUTTON_WDOWDISP, &CFileList::OnBnClickedMfcmenubuttonWdowdisp)
	ON_BN_CLICKED(IDC_BUTTON_MG, &CFileList::OnBnClickedButtonMg)
	ON_BN_CLICKED(IDCANCEL, &CFileList::OnBnClickedCancel)
END_MESSAGE_MAP()


// CFileList 消息处理程序


BOOL CFileList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD dwStyles=GetWindowLong(m_TreeFile.m_hWnd,GWL_STYLE);//获取树控制原风格
	dwStyles|=TVS_EDITLABELS|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT;
	SetWindowLong(m_TreeFile.m_hWnd,GWL_STYLE,dwStyles);//设置风格

	this->ModifyStyleEx(0,WS_EX_APPWINDOW);

	CRect rect;
	GetClientRect(&rect);
	MoveWindow(800,75,rect.Width(),rect.Height(),false);

	pathname.clear();

	////初始化按钮菜单
	pMenu = new CMenu;
	pMenu->LoadMenu(IDR_MENU2);
	m_menubutton.m_hMenu= pMenu->GetSubMenu(1)->GetSafeHmenu();
	m_menubutton.SizeToContent();
	m_menubutton.m_bOSMenu=FALSE;

	imageS2=imageS;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


// //显示信息
void CFileList::setMessage(CString filepath,CString projection,CString cordin,CString datum,int bandcount,int sizex,int sizey,double gt1,double gt2,double gt3,double gt4,double gt5,double gt6)
{
	HTREEITEM hSel=TVI_ROOT;//m_TreeFile.GetRootItem();//取得根项句柄
	static int nAddNo=10;//编号大于10为打开数据
	TV_INSERTSTRUCT TCItem;//定义插入项数据结构

	TCItem.hParent=hSel; //设置父项句柄
	TCItem.hInsertAfter=TVI_LAST;//在最后增加
	TCItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;//设屏蔽
	TCItem.item.pszText=(LPSTR)(LPCSTR)filepath;
	TCItem.item.lParam=nAddNo+10;//索引号增加
	TCItem.item.iImage=0;//正常图标
	TCItem.item.iSelectedImage=1;//选中时图标
	HTREEITEM hInsert=m_TreeFile.InsertItem(&TCItem);//增加

	CString cimgxy;
	cimgxy.Format("影像大小：%d*%d",sizex,sizey);
	int Bands = bandcount;
	CString bnd;
	bnd.Format("波段数量：%d",Bands);
	CString g1,g2,g3;
	g1.Format("左上角点坐标X：%f\r\n Y:%f",gt1,gt4);
	g2.Format("X向分辨率：%f\r\n Y向分辨率：%f",gt2,gt6);
	g3.Format("X向旋转系数：%f\r\n Y向分辨率：%f",gt3,gt5);
	//投影变换六参数解算

	CString a[8]={projection,cordin,datum,bnd,cimgxy,g1,g2,g3};

	//添加子项
	for (int i=0;i<8;i++)
	{
		TCItem.hParent=hInsert;
		TCItem.item.pszText=(LPSTR)(LPCSTR)a[i];
		TCItem.item.lParam=nAddNo+i;//子项序号
		m_TreeFile.InsertItem(&TCItem);
		m_TreeFile.Expand(hSel,TVE_EXPAND);
	}
	pathname.push_back(filepath);
	isAdded=FALSE;
}


void CFileList::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hSel=m_TreeFile.GetSelectedItem();//取得选择项句柄
	if (hSel==NULL)
	{
		return;
	}

	CString p1=m_TreeFile.GetItemText(hSel);
	if (p1.Find(":")>=0)
	{
		path=m_TreeFile.GetItemText(hSel);
	}
	else
	{
		return;
	}
	if (imageS2->GetSafeHwnd()!=NULL)
	{
		imageS2->Invalidate(TRUE);
		imageS2->UpdateWindow();
		imageS2->ShowWindow(SW_SHOW);
		imageS2->SetWindowText(path);
	}
	//else
	//{
	//	ImageShow* imageS2=new ImageShow;
	//	imageS2->Create(IDD_IMAGE_SHOW);  有问题这个地方
	//}
//	CDialogEx::OnOK();
}


void CFileList::OnBnClickedButtonAddfile()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.bmp)|*.bmp|JPG文件(*.jpg)|*.jpg;*.jpeg|tiff文件(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="请输入待分类遥感影像";
	if (file.DoModal()==IDOK)
	{
		filename=file.GetPathName();
	}
	else	
	{
		return;
	}

	GDALDataset* poDataset;//GDAL数据集
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(filename,GA_ReadOnly);
	if(poDataset==NULL)
	{
		AfxMessageBox(_T("打开文件失败！！！"));
		return;
	}

	int nImageSizeX=poDataset->GetRasterXSize();
	int nImageSizeY=poDataset->GetRasterYSize();
	CString cimgxy;
	cimgxy.Format("影像大小：%d*%d",nImageSizeX,nImageSizeY);
	int Bands = poDataset->GetRasterCount();
	CString bnd;
	bnd.Format(" 波段数：%d",Bands);

	CString prj=poDataset->GetProjectionRef();
	CString projection="投影：";
	int a1=prj.Find("PROJCS[");
	int a2;
	if (!prj.IsEmpty())
	{
		if (a1>=0)
		{
			a2=prj.Find(",",a1);
			projection=projection+prj.Mid(a1+7,a2-(a1+7));
		}
		else
		{
			projection=projection+"geographic Lat/Lot";
		}
	}
	int a3=prj.Find("GEOGCS[");
	int a4;
	CString cordin="坐标系：";
	if (!prj.IsEmpty())
	{
		if (a3>=0)
		{
			a4=prj.Find(",",a3);
			cordin=cordin+prj.Mid(a3+7,a4-(a3+7));
		}
	}

	int a5=prj.Find("DATUM[");
	int a6;
	CString datum="椭球：";
	if (!prj.IsEmpty())
	{
		if (a5>=0)
		{
			a6=prj.Find(",",a5);
			datum=datum+prj.Mid(a5+6,a6-(a5+6));
		}
	}

	CString a[5]={projection,cordin,datum,bnd,cimgxy};


	HTREEITEM hSel=TVI_ROOT;//m_TreeFile.GetRootItem();//取得根项句柄
	static int nAddNo=100;//编号大于100为新增数据
	TV_INSERTSTRUCT TCItem;//定义插入项数据结构

	TCItem.hParent=hSel; //设置父项句柄
	TCItem.hInsertAfter=TVI_LAST;//在最后增加
	TCItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;//设屏蔽
	TCItem.item.pszText=(LPSTR)(LPCSTR)filename;
	TCItem.item.lParam=nAddNo+10;//索引号增加
	TCItem.item.iImage=0;//正常图标
	TCItem.item.iSelectedImage=1;//选中时图标
	HTREEITEM hInsert=m_TreeFile.InsertItem(&TCItem);//增加

	for (int i=0;i<5;i++)
	{
		TCItem.hParent=hInsert;
		TCItem.item.pszText=(LPSTR)(LPCSTR)a[i];
		TCItem.item.lParam=nAddNo+i;//子项序号
		m_TreeFile.InsertItem(&TCItem);
		m_TreeFile.Expand(hSel,TVE_EXPAND);
	}
	isAdded=TRUE;
	pathname.push_back(filename);
	GDALClose(poDataset);
}


void CFileList::OnBnClickedButtonDelfile()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hSel=m_TreeFile.GetSelectedItem();//取得选项句柄;
	if(hSel==NULL) return;//无任何选项则返回
	if (path==m_TreeFile.GetItemText(hSel))
	{
		if (AfxMessageBox("删除该文件相关视图也将关闭，是否关闭？",MB_OKCANCEL)==IDOK)
		{
			//imageS->DestroyWindow();
			//delete imageS;
			//imageS=NULL;
			imageS->CloseWindow();
			path="";
		}
		else
		{
			return;
		}
	}
	m_TreeFile.DeleteItem(hSel);
}


void CFileList::OnBnClickedMfcmenubuttonWdowdisp()
{
	// TODO: 在此添加控件通知处理程序代码
	switch (m_menubutton.m_nMenuResult)
	{
	case ID_WIND1:
		imageS2=imageS;
		imageS2->ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_MFCMENUBUTTON_WDOWDISP,"WIN_1");
		break;
	case ID_WIND2:
		imageS2=MemoryDialog;
		imageS2->ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_MFCMENUBUTTON_WDOWDISP,"WIN_2");
	case ID_NEWWIND:
		if (MemoryDialog->GetSafeHwnd()==NULL)
		{
			MemoryDialog=new CCustomProgressDialog;
			MemoryDialog->CreateMemoryDialog();
			InsertMenu(pMenu->GetSubMenu(1)->m_hMenu,1,MF_POPUP|MF_BYPOSITION,ID_WIND2,"窗口2");
			DrawMenuBar();
			SetFocus();
		}
		break;
	default:
		break; 
	}
}


void CFileList::OnBnClickedButtonMg()
{
	// TODO: 在此添加控件通知处理程序代码
	BandSythesis* Bandmg=new BandSythesis;
	Bandmg->Create(IDD_DIALOG_BandSy);
	Bandmg->ShowWindow(SW_SHOW);
}


void CFileList::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(SW_HIDE);
//	CDialogEx::OnCancel();
}
