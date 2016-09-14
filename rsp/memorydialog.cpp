// memorydialog.cpp : implementation file
//

#include "stdafx.h"
#include "rsp.h"
#include "MemoryDialog.h"
#include <wchar.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMemoryDialog dialog


CMemoryDialog::CMemoryDialog(CWnd* pParent /*=NULL*/)
{
	//{{AFX_DATA_INIT(CMemoryDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_dlgTempl.cx = 200;  
	m_dlgTempl.cy = 50;  
	m_dlgTempl.style = WS_CAPTION | WS_VISIBLE | WS_DLGFRAME | 
		WS_POPUP | DS_MODALFRAME | DS_SETFONT |WS_MAXIMIZEBOX |
		WS_MINIMIZEBOX |WS_SYSMENU;
	m_dlgTempl.dwExtendedStyle = 0;
	m_dlgTempl.x = 0;
	m_dlgTempl.y = 0;
	m_dlgTempl.cdit = 0;  

	m_StrTitle = "Dialog_2";

}


void CMemoryDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMemoryDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMemoryDialog, CDialogEx)
	//{{AFX_MSG_MAP(CMemoryDialog)
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemoryDialog message handlers

BOOL CMemoryDialog::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	// TODO: Add extra initialization here
	MoveWindow(690,75,660,660);
	m_brush.CreateSolidBrush(RGB(255, 255, 255)); // 生成一白色刷子
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//创建非模态对话框
void CMemoryDialog::CreateMemoryDialog()
{
	// The first step is to allocate memory to define the dialog.  The information to be
	// stored in the allocated buffer is the following:
	//
	// 1.  DLGTEMPLATE structure
	// 2.    0x0000 (Word) indicating the dialog has no menu
	// 3.    0x0000 (Word) Let windows assign default class to the dialog
	// 4.    (Caption)  Null terminated unicode string
	// 5.    0x000B  (size of the font to be used)
	// 6.    "Arial"  (name of the typeface to be used)
	// 7.  DLGITEMTEMPLATE structure for the button  (HAS TO BE DWORD ALIGNED)
	// 8.    0x0080  to indicate the control is a button
	// 9.    (Title). Unicode null terminated string with the caption
	// 10.    0x0000   0 extra bytes of data for this control
	// 11.  DLGITEMTEMPLATE structure for the Static Text  (HAS TO BE DWORD ALIGNED)
	// 12.    0x0081 to indicate the control is static text
	// 13.   (Title). Unicode null terminated string with the text
	// 14     0x0000.  0 extra bytes of data for this control
	// 15. DLGITEMTEMPLATE structure for the Edit Control (HAS TO BE DWORD ALIGNED)
	// 16.   0x0082 to indicate an Edit control
	// 17.   (Text) - Null terminated unicode string to appear in the edit control
	// 18.   0x0000. 0 extra bytes of data for this control

	WCHAR szBoxCaption[] = L"Dialog Template Sample";
	WCHAR szFontName[] = L"ARIAL";


	WCHAR *pszBoxCaption ;
	int     nLenTitle = m_StrTitle.GetLength() + 1;
	pszBoxCaption = new WCHAR[nLenTitle];

	MultiByteToWideChar(CP_ACP, 0, m_StrTitle, -1, pszBoxCaption, nLenTitle);

	TRY  // catch memory exceptions and don't worry about allocation failures
	{
		// The following expressions have unnecessary parenthesis trying to make the
		// comments more clear.
		int nBufferSize =  sizeof(DLGTEMPLATE) + (2 * sizeof(WORD))/*menu and class*/ + nLenTitle*sizeof(WCHAR);
		nBufferSize += sizeof(WORD) + sizeof(szFontName); /* font information*/ 

		nBufferSize = (nBufferSize + 2) & ~2;  // adjust size to make first control DWORD aligned
/*
		for (i = 0; i < m_nItems; i++)
		{
			int nItemLength = sizeof(DLGITEMTEMPLATE) + 3 * sizeof(WORD);
			nItemLength += (m_rgDlgItem[i].m_strCaption.GetLength() + 1) * sizeof(WCHAR);

			if (i != m_nItems -1 )   // the last control does not need extra bytes
				nItemLength = (nItemLength + 3) & ~3;  // take into account gap so next control is DWORD aligned

			nBufferSize += nItemLength;
		}
*/

		HLOCAL hLocal;

		hLocal = LocalAlloc(LHND, nBufferSize);
		if (hLocal == NULL)
			AfxThrowMemoryException();

		BYTE*   pBuffer = (BYTE*)LocalLock(hLocal);
		if (pBuffer == NULL)
		{
			LocalFree(hLocal);
			AfxThrowMemoryException();
		}

		BYTE*   pdest = pBuffer;
		// transfer DLGTEMPLATE structure to the buffer
		memcpy(pdest, &m_dlgTempl, sizeof(DLGTEMPLATE));
		pdest += sizeof(DLGTEMPLATE);
		*(WORD*)pdest = 0; // no menu
		*(WORD*)(pdest + 1) = 0;  // use default window class
		pdest += 2 * sizeof(WORD);
		memcpy(pdest, pszBoxCaption, nLenTitle*sizeof(WCHAR));

		pdest += nLenTitle*sizeof(WCHAR);
		*(WORD*)pdest = 11;  // font size
		pdest += sizeof(WORD);
		memcpy(pdest, szFontName, sizeof(szFontName));
		pdest += sizeof(szFontName);

		delete [] pszBoxCaption;

/*
		// will now transfer the information for each one of the item templates
		for (i = 0; i < m_nItems; i++)
		{
			pdest = (BYTE*)(((DWORD)pdest + 3) & ~3);  // make the pointer DWORD aligned
			memcpy(pdest, (void *)&m_rgDlgItem[i].m_dlgItemTemplate, sizeof(DLGITEMTEMPLATE));
			pdest += sizeof(DLGITEMTEMPLATE);
			*(WORD*)pdest = 0xFFFF;  // indicating atom value
			pdest += sizeof(WORD);
			*(WORD*)pdest = m_rgDlgItem[i].m_controltype;    // atom value for the control
			pdest += sizeof(WORD);

			// transfer the caption even when it is an empty string
			WCHAR*  pchCaption;
			int     nChars, nActualChars;

			nChars = m_rgDlgItem[i].m_strCaption.GetLength() + 1;
			pchCaption = new WCHAR[nChars];
			nActualChars = MultiByteToWideChar(CP_ACP, 0, m_rgDlgItem[i].m_strCaption, -1, pchCaption, nChars);
			ASSERT(nActualChars > 0);
			memcpy(pdest, pchCaption, nActualChars * sizeof(WCHAR));
			pdest += nActualChars * sizeof(WCHAR);
			delete pchCaption;

			*(WORD*)pdest = 0;  // How many bytes in data for control
			pdest += sizeof(WORD);
		}
		ASSERT(pdest - pBuffer == nBufferSize); // just make sure we did not overrun the heap

*/
//		CDialog dlg;
//		dlg.InitModalIndirect((DLGTEMPLATE*)pBuffer);
//		dlg.DoModal();  // tadaaa!  this is the line everyone's been waiting for!!!

		CreateIndirect((DLGTEMPLATE*)pBuffer);
		
		ShowWindow(SW_SHOW);
//		InitModalIndirect((DLGTEMPLATE*)pBuffer);
//		DoModal();	
		LocalUnlock(hLocal);
		LocalFree(hLocal);
	}
	CATCH(CMemoryException, e)
	{
		AfxMessageBox("对话框创建失败!");
	}
	END_CATCH

}

