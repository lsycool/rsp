// CustomInputDialog.h: interface for the CCustomInputDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMINPUTDIALOG_H__65C2C380_85E4_4C88_9753_4A32A01342AF__INCLUDED_)
#define AFX_CUSTOMINPUTDIALOG_H__65C2C380_85E4_4C88_9753_4A32A01342AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "memorydialog.h"

class CCustomInputDialog : public CMemoryDialog  
{
public:
	CButton m_Button;
	CStatic m_StaticCtrl;
	CEdit m_Edit;
	CString flagD;

	CCustomInputDialog();
	virtual ~CCustomInputDialog();
	virtual BOOL OnInitDialog();
	void setflag(CString file);

	DECLARE_MESSAGE_MAP()
	// //…Ë÷√±ÍÃ‚
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
};

#endif // !defined(AFX_CUSTOMINPUTDIALOG_H__65C2C380_85E4_4C88_9753_4A32A01342AF__INCLUDED_)
