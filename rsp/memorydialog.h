#if !defined(AFX_MEMORYDIALOG_H__096F2416_DDC1_464E_8151_069052D2F0D1__INCLUDED_)
#define AFX_MEMORYDIALOG_H__096F2416_DDC1_464E_8151_069052D2F0D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// memorydialog.h : header file
//
#include "ImageShow.h"
/////////////////////////////////////////////////////////////////////////////
// CMemoryDialog dialog

class CMemoryDialog : public ImageShow
{
// Construction
public:
	CString m_StrTitle;
	CBrush m_brush;
	int DoModalMemoryDialog();
	void CreateMemoryDialog();
	DLGTEMPLATE m_dlgTempl;
	CMemoryDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMemoryDialog)
	
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMemoryDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMORYDIALOG_H__096F2416_DDC1_464E_8151_069052D2F0D1__INCLUDED_)
