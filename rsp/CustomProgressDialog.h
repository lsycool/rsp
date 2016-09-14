// CustomProgressDialog.h: interface for the CCustomProgressDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMPROGRESSDIALOG_H__1F2F02EA_42BE_4919_B675_350FF7AE250F__INCLUDED_)
#define AFX_CUSTOMPROGRESSDIALOG_H__1F2F02EA_42BE_4919_B675_350FF7AE250F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "memorydialog.h"

class CCustomProgressDialog : public CMemoryDialog  
{
public:
	CCustomProgressDialog();
	virtual ~CCustomProgressDialog();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};

#endif // !defined(AFX_CUSTOMPROGRESSDIALOG_H__1F2F02EA_42BE_4919_B675_350FF7AE250F__INCLUDED_)
