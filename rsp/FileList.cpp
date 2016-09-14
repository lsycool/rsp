// FileList.cpp : ʵ���ļ�
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

// CFileList �Ի���

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


// CFileList ��Ϣ�������


BOOL CFileList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	DWORD dwStyles=GetWindowLong(m_TreeFile.m_hWnd,GWL_STYLE);//��ȡ������ԭ���
	dwStyles|=TVS_EDITLABELS|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT;
	SetWindowLong(m_TreeFile.m_hWnd,GWL_STYLE,dwStyles);//���÷��

	this->ModifyStyleEx(0,WS_EX_APPWINDOW);

	CRect rect;
	GetClientRect(&rect);
	MoveWindow(800,75,rect.Width(),rect.Height(),false);

	pathname.clear();

	////��ʼ����ť�˵�
	pMenu = new CMenu;
	pMenu->LoadMenu(IDR_MENU2);
	m_menubutton.m_hMenu= pMenu->GetSubMenu(1)->GetSafeHmenu();
	m_menubutton.SizeToContent();
	m_menubutton.m_bOSMenu=FALSE;

	imageS2=imageS;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


// //��ʾ��Ϣ
void CFileList::setMessage(CString filepath,CString projection,CString cordin,CString datum,int bandcount,int sizex,int sizey,double gt1,double gt2,double gt3,double gt4,double gt5,double gt6)
{
	HTREEITEM hSel=TVI_ROOT;//m_TreeFile.GetRootItem();//ȡ�ø�����
	static int nAddNo=10;//��Ŵ���10Ϊ������
	TV_INSERTSTRUCT TCItem;//������������ݽṹ

	TCItem.hParent=hSel; //���ø�����
	TCItem.hInsertAfter=TVI_LAST;//���������
	TCItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;//������
	TCItem.item.pszText=(LPSTR)(LPCSTR)filepath;
	TCItem.item.lParam=nAddNo+10;//����������
	TCItem.item.iImage=0;//����ͼ��
	TCItem.item.iSelectedImage=1;//ѡ��ʱͼ��
	HTREEITEM hInsert=m_TreeFile.InsertItem(&TCItem);//����

	CString cimgxy;
	cimgxy.Format("Ӱ���С��%d*%d",sizex,sizey);
	int Bands = bandcount;
	CString bnd;
	bnd.Format("����������%d",Bands);
	CString g1,g2,g3;
	g1.Format("���Ͻǵ�����X��%f\r\n Y:%f",gt1,gt4);
	g2.Format("X��ֱ��ʣ�%f\r\n Y��ֱ��ʣ�%f",gt2,gt6);
	g3.Format("X����תϵ����%f\r\n Y��ֱ��ʣ�%f",gt3,gt5);
	//ͶӰ�任����������

	CString a[8]={projection,cordin,datum,bnd,cimgxy,g1,g2,g3};

	//�������
	for (int i=0;i<8;i++)
	{
		TCItem.hParent=hInsert;
		TCItem.item.pszText=(LPSTR)(LPCSTR)a[i];
		TCItem.item.lParam=nAddNo+i;//�������
		m_TreeFile.InsertItem(&TCItem);
		m_TreeFile.Expand(hSel,TVE_EXPAND);
	}
	pathname.push_back(filepath);
	isAdded=FALSE;
}


void CFileList::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hSel=m_TreeFile.GetSelectedItem();//ȡ��ѡ������
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
	//	imageS2->Create(IDD_IMAGE_SHOW);  ����������ط�
	//}
//	CDialogEx::OnOK();
}


void CFileList::OnBnClickedButtonAddfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="�����������ң��Ӱ��";
	if (file.DoModal()==IDOK)
	{
		filename=file.GetPathName();
	}
	else	
	{
		return;
	}

	GDALDataset* poDataset;//GDAL���ݼ�
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(filename,GA_ReadOnly);
	if(poDataset==NULL)
	{
		AfxMessageBox(_T("���ļ�ʧ�ܣ�����"));
		return;
	}

	int nImageSizeX=poDataset->GetRasterXSize();
	int nImageSizeY=poDataset->GetRasterYSize();
	CString cimgxy;
	cimgxy.Format("Ӱ���С��%d*%d",nImageSizeX,nImageSizeY);
	int Bands = poDataset->GetRasterCount();
	CString bnd;
	bnd.Format(" ��������%d",Bands);

	CString prj=poDataset->GetProjectionRef();
	CString projection="ͶӰ��";
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
	CString cordin="����ϵ��";
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
	CString datum="����";
	if (!prj.IsEmpty())
	{
		if (a5>=0)
		{
			a6=prj.Find(",",a5);
			datum=datum+prj.Mid(a5+6,a6-(a5+6));
		}
	}

	CString a[5]={projection,cordin,datum,bnd,cimgxy};


	HTREEITEM hSel=TVI_ROOT;//m_TreeFile.GetRootItem();//ȡ�ø�����
	static int nAddNo=100;//��Ŵ���100Ϊ��������
	TV_INSERTSTRUCT TCItem;//������������ݽṹ

	TCItem.hParent=hSel; //���ø�����
	TCItem.hInsertAfter=TVI_LAST;//���������
	TCItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;//������
	TCItem.item.pszText=(LPSTR)(LPCSTR)filename;
	TCItem.item.lParam=nAddNo+10;//����������
	TCItem.item.iImage=0;//����ͼ��
	TCItem.item.iSelectedImage=1;//ѡ��ʱͼ��
	HTREEITEM hInsert=m_TreeFile.InsertItem(&TCItem);//����

	for (int i=0;i<5;i++)
	{
		TCItem.hParent=hInsert;
		TCItem.item.pszText=(LPSTR)(LPCSTR)a[i];
		TCItem.item.lParam=nAddNo+i;//�������
		m_TreeFile.InsertItem(&TCItem);
		m_TreeFile.Expand(hSel,TVE_EXPAND);
	}
	isAdded=TRUE;
	pathname.push_back(filename);
	GDALClose(poDataset);
}


void CFileList::OnBnClickedButtonDelfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hSel=m_TreeFile.GetSelectedItem();//ȡ��ѡ����;
	if(hSel==NULL) return;//���κ�ѡ���򷵻�
	if (path==m_TreeFile.GetItemText(hSel))
	{
		if (AfxMessageBox("ɾ�����ļ������ͼҲ���رգ��Ƿ�رգ�",MB_OKCANCEL)==IDOK)
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
			InsertMenu(pMenu->GetSubMenu(1)->m_hMenu,1,MF_POPUP|MF_BYPOSITION,ID_WIND2,"����2");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BandSythesis* Bandmg=new BandSythesis;
	Bandmg->Create(IDD_DIALOG_BandSy);
	Bandmg->ShowWindow(SW_SHOW);
}


void CFileList::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowWindow(SW_HIDE);
//	CDialogEx::OnCancel();
}
