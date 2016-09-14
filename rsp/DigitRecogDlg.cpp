// DigitRecogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rsp.h"
#include "DigitRecogDlg.h"
#include "AddSampleDlg.h"
#include "math.h"


/////////////////////////////////////////////////////////////////////////////
// CDigitRecogDlg dialog

CDigitRecogDlg::CDigitRecogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDigitRecogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDigitRecogDlg)
	m_nDigitTemp = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDigitRecogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDigitRecogDlg)
	DDX_Text(pDX, IDC_EDIT_DIGIT, m_nDigitTemp);
	DDV_MinMaxInt(pDX, m_nDigitTemp, 0, 9);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDigitRecogDlg, CDialog)
	//{{AFX_MSG_MAP(CDigitRecogDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_INPUT_TEST, OnButtonInputTest)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_TRAIN, OnButtonInputTrain)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_PREV, OnButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_RECOG, OnButtonRecog)
	ON_BN_CLICKED(IDC_BUTTON_TEMPLATE, OnButtonTemplate)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_TEMPLATE, OnButtonSaveTemplate)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_TEMPLATE, OnButtonOpenTemplate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDigitRecogDlg message handlers

BOOL CDigitRecogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX must be in the system command range.
	
	CWnd *pWnd0 = (CWnd *)GetDlgItem(IDC_PIC_TRAIN);
	pDCOrig = pWnd0->GetDC();

	CWnd *pWnd1 = (CWnd *)GetDlgItem(IDC_PIC_TEST);
	pDCTest = pWnd1->GetDC();

	pEditResult = (CEdit *)GetDlgItem(IDC_EDIT_RESULT);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDigitRecogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{	
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDigitRecogDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDigitRecogDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDigitRecogDlg::OnButtonInputTest() 
{
	CString FilePathName;

	CAddSampleDlg FileDlg(TRUE, "", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, 
		"BMP��ʽ�ַ���������ͼ��(*.bmp)|*.bmp|�����ļ�(*.*)|*.*||",
		AfxGetMainWnd());

	if (FileDlg.DoModal () != IDOK)
		return;

	CString strFile;
	CString strInfo;
	arTestSetFilePath.RemoveAll();
	POSITION pos = FileDlg.GetStartPosition();
	pEditResult->SetSel(0,-1);
	pEditResult->ReplaceSel("");
	strInfo.Format("������������\r\n");
	while( pos ) 
	{
		strFile =FileDlg.GetNextPathName( pos );//��ȡ�ļ���
		arTestSetFilePath.Add(strFile);
		strInfo += strFile;
		strInfo += "\r\n";
		pEditResult->SetSel(0,-1);
		pEditResult->ReplaceSel(strInfo);
	}
	if((iTotalTestNum = arTestSetFilePath.GetSize()) > 0){
		iCurTestNum = 0;
		strFile = arTestSetFilePath.GetAt(0);
		digitTest.digitdib.Load(strFile);
		digitTest.digitdib.Display(pDCTest, 37, 40);
		digitTest.CopyBitmapToAr();
	}
}

void CDigitRecogDlg::OnButtonInputTrain() 
{
	// TODO: Add your control notification handler code here
	CString FilePathName;

	CAddSampleDlg FileDlg(TRUE, "", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, 
		"BMP��ʽ�ַ�ͼ��(*.bmp)|*.bmp|�����ļ�(*.*)|*.*||",
		AfxGetMainWnd());

	if (FileDlg.DoModal () != IDOK)
		return;

	CString strFile;
	CString strInfo;
	arTrainSetFilePath.RemoveAll();
	POSITION pos = FileDlg.GetStartPosition();
	pEditResult->SetSel(0,-1);
	pEditResult->ReplaceSel("");
	strInfo.Format("ѵ������ͼƬ��\r\n");
	strFile =FileDlg.GetNextPathName( pos );//��ȡ�ļ���
	arTrainSetFilePath.Add(strFile);
	strInfo += strFile;
	strInfo += "\r\n";
	pEditResult->SetSel(0,-1);
	pEditResult->ReplaceSel(strInfo);

	if((iTotalTrainNum = arTrainSetFilePath.GetSize()) > 0){
		strFile = arTrainSetFilePath.GetAt(0);
		iCurTrainNum = 0;
		digitTrain.digitdib.Load(strFile);
		digitTrain.digitdib.Display(pDCOrig, 37, 40);
		digitTrain.CopyBitmapToAr();
	}
}

void CDigitRecogDlg::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	if(iCurTestNum >= iTotalTestNum-1)
		return;
	CString strFile;
	iCurTestNum++;
	strFile = arTestSetFilePath.GetAt(iCurTestNum);
	digitTest.digitdib.Load(strFile);
	digitTest.digitdib.Display(pDCTest, 37, 40);
	digitTest.CopyBitmapToAr();
}