//创建模态对话框
int CMemoryDialog::DoModalMemoryDialog()
{
	// The first step is to allocate memory to define the dialog.  The information to be
	// stored in the allocated buffer is the following:
	//
	// 1.  DLGTEMPLATE structure
	// 2.    0x0000 (Word) indicating the dialog has no menu
	// 3.    0x0000 (Word) Let windows assign default class to the dialog
	// 4.    (Caption)  Null terminated unicode string
	// 5.    0x000B  (size of the font to be used)
	// 6.    "Arial"  (name of the typeface to be used)
	// 7.  DLGITEMTEMPLATE structure for the button  (HAS TO BE DWORD ALIGNED)
	// 8.    0x0080  to indicate the control is a button
	// 9.    (Title). Unicode null terminated string with the caption
	// 10.    0x0000   0 extra bytes of data for this control
	// 11.  DLGITEMTEMPLATE structure for the Static Text  (HAS TO BE DWORD ALIGNED)
	// 12.    0x0081 to indicate the control is static text
	// 13.   (Title). Unicode null terminated string with the text
	// 14     0x0000.  0 extra bytes of data for this control
	// 15. DLGITEMTEMPLATE structure for the Edit Control (HAS TO BE DWORD ALIGNED)
	// 16.   0x0082 to indicate an Edit control
	// 17.   (Text) - Null terminated unicode string to appear in the edit control
	// 18.   0x0000. 0 extra bytes of data for this control

	int r;
//	WCHAR szBoxCaption[] = L"Dialog Template Sample";
	WCHAR szFontName[] = L"ARIAL";


	WCHAR *pszBoxCaption ;
	int     nLenTitle = m_StrTitle.GetLength() + 1;
	pszBoxCaption = new WCHAR[nLenTitle];

	MultiByteToWideChar(CP_ACP, 0, m_StrTitle, -1, pszBoxCaption, nLenTitle);


	TRY  // catch memory exceptions and don't worry about allocation failures
	{
		// The following expressions have unnecessary parenthesis trying to make the
		// comments more clear.
		int nBufferSize =  sizeof(DLGTEMPLATE) + (2 * sizeof(WORD))/*menu and class*/ + nLenTitle*sizeof(WCHAR);
		nBufferSize += sizeof(WORD) + sizeof(szFontName); /* font information*/ 

		nBufferSize = (nBufferSize + 2) & ~2;  // adjust size to make first control DWORD aligned
/*
		for (i = 0; i < m_nItems; i++)
		{
			int nItemLength = sizeof(DLGITEMTEMPLATE) + 3 * sizeof(WORD);
			nItemLength += (m_rgDlgItem[i].m_strCaption.GetLength() + 1) * sizeof(WCHAR);

			if (i != m_nItems -1 )   // the last control does not need extra bytes
				nItemLength = (nItemLength + 3) & ~3;  // take into account gap so next control is DWORD aligned

			nBufferSize += nItemLength;
		}
*/

		HLOCAL hLocal;

		hLocal = LocalAlloc(LHND, nBufferSize);
		if (hLocal == NULL)
			AfxThrowMemoryException();

		BYTE*   pBuffer = (BYTE*)LocalLock(hLocal);
		if (pBuffer == NULL)
		{
			LocalFree(hLocal);
			AfxThrowMemoryException();
		}

		BYTE*   pdest = pBuffer;
		// transfer DLGTEMPLATE structure to the buffer
		memcpy(pdest, &m_dlgTempl, sizeof(DLGTEMPLATE));
		pdest += sizeof(DLGTEMPLATE);
		*(WORD*)pdest = 0; // no menu
		*(WORD*)(pdest + 1) = 0;  // use default window class
		pdest += 2 * sizeof(WORD);
		memcpy(pdest, pszBoxCaption, nLenTitle*sizeof(WCHAR));

		pdest += nLenTitle*sizeof(WCHAR);
		*(WORD*)pdest = 11;  // font size
		pdest += sizeof(WORD);
		memcpy(pdest, szFontName, sizeof(szFontName));
		pdest += sizeof(szFontName);

		delete [] pszBoxCaption;

/*
		// will now transfer the information for each one of the item templates
		for (i = 0; i < m_nItems; i++)
		{
			pdest = (BYTE*)(((DWORD)pdest + 3) & ~3);  // make the pointer DWORD aligned
			memcpy(pdest, (void *)&m_rgDlgItem[i].m_dlgItemTemplate, sizeof(DLGITEMTEMPLATE));
			pdest += sizeof(DLGITEMTEMPLATE);
			*(WORD*)pdest = 0xFFFF;  // indicating atom value
			pdest += sizeof(WORD);
			*(WORD*)pdest = m_rgDlgItem[i].m_controltype;    // atom value for the control
			pdest += sizeof(WORD);

			// transfer the caption even when it is an empty string
			WCHAR*  pchCaption;
			int     nChars, nActualChars;

			nChars = m_rgDlgItem[i].m_strCaption.GetLength() + 1;
			pchCaption = new WCHAR[nChars];
			nActualChars = MultiByteToWideChar(CP_ACP, 0, m_rgDlgItem[i].m_strCaption, -1, pchCaption, nChars);
			ASSERT(nActualChars > 0);
			memcpy(pdest, pchCaption, nActualChars * sizeof(WCHAR));
			pdest += nActualChars * sizeof(WCHAR);
			delete pchCaption;

			*(WORD*)pdest = 0;  // How many bytes in data for control
			pdest += sizeof(WORD);
		}
		ASSERT(pdest - pBuffer == nBufferSize); // just make sure we did not overrun the heap

*/
//		CDialog dlg;
//		dlg.InitModalIndirect((DLGTEMPLATE*)pBuffer);
//		dlg.DoModal();  // tadaaa!  this is the line everyone's been waiting for!!!

//		CreateIndirect((DLGTEMPLATE*)pBuffer);
		
//		ShowWindow(SW_SHOW);
		InitModalIndirect((DLGTEMPLATE*)pBuffer);
		r = DoModal();	
		
		LocalUnlock(hLocal);
		LocalFree(hLocal);
	}
	CATCH(CMemoryException, e)
	{
		AfxMessageBox("Memory allocation failed!");
	}
	END_CATCH

		return r;
}


HBRUSH CMemoryDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(nCtlColor ==CTLCOLOR_DLG)
		return m_brush;  //返加绿色刷子
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CMemoryDialog::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DestroyWindow();
	CDialogEx::OnClose();
}