void CDigitRecogDlg::OnButtonPrev() 
{
	// TODO: Add your control notification handler code here
	if(iCurTestNum <= 0)
		return;
	CString strFile;
	iCurTestNum--;
	strFile = arTestSetFilePath.GetAt(iCurTestNum);
	digitTest.digitdib.Load(strFile);
	digitTest.digitdib.Display(pDCTest, 37, 40);
	digitTest.CopyBitmapToAr();
}

void CDigitRecogDlg::OnButtonRecog() 
{
	// TODO: Add your control notification handler code here
	digitTest.BinaryDigit(120);
	digitTest.ThinDigit_1();
	pEditResult->SetSel(0,-1);
	pEditResult->ReplaceSel("");
	CString strInfo, strTemp;
	strInfo.Format("");
	int i,j;
	for(i=0; i<digitTest.digitHeight; i++)
	{
		for(j=0; j<digitTest.digitWidth; j++)
		{
			if(digitTest.digitarray[j][i] == 1)
				strInfo += "��";
			else
				strInfo += "  ";
		}
		strInfo += "\r\n";
	}
	pEditResult->SetSel(0,-1);
	pEditResult->ReplaceSel(strInfo);
	digitTest.GetFeature();
	double min=100000.0;
	int classResult=-1;
	for(i=0; i<10; i++)
	{
		double diff = 0.0;

		for(j=0; j<9; j++)
		{
			diff += fabs(digitTest.feature[j] - digitTemplate[i].feature[j]);
		}

		for(j=9; j<13; j++)
		{
			diff += fabs(digitTest.feature[j] - digitTemplate[i].feature[j])*9;
		}

		if(diff < min)
		{
			min = diff;
			classResult = i;
		}
	}

	strTemp.Format("%d", classResult);
	strInfo += "\n\rʶ����Ϊ��  ";
	strInfo += strTemp;
	AfxMessageBox(strInfo);

	pEditResult->SetSel(0,-1);
	pEditResult->ReplaceSel(strInfo);
}

//����ģ��
void CDigitRecogDlg::OnButtonTemplate() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_nDigitTemp<0 || m_nDigitTemp>9)
	{
		MessageBox("������ģ���ʾ�����֣�");
		return;
	}
	digitTrain.BinaryDigit(120);
	digitTrain.ThinDigit_1();
	pEditResult->SetSel(0,-1);
	pEditResult->ReplaceSel("");
	CString strInfo, strTemp;
	strInfo.Format("");
	int i,j;
	for(i=0; i<digitTrain.digitHeight; i++)
	{
		for(j=0; j<digitTrain.digitWidth; j++)
		{
			if(digitTrain.digitarray[j][i] == 1)
				strInfo += "��";
			else
				strInfo += "  ";
		}
		strInfo += "\r\n";
	}
	pEditResult->SetSel(0,-1);
	pEditResult->ReplaceSel(strInfo);

	digitTrain.GetFeature();
	
	for(i=0; i<13; i++)
		digitTemplate[m_nDigitTemp].feature[i] = digitTrain.feature[i];
	digitTemplate[m_nDigitTemp].trueClass = m_nDigitTemp;
}

void CDigitRecogDlg::OnButtonSaveTemplate() 
{
	// TODO: Add your control notification handler code here
	CFile cf;
	if(cf.Open("D:\\USPS\\mydata.dat",CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite)==NULL)
	{
		AfxMessageBox("���ļ�ʧ�ܣ�\n������˳�����");
		return;
	}

	Sample1 sampleTemp;
	try
	{
		cf.Write((void *)digitTemplate, 10*sizeof(sampleTemp));
	}
	catch(CFileException *e){
	//Alert user to error
		e->Delete();
	}//end try-catch
}

void CDigitRecogDlg::OnButtonOpenTemplate() 
{
	// TODO: Add your control notification handler code here
	CFile cf;
	if(cf.Open("D:\\USPS\\mydata.dat",CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite)==NULL)
	{
		AfxMessageBox("���ļ�ʧ�ܣ�\n������˳�����");
		return;
	}

	Sample1 sampleTemp;
	try
	{
		cf.Read((void *)digitTemplate, 10*sizeof(sampleTemp));	
	}
	catch(CFileException *e){
	//Alert user to error
		e->Delete();
	}//end try-catch
}
